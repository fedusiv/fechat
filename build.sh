rm -rf build/
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_C_COMPILER=/usr/bin/gcc ..
make
cd -
cp build/fechat .