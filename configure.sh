export LDFLAGS="-L/usr/local/opt/llvm/lib"
export CPPFLAGS="-I/usr/local/opt/llvm/include"
export PATH="/usr/local/opt/llvm/bin:$PATH"

cmake --no-warn-unused-cli \
    -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER=$(which clang) \
    -DCMAKE_CXX_COMPILER=$(which clang++) \
    -DCMAKE_CXX_CLANG_TIDY=clang-tidy \
    -B $(pwd)/build \
    -G "Unix Makefiles"

cmake --build $(pwd)/build --config Debug --target all -j 10 --