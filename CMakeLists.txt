cmake_minimum_required (VERSION 3.10)
project (aeglos)

find_package(OpenCV REQUIRED)
# find_package(X11 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include /usr/local/include)

add_library(mjolnir
    mjolnir/src/mj.cpp
    mjolnir/src/nodo.cpp
    mjolnir/src/input_nodes.cpp
    mjolnir/src/processing_nodes.cpp
    mjolnir/include/screenshot.h
    mjolnir/src/screenshot.cpp
    mjolnir/src/nodos_tibianos.cpp
    mjolnir/include/nodos_tibianos.h
)

target_include_directories (mjolnir PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mjolnir/include
)

target_link_libraries (mjolnir
    ${OpenCV_LIBS}
    # pthread
    # ${X11_LIBRARIES}
)

target_compile_definitions (mjolnir PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)


add_executable (aeglos
    main.cpp
    utils/sistema_archivos.cpp
)

target_include_directories (aeglos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries (aeglos
    ${OpenCV_LIBS}
    mjolnir
)

target_compile_definitions (aeglos PRIVATE
    PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

