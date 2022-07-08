set(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
#set(CMAKE_CXX_CLANG_TIDY /usr/local/opt/llvm/bin/clang-tidy)
set(CMAKE_CXX_FLAGS -I/usr/local/opt/llvm/include)

link_directories(/usr/local/opt/llvm/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
