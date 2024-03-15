#pragma once

/*
 * This file contains the interface for a Viper Module
 * 
 * A module is a single compilation unit for Viper. This can be split
 * across multiple files
 *
 */

#include "defines.h"
#include "file.h"

#include <vector>
#include <memory>

namespace viper {

class VModule {
    public:
        ~VModule() {}
    private:
        VModule() {};
        VModule(const VModule&) = delete;
        VModule(VModule&&) = default;

        VModule create_new();
        void add_file(std::unique_ptr<VFile> file);

        std::vector<std::unique_ptr<VFile> > m_files;
};

}
