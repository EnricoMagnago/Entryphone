/*!****************************************************************************
 *    \file    Algorithm.hpp
 *    \brief
 *    \version 1.0
 *    \date    2017
 *****************************************************************************/
/// @file   Algorithm.cpp
/// @brief
/// @author Enrico Magnago
///         Valerio Magnago
#ifndef __ENTRYPHONE_ENTRY_ALGORITHM_H__
#define __ENTRYPHONE_ENTRY_ALGORITHM_H__

#include <fstream>
#include <string>
#include <memory>
#include "../../common/single_thread.hpp"
#include "../../common/CommonTypes.h"


class Algorithm : public single_thread {
public:

    /// @brief Structure for the input options.
    ///
    /// This structure contains the input options for the walker hardware
    /// algorithm. The parameters that the algorithm accepts are:
    ///
    /// period_ms is the publish period of the localization
    ///
    /// streamer_address is the IP address and port of the WalkerLocalizationPublisher, which periodically
    /// publishes the current state of the nodes of the Walker.
    ///

    struct input_options_t {
        Cit_Types::Time_ms_t period_ms;           ///< The period of the algorithm's activation.
        std::string entrance_publisher,entrance_server;

        input_options_t(){
            entrance_publisher     = "tcp://127.0.0.1:6666";
            entrance_server        = "tcp://127.0.0.1:6666";
            period_ms = 4;
        }
    };

    /// @brief Constructor.
    ///
    /// This is the constructor of the class.
    Algorithm(input_options_t options);

    /// @brief Destructor.
    ///
    /// This is the destructor of the class.
    ~Algorithm();

private:
    std::mutex mutex_data;
    input_options_t input_options;                                     ///< The input options of the algorithm.



    /// @brief Set the period of the publisher.
    ///
    /// This method sets the period of the publisher to adjust the frequency
    /// of publication of the hardware messages.
    ///
    /// @param ms is the desired period.
    void setPublishPeriod(Cit_Types::Time_ms_t ms);

    /// @brief Get the period of the publisher.
    ///
    /// This method gets the period of the publisher to adjust the frequency
    /// of publication of the hardware messages.
    ///
    /// @param ms is the period of the publisher.
    void getPublishPeriod(Cit_Types::Time_ms_t &ms);


    /// @brief Executes the algorithm.
    ///
    /// This method executes the walker hardware algorithm.
    ///
    /// @param terminating is the condition to finish the algorithm.
    void worker(const bool &terminating);

};

//
#endif /* __ENTRYPHONE_ENTRY_ALGORITHM_H__ */
