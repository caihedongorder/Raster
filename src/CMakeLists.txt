#set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4305")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4018")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4312")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")

include_directories(
    ./
    ../Dependencies
    ../Dependencies/mathfu
    ../Dependencies/freeimage
    ../Dependencies/freeglut/include
    ../Dependencies/glew/include
    )
link_directories(
    ../Dependencies/freeimage
    ../Dependencies/freeglut/lib/x64
    ../Dependencies/glew/lib/x64
    )

add_executable(Raster 
    main.cpp
    scene.cpp
    utils.cpp
    )
target_link_libraries(Raster 
        freeimage
        freeglut
        glew32
        opengl32
        )
