#pragma once
#include "defines.h"
#include "http_defines.h"
#include "controller.h"

#include <unordered_map>
#include <memory>

namespace mantis {

class HTTPRoute {
public:
private:
    std::unordered_map<std::string, std::shared_ptr<HTTPRoute>> m_subroutes;

    std::function<std::unique_ptr<HTTPResponse>(HTTPRequest& req)> m_action;
};

}
