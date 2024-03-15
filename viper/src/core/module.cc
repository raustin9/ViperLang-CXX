#include "module.h"
#include <memory>

namespace viper {

/// @brief Create a new module from some files
///
VModule VModule::create_new() {
    VModule vmod = VModule();

    return vmod;
}

void VModule::add_file(std::unique_ptr<VFile> file) {
    m_files.push_back(std::move(file));
}

}
