#include "ApartmentClient.hpp"

//#define DEBUG
#include "../../../common/debug.hpp"


ApartmentClient::ApartmentClient(const std::string address) : Requester(address) {}


bool ApartmentClient::Start() {
  return Requester::start();
}

bool ApartmentClient::ringBell() {
  nlohmann::json reply;
  nlohmann::json request;
  request[cmd_field] = cmd_bimap[cmd_t::RING_BELL];
  std::cout << "ring_bell request: " << request.dump(2) << std::endl;
  this->request(request, reply);

  try{
    const std::string response = reply.at(response_field).get<std::string>();
    return response_bimap[response] == response_t::OK;
  }
  catch(const std::exception& e){
    std::cerr << "ApartmentClient::ringBell : exception during response parse: " << e.what() << std::endl;
    return false;
  }
}

bool ApartmentClient::request(const nlohmann::json& request, nlohmann::json& response) {
  std::string received;
  status_t status;
  Requester::request(request.dump(), received, status);

  if(status != status_t::STATUS_OK) return false;

  try{
    response = nlohmann::json::parse(received);
  }
  catch(const std::exception& e){
    std::cerr << "ApartmentClient::request : exception during parse: " << e.what() << std::endl;
    return false;
  }
  return true;
}

ApartmentClient::~ApartmentClient() {
  Requester::stop();
}
