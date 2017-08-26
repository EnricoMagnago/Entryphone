/*!****************************************************************************
*    \file    ApartmentClient.hpp
*    \brief
*    \version 1.0
*    \date    2017
*****************************************************************************/
/// @file   ApartmentClient.hpp
/// @brief
/// @author Enrico Magnago
///         Valerio Magnago   <valerio.magnago@unitn.it>
///

#ifndef __APARTMENT_CLIENT_HPP__
#define __APARTMENT_CLIENT_HPP__

#include "../../../common/zmq_layer/Requester.hpp"
#include "../interface/ApartmentServerDefinitions.hpp"
#include "../../../common/json/Json.hpp"
#include <cstdint>


class ApartmentClient : private Requester, private ApartmentServerDefinitions {
public:
	/**
	 * Default constructor
	 */
	ApartmentClient(const std::string address);

	/**
	 * Destructor of the client, it basically run a stop
	 */
	~ApartmentClient();

	/**
	 *
	 * @param serverAddress
	 * @return
	 */
	bool Start();

	bool ringBell();

private:
	bool request(const nlohmann::json& in, nlohmann::json& out);
};

#endif /* __APARTMENT_CLIENT_HPP__ */
