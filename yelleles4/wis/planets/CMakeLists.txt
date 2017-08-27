# Set project name and CMake version
cmake_minimum_required (VERSION 3.2)
project (planets)

set (BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set (SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set (ART_DIR "${CMAKE_SOURCE_DIR}/art")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_package (SDL2 REQUIRED)
include_directories (SDL2_INCLUDE_DIR)

find_package (SDL2_IMAGE REQUIRED)
include_directories (SDL2_IMAGE_INCLUDE_DIRS)

include_directories (${SRC_DIR})

set (SOURCES "${SRC_DIR}/main.c" "${SRC_DIR}/sdlFunc.c")

add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${BIN_DIR})
install (DIRECTORY ${ART_DIR}/ DESTINATION ${BIN_DIR}/art PATTERN "${ART_DIR}/*")
