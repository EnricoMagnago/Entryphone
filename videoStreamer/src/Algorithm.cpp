/*
 * Algorithm.cpp
 */

#include "Algorithm.hpp"
#include "../../common/TimeUtils.hpp"

/// Check if M_PI is defined
#ifndef M_PI
#define M_PI           3.14159265358979323846  /* pi */
#endif

//#define DEBUG
#include <unistd.h>
#include <raspicam/raspicam.h>

#include "../../common/zmq_layer/Publisher.hpp"

const uint32_t FRAME_HEIGHT_PX  = 480;
const uint32_t FRAME_WIDTH_PX   = 640;
const uint32_t CAMERA_FPS       =  10;

using namespace std;
using namespace TimeUtils;

VideoStreamer::Algorithm::Algorithm(unique_ptr<Algorithm::input_options_t> &alg_param_):
  single_thread(thread_fun_t([this](const bool& t){this->worker(t);}), "Algorithm"),
  timer(50) {
  // Move the unique pointer and delete the reference coming from outside
  alg_param = std::move(alg_param_);
  setPublishPeriod(alg_param->period_ms);
}



/// @brief Destructor.
VideoStreamer::Algorithm::~Algorithm() { }

/// @brief Set the period of the publisher.
void VideoStreamer::Algorithm::setPublishPeriod(sampling_time_t ms) {
  // min time of the internal loop of the alghoritm worker in millisec
  cout << "Algorithm::setPublishPeriod " << ms << "[ms]" << endl;

  const sampling_time_t MIN_FREQ_TIME_MS   = 50;

  /* The minimum period is hard-coded to 50m */
  alg_param->period_ms = ms > MIN_FREQ_TIME_MS ? ms : MIN_FREQ_TIME_MS;

  timer.setPeriod(alg_param->period_ms);

}

/// @brief Get the period of the publisher.
void VideoStreamer::Algorithm::getPublishPeriod(sampling_time_t &ms) {
  /* Return the period of the publisherIGN */
  ms = alg_param->period_ms;
  cout << "Algorithm::getPublishPeriod " << ms << "[ms]" << endl;
}

/// @brief Executes the algorithm.
void VideoStreamer::Algorithm::worker(const bool& terminating) {
  std::cout<<"Algorithm::worker() started "<<std::endl;

  try {
    Publisher publisher(alg_param->publisher);

    raspicam::RaspiCam camera;
    // set camera parameters.
    camera.setWidth(FRAME_WIDTH_PX);
    camera.setHeight(FRAME_HEIGHT_PX);
    camera.setFrameRate(CAMERA_FPS);

    if(!camera.open()){
      cout << "Failed to connect to the camera." << endl;
      return;
    }

    cout << "Connected to camera: " << camera.getId()
         << " bufs: " << camera.getImageBufferSize( ) << endl;

    // stabilization ?
    sleep(3);


    const size_t image_size = camera.getImageBufferSize();
    unsigned char* image = new unsigned char[image_size];

    /// Start the timer
    timer.start(0);

    ///< Get the current time
    std::cout<<"Algorithm::worker() starting the loop with a period of "<<alg_param->period_ms<<std::endl;
    while (!terminating) {

      ///< Waits for the next activation
      timer.wait_for_next_activation();

      // grab image from camera.
      camera.grab();
      camera.retrieve(image);

      /*AcantoTime<uint64_t, Granularity::MICRO> t_activation;  ///< Get the current time
      getSystemTime(t_activation);
      uint64_t time_us_start = t_activation.value;*/

      std::cout<<"publisher send"<<std::endl;
      publisher.send("VIDEO",(char*)image, image_size);
    }

    // free resources: camera and image.
    camera.release();
    delete[] image;



  } catch (exception& e) {
    cerr << "Algorithm::worker(), exception generated, exiting." << endl;
    cerr << "\"" << e.what() << "\"" << endl;
    cout.flush();
  }

  cout << "Algorithm::worker(), stopping the QR server." << endl;

}
