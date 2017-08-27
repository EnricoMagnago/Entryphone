/*!****************************************************************************
 *    \file    main.cpp
 *    \brief   Main program of the aruco localization module.
 *    \version 1.0
 *    \date    2017
 *    \authors Alessio Colombo          <alessio.colombo_1@unitn.it>
 *             Bernardo Villalba Frías  <br.villalbafrias@unitn.it>
 *             Valerio Magnago          <valerio.magnago@unitn.it>
 *****************************************************************************/


#include <iostream>
#include <signal.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>    // for getpid() function

#include "Algorithm.hpp"

using namespace std;

static bool interrupted = false;

static condition_variable wait_cond;
static mutex wait_mutex;

static void signal_handler(int /*sig*/);

/* frame resolution */
const uint32_t FRAME_HEIGHT_PX  = 480;
const uint32_t FRAME_WIDTH_PX   = 640;
const uint32_t CAMERA_FPS       =  10;
const uint32_t CAMERA_ID        =   0;

using namespace VideoStreamer;

int main() {
    /* Initial message */
    cout << "****************************************" << endl;
    cout << "              VIDEO STREAMER              " << endl;
    cout << " !!if you are a man don't get naked now!!   " << endl;
    cout << "   if you are a girl ...                 "    << endl;
    cout << "****************************************" << endl << endl;


    // Register the signal handler for the SIGINT signal (Ctrl+C)
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* Input option for alghorithm class */
    unique_ptr<Algorithm::input_options_t> alg_opt =  unique_ptr<Algorithm::input_options_t>(new Algorithm::input_options_t());
    alg_opt->period_ms        = 50;
    //alg_opt->publisher        = "tcp://127.0.0.1:6669";
    alg_opt->publisher        = "tcp://192.168.1.254:6669";

    Algorithm algorithm(alg_opt);

    bool error = false, force_term = false;


    try {
        algorithm.setDefaultTimeoutMilli(1000);  // 1 sec timeout

        // waiting
        cout << "CTRL-C or \"kill -2 " << getpid() << "\" to quit." << endl;
        cout.flush();
        // starting
        if (!algorithm.start()) {
            // error starting...
            cerr << "Can't start, cannot continue. Exiting." << endl;
            cerr.flush();
            error = true;
        } else {
            // correctly started
            while (!interrupted && algorithm.isAlive()) {
                unique_lock<mutex> lock(wait_mutex);
                wait_cond.wait_for(lock, chrono::milliseconds(1000));
            }
            // stopping
            cout << "Stopping ("<< algorithm.getDefaultTimeoutMilli() << " ms timeout)..."
                 << endl;
            cout.flush();
            if (!algorithm.stop()) {
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
            return 1;
        }
    } else {
        /* Final message */
        cout << endl << "***************************" << endl;
        cout << "   VIDEO STREAMER Finished   " << endl;
        cout << "   see you later alligator  " << endl;
        cout << "*******************************" << endl << endl;
        cout.flush();
        cout.flush();
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
