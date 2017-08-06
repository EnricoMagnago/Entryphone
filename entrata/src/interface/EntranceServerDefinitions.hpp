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

#ifndef VIDEOCITOFONO_SERVER_DEFINITIONS_HPP__
#define VIDEOCITOFONO_SERVER_DEFINITIONS_HPP__

#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "../../../common/bimap.hpp"


class EntranceServerDefinitions {
public:

    enum class cmd_t {
        OPEN_DOOR
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


#endif /* REMOTELOCALIZATIONDEFINITIONS_H_ */
