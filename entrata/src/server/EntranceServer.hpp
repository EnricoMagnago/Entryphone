/*!****************************************************************************
*    \file    EntranceServer.hpp
*    \brief
*    \version 1.0
*    \date    2017
*****************************************************************************/
/// @file   Entrance.hpp
/// @brief
/// @author Enrico Magnago
///         Valerio Magnago   <valerio.magnago@unitn.it>
///

#ifndef ENTRANCE_SERVER_H_
#define ENTRANCE_SERVER_H_

#include "../../../common/zmq_layer/Replier.hpp"
#include "../interface/EntranceServerDefinitions.hpp"


class EntranceServer:  private Replier, private EntranceServerDefinitions {
public:
    /**
     * Default constructor
     * @param kf a pointer to the kalman filter algoritm
     * @return
     */
    EntranceServer();

    /**
     * Destructor of the server, it baisicalli run a stop
     */
    ~EntranceServer();

    /// start server action
    bool Start(std::string serverAddress);


private:

    /**
     * ProcessMessage take a string in input
     * @param received  the input string requested at the replier
     * @param data      optional data
     * @param tosend    reply to send at the requester
     */
    void processMessage(const std::string& received, std::string& tosend);


};

#endif /* ENTRANCE_SERVER_H_ */
