@cmake -G "NMake Makefiles"  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
@del Makefile
@del cmake_install.cmake
@del CMakeCache.txt
