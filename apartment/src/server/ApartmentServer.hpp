/*!****************************************************************************
*    \file    ApartmentServer.hpp
*    \brief
*    \version 1.0
*    \date    2017
*****************************************************************************/
/// @file   ApartmentServer.hpp
/// @brief
/// @author Enrico Magnago
///         Valerio Magnago   <valerio.magnago@unitn.it>
///

#ifndef __ENTRYPHONE_APARTMENT_SERVER_H__
#define __ENTRYPHONE_APARTMENT_SERVER_H__

#include "../../../common/zmq_layer/Replier.hpp"
#include "../interface/ApartmentServerDefinitions.hpp"


class ApartmentServer:  private Replier, private ApartmentServerDefinitions {
public:
    /**
     * Default constructor
     * @param kf a pointer to the kalman filter algoritm
     * @return
     */
    ApartmentServer();

    /**
     * Destructor of the server, it baisicalli run a stop
     */
    ~ApartmentServer();

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

#endif /* __ENTRYPHONE_APARTMENT_SERVER_H__ */
