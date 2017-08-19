/*!****************************************************************************
 *    \file ApartmentServerDefinitions.hpp
 *    \brief
 *    \version 1.0
 *    \date    2017
 *****************************************************************************/
/// @file ApartmentServerDefinitions.hpp
/// @brief
/// @author Enrico Magnago
///         Valerio Magnago

#ifndef __APARTMENT_SERVER_DEFINITIONS_HPP__
#define __APARTMENT_SERVER_DEFINITIONS_HPP__

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "../../../common/bimap.hpp"


class ApartmentServerDefinitions {
public:

    enum class cmd_t {
        RING_BELL
    };


    // remote response
    enum class response_t {
        OK,
        FAIL,
        TIMEOUT
    };

    // JSON fields
    static const std::string response_field;   // Succes of not of the requested action
    static const std::string cmd_field;   // Command field to choose the action of the server

    static const std::string value_field; // generic value type

    // QR LIST INFORMATION
    static const std::string x_field;
    static const std::string y_field;
    static const std::string theta_field;

    // Convert cmd_t in string and viceversa
    static const bimap<cmd_t, std::string> cmd_bimap;
    static const bimap<response_t, std::string> response_bimap;

};


#endif /* __APARTMENT_SERVER_DEFINITIONS_HPP__ */
