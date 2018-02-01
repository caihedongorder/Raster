cmake_minimum_required(VERSION 3.10)

project(Raster)
message(STATUS "This is Binary dir:" ${RASTER_BINARY_DIR})
message(STATUS "This is SOURCE dir:" ${RASTER_SOURCE_DIR})
add_subdirectory(src bin)
