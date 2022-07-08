set(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
#set(CMAKE_CXX_CLANG_TIDY /usr/local/opt/llvm/bin/clang-tidy)
set(CMAKE_CXX_FLAGS -I/usr/local/opt/llvm/include)
set(CMAKE_PREFIX_PATH /usr/local/opt/llvm)

#link_directories(/usr/local/opt/llvm/lib)

set(OPENSSL_ROOT_DIR /usr/local/opt/openssl)
