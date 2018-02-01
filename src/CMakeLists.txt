#set(CMAKE_EXPORT_COMPILE_COMMANDS on)

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
