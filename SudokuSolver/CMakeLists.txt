cmake_minimum_required(VERSION 3.7)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

project(SudokuSolver)

PROJECT(${PROJECT_NAME} CXX C ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ansi -pedantic")

# Custom includes
set(LIBS_DIR  "../")

set(CUSTOM_HEADERS "")
#list(APPEND CUSTOM_HEADERS "${LIBS_DIR}/ThirdParty/include/")

# Custom sources
set(CUSTOM_SOURCES "")
list(APPEND CUSTOM_SOURCES "../../01_Libs/Utils/include/*.c" "${LIBS_DIR}/Utils/include/*.cpp")

# Includes
include_directories("${LIBS_DIR}ThirdParty/include/")
include_directories("${LIBS_DIR}Utils/include/")

######################################Include everuthing src##################################
file(GLOB_RECURSE Proj_SOURCES ${CUSTOM_SOURCES} "src/*.cpp" "src/*.c" "include/*.c" "include/*.cpp")
file(GLOB_RECURSE Proj_HEADERS ${CUSTOM_HEADERS} "src/*.h" "src/*.hpp" "include/*.h" "include/*.hpp")

set( Proj_INCLUDE_DIRS "include/")
foreach (_headerFile ${Proj_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Proj_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES Proj_INCLUDE_DIRS)
include_directories(${Proj_INCLUDE_DIRS})
link_directories(${Proj_INCLUDE_DIRS})
##############################################################################################

add_executable (${PROJECT_NAME} ${Proj_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Proj_INCLUDE_DIRS})

    target_link_libraries(${PROJECT_NAME} libstdc++)
#target_link_libraries(${PROJECT_NAME} pthread)
#target_link_libraries(${PROJECT_NAME} myhtml)
