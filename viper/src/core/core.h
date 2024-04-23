#pragma once

#include "defines.h"
#include "vresult.h"
#include "verror.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include <format>


namespace viper {
struct VFile;
struct AST;
struct Scope;
class Parser;

/* Module of source code
 *
 * Potentially contains many files
 */
class VModule {
    public:
        ~VModule() {}
    private:
        VModule() {};
        VModule(struct VFile* file) {
            m_files.push_back(file);
        };
        VModule(const VModule&) = delete;
        VModule(VModule&&) = default;

        VModule create_new();
        void add_file(VFile* file);

        std::vector<VFile*> m_files;
        Scope* scope;

        void parse();
};


// Source codes files
struct VFile {
    VFile() {}
    ~VFile() {}
    std::string name;
    i32 file_number;
    std::string content;
    Scope* scope;

    // From chibic project. Maybe use?
    std::string display_name;
    i32 line_delta;

    std::unordered_map<std::string, VModule*> dependency_modules;

    static VFile from(const std::string& file_path);
    static VFile* create_new_ptr();

    VResult<std::string, VError> add_dependency_module(const std::string& name, VModule* mod);

    std::shared_ptr<AST> ast; // Root node of the file
    void parse();
    void print_ast();
};

}
