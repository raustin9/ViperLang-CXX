#include "response.h"

namespace mantis {

/// @brief Create an empty HTTPResponse with default values 
HTTPResponse HTTPResponse::create_new() {
    HTTPResponse res = HTTPResponse();
    res.m_data_size = 0;
    return res;
}

// @brief Create a string out of the http response to be sent back to the server
std::string HTTPResponse::encode() {
    std::string encoded = "HTTP/1.0 " + m_status + "\r\n";
    encoded += "Server: test-server\r\n";
    encoded += "Connection: close\r\n";

    // Encode the headers
    for (const auto& [field, value] : m_headers) {
        encoded += field + ": " + value + "\r\n";
    }

    encoded += "\r\n";
    encoded += m_body->data();
     
    return encoded;
}

}
