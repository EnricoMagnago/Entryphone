/*
 * Algorithm.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: "Alessio Colombo <colombo@disi.unitn.it>"
 */


//#define DEBUG
#include "../../common/debug.hpp"

#include "Algorithm.hpp"
#include "../../common/TimeUtils.hpp"
#include "server/EntranceServer.hpp"
#include "HwMgr/HardwareManager.h"
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
		std::cout << "Algorithm::worker() starting entrance_server at: " << input_options.entrance_server << std::endl;
		EntranceServer entrance_server(input_options.entrance_server);
		/// Link the server
		if(!entrance_server.Start()){
			std::cerr << "Algorithm::worker() error in starting the entrance_server at : "
			          << input_options.entrance_server << std::endl;
			return;
		}
		std::cout << "Algorithm::worker() entrance_server started at: " << input_options.entrance_server << std::endl;

		// start apartment client
		std::cout << "Algorithm::worker() starting apartment_client" << std::endl;
		ApartmentClient apartment_client(input_options.apartment_server);
		/// Link the server
		if(!apartment_client.Start()){
			std::cerr << "Algorithm::worker() error in starting the apartment_client at : "
			          << input_options.apartment_server << std::endl;
			return;
		}
		std::cout << "Algorithm::worker() apartment_client started at: " << input_options.apartment_server << std::endl;

		// function pointer (lambda) to ringbell function
		const HardwareManager::ringBellFun_t ringBell_fptr = [&apartment_client](){return apartment_client.ringBell();};
		const HardwareManager hwMgr(ringBell_fptr);


		// Create a element of data to be streamed
		// LocalizationPublisher::data_t streamer_data;
		TimeUtils::PeriodicTimer_t<Cit_Types::Time_ms_t, std::milli> timer(input_options.period_ms);
		timer.start();
		// Cit_Types::Time_ms_t time_ns;

		while(!terminating){

			///< Waits for the next activation
			timer.wait_for_next_activation();
			// fill the streamer data structure with the status of the nodes

			SDEBUG("Algorithm::worker i'm still alive");
		}
	}
	catch(exception& e){
		cout << "Algorithm::worker(), exception generated, exiting." << endl;
		cout << "\"" << e.what() << "\"" << endl;
		cout.flush();
	}
}
