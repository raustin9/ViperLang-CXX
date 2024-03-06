#pragma once
#include "defines.h"
#include "http_defines.h"

#include <typeinfo>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <memory>
#include <string_view>
#include <string>
#include <cstdio>
#include <unordered_map>
#include <vector>

namespace mantis {

/// TODO: Move this to its own file
class HTTPMessageBody {
public:
    virtual ~HTTPMessageBody() {}

    // NOTE: The only type being used currently is the String type
    enum class BodyType {
        None = 0,
        String,
        StringView
    };

    virtual const char* data() const {
        return nullptr;
    }

    virtual char* data() {
        return nullptr;
    }

    BodyType type() const {
        return m_body_type;
    }

    virtual std::size_t length() {
        return 0;
    }

    virtual std::string_view string_view() const = 0;
protected:
    BodyType m_body_type{BodyType::None};
};

class HTTPMessageStringBody : public HTTPMessageBody {
public:
    HTTPMessageStringBody() {
        m_body_type = BodyType::String;
    }

    HTTPMessageStringBody(std::string &body) : m_body{body} {
        m_body_type = BodyType::String;
    }
    
    HTTPMessageStringBody(std::string &&body) : m_body{std::move(body)} {
        m_body_type = BodyType::String;
    }

    const char* data() const override {
        return const_cast<char*>(m_body.data());
    }

    char* data() override {
        return m_body.data();
    }

    std::size_t length() override {
        return m_body.length();
    }

    std::string_view string_view() const  override{
        return std::string_view(m_body.data(), m_body.length());
    }

    void append(const char* buf, std::size_t len) {
        m_body.append(buf, len);
    }

private:
    std::string m_body;
};


class HTTPResponse {
public:
    static HTTPResponse create_new();

    void add_header(std::string field, std::string &&value) {
        std::transform(
            field.begin(),
            field.end(),
            field.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        m_headers[std::move(field)] = std::move(value);
    }

    void add_header(std::string field, std::string &value) {
        std::transform(
            field.begin(),
            field.end(),
            field.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );

        m_headers[std::move(field)] = value;
    }
    
    // Set the status message of the response
    void set_status(status::response_status_code_e code) {
        m_status = status::make_status_str(code);
        // m_status = STATUS_CODE_TO_STR(code);
        printf("STATUS: %s\n", m_status.c_str());
    }

    // Set the body data of the http response
    void set_body(std::string& body) {
        m_body = std::make_shared<HTTPMessageStringBody>(body);
        add_header("content-length", std::to_string(m_body->length()));
    }

    void set_body(std::string&& body) {
        m_body = std::make_shared<HTTPMessageStringBody>(std::move(body));
        add_header("content-length", std::to_string(m_body->length()));
    }

    // Encode the response fields into bytes to be sent back to client
    std::string encode(); 

private:
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_status;
   
    std::shared_ptr<HTTPMessageBody> m_body;
    std::size_t m_data_size;
};

}
