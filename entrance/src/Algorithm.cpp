/*
 * Algorithm.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: "Alessio Colombo <colombo@disi.unitn.it>"
 */


//#define DEBUG
#include "../../common/debug.hpp"
#include <QApplication>

#include "gui/mainwindow.h"
#include "Algorithm.hpp"
#include "../../common/TimeUtils.hpp"
#include "server/EntranceServer.hpp"
//#include "HwMgr/HardwareManager.h"
#include "../../apartment/src/client/ApartmentClient.hpp"

using namespace std;

Algorithm::Algorithm(Algorithm::input_options_t options) :
  single_thread(thread_fun_t([this](const bool& t) { this->worker(t); }), "Algorithm"),
  input_options(options) {}


/// @brief Destructor.
Algorithm::~Algorithm() {}

/// @brief Set the period of the publisher.
void Algorithm::setPublishPeriod(Cit_Types::Time_ms_t ms) {

  /* The minimum period is hard-coded to 4m */
  input_options.period_ms = ms > 4 ? ms : 4;

}

/// @brief Get the period of the publisher.
void Algorithm::getPublishPeriod(Cit_Types::Time_ms_t& ms) {

  /* Return the period of the publisher */
  ms = input_options.period_ms;

}

/// @brief Executes the algorithm.
void Algorithm::worker(const bool& terminating) {
  try{
    // start entrance server
    EntranceServer entrance_server(input_options.entrance_server);
    /// Link the server
    if(!entrance_server.Start()){
      std::cerr << "Algorithm::worker() error in starting the entrance_server at : "
                << input_options.entrance_server << std::endl;
      return;
    }
    std::cout << "Algorithm::worker() entrance_server started at: " << input_options.entrance_server << std::endl;

    // start apartment client
    ApartmentClient apartment_client(input_options.apartment_server);
    /// Link the server
    if(!apartment_client.Start()){
      std::cerr << "Algorithm::worker() error in starting the apartment_client at : "
                << input_options.apartment_server << std::endl;
      return;
    }
    std::cout << "Algorithm::worker() apartment_client started at: " << input_options.apartment_server << std::endl;

    int argc = 0;
    QApplication gui(argc, nullptr);
    MainWindow gui_window(apartment_client);
    gui_window.show();
    gui.exec();
    /*
    // function pointer (lambda) to ringbell function
    const HardwareManager::ringBellFun_t ringBell_fptr = [&apartment_client](){return apartment_client.ringBell();};

    if(!HardwareManager::initHardware(ringBell_fptr)){
    std::cerr << "Algorithm::worker() error in the initialization of hardware manager" << std::endl;
    }


    // Create a element of data to be streamed
    // LocalizationPublisher::data_t streamer_data;
    TimeUtils::PeriodicTimer<Cit_Types::Time_ms_t, TimeUtils::Granularity::MILLI> timer(input_options.period_ms);
    timer.start(0);
    // Cit_Types::Time_ms_t time_ns;

    while(!terminating){

    ///< Waits for the next activation
    timer.wait_for_next_activation();
    // fill the streamer data structure with the status of the nodes

    SDEBUG("Algorithm::worker i'm still alive");
    }*/
  }
  catch(exception& e){
    cout << "Algorithm::worker(), exception generated, exiting." << endl;
    cout << "\"" << e.what() << "\"" << endl;
    cout.flush();
  }
}
