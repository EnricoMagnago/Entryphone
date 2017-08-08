

#include "EntranceServerDefinitions.hpp"

// JSON fields
const std::string EntranceServerDefinitions::response_field = "response";
const std::string EntranceServerDefinitions::cmd_field      = "cmd";




// BIMAP from cmd to string and viceversa
const bimap<EntranceServerDefinitions::cmd_t, std::string> EntranceServerDefinitions::cmd_bimap{
        {cmd_t::OPEN_DOOR,             "apri_porta"}
};


// BIMAP from respons_t to string and viceversa
const bimap<EntranceServerDefinitions::response_t, std::string> EntranceServerDefinitions::response_bimap {
        {response_t::OK, "ok"},
        {response_t::FAIL, "fail"},
        {response_t::TIMEOUT, "timeout"}
};
