#pragma once

#include "defines.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <format>


namespace viper {
struct VFile;

/* Severity of compiler error */
enum error_level {
    ERR_LEVEL_WARNING, // Recoverable or unreccomended
    ERR_LEVEL_FATAL,   // Completely invalid, unrecoverable
};

/* Error class for something we ran into when compiling */
class VError {
    public:
        ~VError() {}
        template <typename...Args>
        static VError create_new(error_level level, const std::format_string<Args...> fmt, Args&&... args) {
            VError verr = VError();

            verr.m_msg = std::vformat(fmt.get(), std::make_format_args(args...));
            verr.m_level = level;

            return verr;
        }
        void print();
    private:
        VError() {}

        std::string m_msg;
        error_level m_level;
};

/* Module of source code
 *
 * Potentially contains many files
 */
class VModule {
    public:
        ~VModule() {}
    private:
        VModule() {};
        VModule(const VModule&) = delete;
        VModule(VModule&&) = default;

        VModule create_new();
        void add_file(VFile* file);

        std::vector<VFile*> m_files;
};


// Source codes files
struct VFile {
    VFile() {}
    ~VFile() {}
    std::string name;
    i32 file_number;
    std::string content;

    // From chibic project. Maybe use?
    std::string display_name;
    i32 line_delta;

    std::unordered_map<std::string, VModule*> dependency_modules;

    static VFile from(const std::string& file_path);
    static VFile* create_new_ptr();

    std::optional<VError> add_dependency_module(const std::string& name, VModule* mod);
};

}
