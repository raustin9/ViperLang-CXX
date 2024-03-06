#include <iostream>
#include <optional>
#include <functional>

#include "mantis/controller.h"
#include "mantis/http.h"
#include "mantis/response.h"

// Test controller to test routing
class TestController : public mantis::HTTPController<TestController> {
public:
    HTTP_METHOD_LIST_BEGIN(users)
    HTTP_METHOD_ADD(mantis::method::GET, TestController::test_method, "/test");
    HTTP_METHOD_ADD(mantis::method::GET, TestController::test_method, "/test/{name}");

    HTTP_METHOD_LIST_END
    
    void test_print() override {
        std::cout << "TEST CONTROLLER PRINT\n";
    }
private:
        mantis::HTTPResponse test_method(mantis::HTTPRequest* req) {
            std::cout << "Test Method Reached\n";
            return mantis::HTTPResponse::create_new();
        }
};

int main() {
    mantis::HTTPServer server = 
        mantis::HTTPServer::create_new()
        .listen_on("0.0.0.0", 8080)
        .set_num_workers(4)
        .register_route_controller<TestController>()
        .run();

    return 0; 
}
