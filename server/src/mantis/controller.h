#pragma once
#include "defines.h"
#include "http_defines.h"
#include "request.h"
#include "response.h"
#include "mantis/route.h"
#include "mantis/utils.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>


/// Macros for adding routes to the controller
#define HTTP_METHOD_LIST_BEGIN(root_route)\
    static constexpr char base[100] = #root_route;\
    static void init_path_routing() {\
        std::string base = #root_route;


#define HTTP_METHOD_ADD(verb, method, pattern)    \
        register_method(verb, &method, base + pattern)

#define HTTP_METHOD_LIST_END        \
        return;                     \
    }

namespace mantis {

class HTTPControllerBase {
    public: 
        HTTPControllerBase(){}
        virtual void test_print() {
            std::cout << "base controller\n";
        }
};

/// @brief HTTP Controller base clase
/// @tparam T the type of the derived class
template <typename T>
class HTTPController : public HTTPControllerBase {
public:
    // below fails for some reason
    // static_assert(std::derived_from<T, HTTPController<T>> == true);

    template <typename FUNC>
    static void register_method(method::request_method_e method, FUNC&& function, const std::string& pattern);

    static std::shared_ptr<T> create_new() {
        std::shared_ptr<T> controller = std::make_shared<T>();
        T::init_path_routing(); 
        return controller;
    }

protected:
private:
};

template <typename T>
template <typename FUNC>
void HTTPController<T>::register_method(
    method::request_method_e method,
    FUNC&& function,
    const std::string &pattern
    ) {
//    std::vector<std::string> split_route = split_str(pattern, "/");
//   
//    std::cout << "Full route: " << pattern << "\n";
//    for (const auto& s : split_route) {
//        std::cout << " " << s;
//    }
//    std::cout << "\n";
}

} // http namespace
