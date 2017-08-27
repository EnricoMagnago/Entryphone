/*!****************************************************************************
 *    \file    main.cpp
 *    \brief   Main program of the aruco localization module.
 *    \version 1.0
 *    \date    2017
 *    \authors Alessio Colombo          <alessio.colombo_1@unitn.it>
 *             Bernardo Villalba Frías  <br.villalbafrias@unitn.it>
 *             Valerio Magnago          <valerio.magnago@unitn.it>
 *****************************************************************************/


#include <iostream>
#include <signal.h>
#include <condition_variable>
#include <thread>
#include <unistd.h>    // for getpid() function
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../../../../common/zmq_layer/Subscriber.hpp"

using namespace std;

static bool interrupted = false;

static condition_variable wait_cond;
static mutex wait_mutex;

static void signal_handler(int /*sig*/);
void callback(const char */*  */, const char *buf, size_t size);


/* frame resolution */
const uint32_t FRAME_HEIGHT_PX  = 480;
const uint32_t FRAME_WIDTH_PX   = 640;
const uint32_t CAMERA_FPS       =  10;

int main() {
  /* Initial message */
  cout << "****************************************" << endl;
  cout << "              VIDEO STREAMER              " << endl;
  cout << " !!if you are a man don't get naked now!!   " << endl;
  cout << "   if you are a girl ...                 "    << endl;
  cout << "****************************************" << endl << endl;


  // Register the signal handler for the SIGINT signal (Ctrl+C)
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  // waiting
  cout << "CTRL-C or \"kill -2 " << getpid() << "\" to quit." << endl;
  cout.flush();

  /* Input option for alghorithm class */
  //std::string publisherIp        = "tcp://127.0.0.1:6669";
  std::string publisherIp        = "tcp://192.168.1.254:6669";
  /* create and show the window */
  cv::namedWindow("Capture", cv::WINDOW_AUTOSIZE);

  Subscriber subscriber;

  subscriber.register_callback(Subscriber::callback_t(
                                                      [](const char *topic, const char *buf, size_t size, void */*data*/){
                                                        callback(topic, buf, size);
                                                      }));

  if(!subscriber.start(publisherIp, "VIDEO")){
    cerr<<"Impossible to start the subscriber"<<endl;
    return -1;
  }

  while (!interrupted) {
    unique_lock<mutex> lock(wait_mutex);
    wait_cond.wait_for(lock, chrono::milliseconds(1000));
  }


  subscriber.stop();

  // stopping
  cout << "Stopping " << endl;
  cout.flush();

  /* Final message */
  cout << endl << "***************************" << endl;
  cout << "   VIDEO STREAMER Finished   " << endl;
  cout << "   see you later alligator  " << endl;
  cout << "*******************************" << endl << endl;
  cout.flush();
  cout.flush();

  return 1;

}

void signal_handler(int /*sig*/) {
  if (!interrupted) {
    cout << "Signal caught, please wait for termination." << endl;
    cout.flush();
  }
  interrupted = true;
  wait_cond.notify_all();
}


void callback(const char */*  */, const char* buf, size_t size) {
  std::cout << "call back" << std::endl;
  std::cout.flush();
  char buf_cpy[FRAME_HEIGHT_PX * FRAME_WIDTH_PX * 3];
  if(FRAME_HEIGHT_PX * FRAME_WIDTH_PX * 3 != size){
    std::cerr << "size of received image and expected size are different"
              << "\n\texpected: " << FRAME_HEIGHT_PX * FRAME_WIDTH_PX * 3
              << "\n\tfound: "  << size << std::endl;
    return;
  }
  memcpy(buf_cpy, buf, size);
  std::cout << "memcpy: " << size
            << " dim: " << FRAME_HEIGHT_PX * FRAME_WIDTH_PX * 3 << std::endl;
  std::cout.flush();

  cv::Mat frame(FRAME_HEIGHT_PX, FRAME_WIDTH_PX, CV_8UC3, buf_cpy, cv::Mat::AUTO_STEP);
  cv::imshow("Capture", frame);
  cv::waitKey(1);
}
