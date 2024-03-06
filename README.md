# Cpp-TemplateProject
A shell for beginning new C++ projects with support for cross-platform support for compilation on both Windows and Linux. (Mac potentially as well)

## Notes
If you want to do things like dynamic linking, there will be a little more work. This is something I am adding in a new branch soon.

The default version of C++ being used is C++20. To change this, you need to chance the CXXSPEC variable in both `Makefile`s and I would also reccomending changing it in `compile_flags.txt` if you are using the clangd LSP.

The `assembly` directory is where project source code goes. Change this to the name you want this folder to be. Follow similar steps to changing the C++ version above to do this.
