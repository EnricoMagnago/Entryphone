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
	std::cout << "Algorithm::worker() started correctly" << std::endl;

	ApartmentClient app_client(input_options.apartment_server);
	const HardwareManager hwMgr(HardwareManager::ringBellFun_t([&app_client] { app_client.ringBell((uint16_t)0); }));

	std::cout << "Algorithm::worker() starting server at: " << input_options.entrance_server << std::endl;
	EntranceServer ent_server(input_options.entrance_server);
	try{

		/// Link the server
		if(!ent_server.Start()){
			std::cerr << "Algorithm::worker() error in starting the server at : " << input_options.entrance_server
			          << std::endl;
		}
		std::cout << "Algorithm::worker() server started at: " << input_options.entrance_server << std::endl;


		// Create a element of data to be streamed
		// LocalizationPublisher::data_t streamer_data;
		TimeUtils::PeriodicTimer_t<Cit_Types::Time_ms_t, std::milli> timer(input_options.period_ms);
		timer.start();
//        Cit_Types::Time_ms_t time_ns;

		while(!terminating){

			///< Waits for the next activation
			timer.wait_for_next_activation();
			std::cout << "invio dati" << std::endl;
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
