#include "httpserver.h"
#include "mantis/controller.h"
#include <concepts>

namespace mantis {

/// @brief Register a controller to handle routes
/// @param controller the controller 
/// @return the server itself
template <typename T>
HTTPServer HTTPServer::register_route_controller(
) {
    static_assert(
        std::derived_from<T, HTTPController<T> > == true, 
        "Route type needs to be derived from |http::HTTPController|"
    );
    m_controllers[T::base] = T::create_new(); 

    return *this;
}

}
