#pragma once
#include "defines.h"
#include "http_defines.h"

#include <unordered_map>
#include <vector>
#include <memory>

namespace mantis {

class HTTPRequest {
public:
    // Defines the result of parsing the HTTP Request
    enum RequestParsingError {
        UnknownMethod,
        InvalidFormat,
        ParseValid,
    };

    // Defines the state of parsing the request. 
    // i.e. which part of the parsing algorithm we are in
    enum ParseStatus {
        ExpectMethod,
        ExpectRequestLine,
        ExpectHeaders,
        ExpectBody,
        ExpectChunkLen,
        ExpectChunkBody,
        ExpectLastEmptyChunk,
        Complete
    };

    bool parse_request_line(const char* begin, const char* end);
    RequestParsingError parse_request(const std::string& request);
    HTTPRequest() {}

    bool set_method(const std::string& method_str);
    // bool set_uri

    static std::unique_ptr<HTTPRequest> create_new(const std::string& request);

    std::unordered_map<std::string, std::string> headers;
    method::request_method_e m_method;
    std::string str;
    std::string uri;
    std::string version;

private:
};

}
