/*!****************************************************************************
 *    \file    Algorithm.hpp
 *    \brief   Main class for manage the aruco identification and localization module
 *    \version 1.2
 *    \date    2017
 *    \authors Alessio Colombo          <alessio.colombo_1@unitn.it>
 *             Valerio Magnago          <valerio.magnago@unitn.it>
 *****************************************************************************/


#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <fstream>
#include <string>
#include <stdint.h>

#include <memory>
#include "../../common/single_thread.hpp"
#include "../../common/TimeUtils.hpp"

namespace VideoStreamer {

  class Algorithm : public single_thread {
  public:
    typedef uint64_t sampling_time_t;
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

    /**
     * @brief Structure for store the input option for the class Alghorithm
     *
     * period_ms is the publisher period of the localization
     *
     * publisher of the aruco information
     *
     * server of the aruco information
     *
     */
    struct input_options_t {

      std::string publisher;       ///< Address of the aruco publisher
      std::string server;          ///< Address of the aruco server

      sampling_time_t period_ms;  ///< The period of the algorithm's activation.

      // Default initializer
      input_options_t() {
        publisher = "127.0.0.1:5003";
        server = "127.0.0.1:5004";
        period_ms = 40;
      }

      // Initializer
      input_options_t(std::string qr_server_, std::string qr_publisher_, uint16_t period_ms_) {
        publisher = qr_publisher_;
        server = qr_server_;
        period_ms = period_ms_;
      }
    };


    /**
     * @brief Constructor of the Algorithm class
     */
    Algorithm(std::unique_ptr<input_options_t> &alg_param_);

    /**
     * @brief Destructor of the Algorithm class
     */
    ~Algorithm();

  private:
    std::unique_ptr<input_options_t> alg_param;                                     ///< The input options of the algorithm.
    TimeUtils::PeriodicTimer<uint64_t, TimeUtils::Granularity::MILLI> timer;


    /**
     * @brief Set the period of the framee analyzation.
     * @param ms  is the desired period in millisec
     *
     */
    void setPublishPeriod(sampling_time_t ms);

    /**
     * @brief Get the period of the frame analyzation
     *
     * @param ms is the period of the frame analyzation
     */
    void getPublishPeriod(sampling_time_t &ms);


    /**
     * @brief core of the class, thread that runs
     * @param terminating  if setted to true the thread stops
     */
    void worker(const bool &terminating);


  };
}
#endif /* ALGORITHM_H_ */
