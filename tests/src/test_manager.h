#pragma once

#include <functional>
#include <cstdint>
#include <string>
#include <vector>


using PFN_test = std::function<uint8_t()>;


struct test_entry {
    PFN_test func;
    std::string description;
};


class TestManager {
    public:
        void register_test(PFN_test&& func, const std::string& desc);
        void run_tests();
    private:
        std::vector<test_entry> m_tests;
};
