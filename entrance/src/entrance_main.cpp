/*!****************************************************************************
 *    \file    main.cpp
 *    \brief   Main program of the entrata videocitofono.
 *    \version 1.0
 *    \date    2017
 *****************************************************************************/
/// @file   main.cpp
/// @brief  Main program of the entrata videocitofono.
/// @author Enroco Magnago
///         Valerio Magnago

#include <iostream>
#include <signal.h>
#include <condition_variable>
#include <thread>
#include <zconf.h>

#include "Algorithm.hpp"
#include "ConsoleInterface.hpp"

using namespace std;

static bool interrupted = false;

static condition_variable wait_cond;
static mutex wait_mutex;

static void signal_handler(int /*sig*/);

int main() {
    // Register the signal handler for the SIGINT signal (Ctrl+C)
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    Algorithm* algorithm;
    ConsoleInterface* ci;

    bool error = false, force_term = false;

    try {
        Algorithm::input_options_t options;
        options.period_ms = 1e3; //1e4;
        options.entrance_publisher     = "tcp://127.0.0.1:6666";
        options.entrance_server        = "tcp://127.0.0.1:6670";

        algorithm = new Algorithm(options);
        ci = new ConsoleInterface(algorithm);
    } catch (const exception& ex) {
        cerr << ex.what();
        cerr.flush();
        cerr << endl;
        cout << "Can't create Algorithm(), Done with errors." << endl;
        cout.flush();
        return 1;
    }

    try {
        algorithm->setDefaultTimeoutMilli(2000);
        ci->setDefaultTimeoutMilli(2000);

        // waiting
        cout << "CTRL-C or \"kill -2 " << getpid() << "\" to quit." << endl;
        cout.flush();
        // starting
        if (!algorithm->start() || ! ci->start()) {
            // error starting...
            cerr << "Can't start, cannot continue. Exiting." << endl;
            cerr.flush();
            error = true;
        } else {
            // correctly started
            while (!interrupted && algorithm->isAlive() && ci->isAlive()) {
                unique_lock<mutex> lock(wait_mutex);
                wait_cond.wait_for(lock, chrono::milliseconds(1000));
            }
            // stopping
            cout << "Stopping ("<< algorithm->getDefaultTimeoutMilli() << " ms timeout)..."
                 << endl;
            cout.flush();
            if (!algorithm->stop()) {
                // error stopping...
                cerr << "Can't stop, cannot continue. Forcing exit..." << endl;
                cerr.flush();
                force_term = true;
            }
            if (!ci->stop()) {
                // error stopping...
                cerr << "Can't stop, cannot continue. Forcing exit..." << endl;
                cerr.flush();
                force_term = true;
            }
        }
    } catch (const std::runtime_error& re) {
        cerr << "Runtime error: ";
        cerr << re.what() << endl;
        error = true;
    } catch (const exception& ex) {
        cerr << "Exception: ";
        cerr << ex.what() << endl;
        error = true;
    }

    if (error || force_term) {
        cout << "Done with errors." << endl;
        cout.flush();
        if (force_term) {
            // last resort :(
            cout << "Calling std::terminate()." << endl;
            cout.flush();
            std::terminate();
        } else {
            delete(algorithm);
            delete(ci);
            return 1;
        }
    } else {
        cout << "Done." << endl;
        cout.flush();
        delete(algorithm);
        delete(ci);
        return 0;
    }
}

void signal_handler(int /*sig*/) {
    if (!interrupted) {
        cout << "Signal caught, please wait for termination." << endl;
        cout.flush();
    }
    interrupted = true;
    wait_cond.notify_all();
}
