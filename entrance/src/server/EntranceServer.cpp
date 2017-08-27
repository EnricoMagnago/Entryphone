//
// Created by valerio on 28/07/17.
//

#include "EntranceServer.hpp"

#include "../../../common/json/Json.hpp"

//#define DEBUG
#include "../../../common/debug.hpp"


EntranceServer::EntranceServer(const std::string address) : Replier(address) {
  // Define the callback and register it
  callback_t tmp_call = [this](const std::string& received, void* /* */, std::string& tosend) {
    this->processMessage(received, tosend);
  };
  register_callback(tmp_call, nullptr);
  SDEBUG("QrServer::QrServer() initialized");
}


void EntranceServer::processMessage(const std::string& received, std::string& tosend) {
  nlohmann::json j;
  nlohmann::json js_tosend;
  response_t response = response_t::TIMEOUT;
  SDEBUG("QrServer::processMessage recieved message: " << received);
  try{
    j = nlohmann::json::parse(received);

    SDEBUG("QrServer::processMessage parsing JSON done");


    // int id = j.at("id");   // id is not needed

    cmd_t rec_cmd = cmd_bimap[j.at(cmd_field).get<std::string>()];

    SDEBUG("QrServer::QrServer() cmd revieved: " << j.at(cmd_field).get<std::string>());
    switch(rec_cmd){
    case (cmd_t::OPEN_DOOR):{
      std::cout << "Richiesta apertura porta" << std::endl;
      response = response_t::OK;
    }
      break;
    }

  }
  catch(const std::out_of_range& oor){
    response = response_t::FAIL;
    std::cerr << " QrServer::processMessage(). Some needed fields are missing: " << oor.what() << '\n';
  }
  catch(const std::invalid_argument& oor){
    response = response_t::FAIL;
    std::cerr << "QrServer::processMessage invalid input argument: " << '\n';
  }
  catch(const std::exception& e){
    response = response_t::FAIL;
    std::cerr << "caught exception: " << e.what() << std::endl;
  }

  js_tosend[response_field] = response_bimap[response];
  tosend = js_tosend.dump();

  SDEBUG("QrServer::QrServer() response: " << js_tosend.dump());
  return;
}


bool EntranceServer::Start() {
  return Replier::start();
}

EntranceServer::~EntranceServer() {
  Replier::stop();
}
