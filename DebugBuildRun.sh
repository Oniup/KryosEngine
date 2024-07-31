cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR=Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 . -B Build
ninja -C Build
./Binaries/KryosEditor
