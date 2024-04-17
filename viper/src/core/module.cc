#include "core.h"
#include <memory>

namespace viper {

/// @brief Create a new module from some files
///
VModule VModule::create_new() {
    VModule vmod = VModule();

    return vmod;
}

void VModule::add_file(VFile* file) {
    m_files.push_back(file);
}

}
