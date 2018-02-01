#set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories(
    ./
    ../Dependencies/mathfu
    ../Dependencies/freeimage
    ../Dependencies/freeglut/include
    )
link_directories(
    ../Dependencies/freeimage
    ../Dependencies/freeglut/lib
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
