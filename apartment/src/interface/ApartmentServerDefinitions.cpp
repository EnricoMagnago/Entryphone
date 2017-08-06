#include "ApartmentServerDefinitions.hpp"

// JSON fields
const std::string ApartmentServerDefinitions::response_field = "response";
const std::string ApartmentServerDefinitions::cmd_field      = "cmd";




// BIMAP from cmd to string and viceversa
const bimap<ApartmentServerDefinitions::cmd_t, std::string> ApartmentServerDefinitions::cmd_bimap{
        {cmd_t::OPEN_DOOR,             "apri_porta"}
};


// BIMAP from respons_t to string and viceversa
const bimap<ApartmentServerDefinitions::response_t, std::string> ApartmentServerDefinitions::response_bimap {
        {response_t::OK, "ok"},
        {response_t::FAIL, "fail"},
        {response_t::TIMEOUT, "timeout"}
};
