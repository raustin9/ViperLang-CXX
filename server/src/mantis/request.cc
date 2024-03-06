#include "request.h"
#include "utils.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>

namespace mantis {

// Create a unique pointer to an HTTPRequest object based on the request string
std::unique_ptr<HTTPRequest> HTTPRequest::create_new(const std::string& request_data) {
    auto req = std::make_unique<HTTPRequest>();
    req->str = request_data;

    if (req->parse_request(request_data) != RequestParsingError::ParseValid) {
        std::cout << "ERROR: Invalid Request\n";
    }

    return req;
}

// Parse the HTTP Request string into its pieces
HTTPRequest::RequestParsingError HTTPRequest::parse_request(const std::string& request) {
    constexpr const char* CLRF = "\r\n";
    ParseStatus status = ParseStatus::ExpectMethod;
    std::size_t pos = 0;
    while (true) {
        switch(status) {
            case ParseStatus::ExpectMethod: {
                std::size_t space_pos = request.find(" ", pos);
                if (space_pos == std::string::npos) {
                    // no string in buffer
                    std::cout << "ERROR: No request method present\n";
                    return RequestParsingError::UnknownMethod;
                } else {
                    std::string methodstr = request.substr(0, space_pos);
                    if (!set_method(methodstr)) {
                        std::cout << "ERROR: Unknown Request Method\n";
                        return RequestParsingError::UnknownMethod;
                    }

                    status = ParseStatus::ExpectRequestLine;
                    pos = space_pos;
                }
            } break;
            
            case ParseStatus::ExpectRequestLine: {
                std::size_t clrf_pos = request.find(CLRF, pos);
                if (clrf_pos == std::string::npos) {
                    // No carriage return here is invalid
                    return RequestParsingError::InvalidFormat;
                }

                if (!parse_request_line(request.c_str()+pos, request.c_str()+clrf_pos)) {
                    // Failed to parse the request line 
                    return RequestParsingError::InvalidFormat;
                }
                status = ParseStatus::ExpectHeaders;
            } break;
            default:
                return RequestParsingError::ParseValid;
        }
    }
}

/// @brief Set the method of the request based on the string input
/// @param method_str The string of the method. Ex: "GET" "POST"...
/// @returns True if valid method. False otherwise
bool HTTPRequest::set_method(const std::string& method_str) { 
    if (method_str == "GET") {
        m_method = method::request_method_e::GET;
    } else if (method_str == "POST") {
        m_method = method::request_method_e::POST;
    } else if (method_str == "PUT") {
        m_method = method::request_method_e::PUT;
    } else if (method_str == "DELETE") {
        m_method = method::request_method_e::DELETE;
    } else if (method_str == "HEAD") {
        m_method = method::request_method_e::HEAD;
    } else if (method_str == "CONNECT") {
        m_method = method::request_method_e::CONNECT;
    } else if (method_str == "OPTIONS") {
        m_method = method::request_method_e::OPTIONS;
    } else if (method_str == "TRACE") {
        m_method = method::request_method_e::TRACE;
    } else if (method_str == "PATCH") {
        m_method = method::request_method_e::PATCH;
    } else {
        return false;
    }

    return true;
}

///  @brief Parse the request line of incoming request
///  @param request The string of the incoming request
///  Request Line Format: Method SP Requets URI SP HTTP-Version CRLF
bool HTTPRequest::parse_request_line(const char* begin, const char* end) {
    bool succeed = false;
    succeed = true;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');

    std::cout << "parse_request_line: |" << strndup(space, end-begin) << "|\n";


    return succeed;
}

}
