# Escarabajo Ranchero build script

cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

#
# Base bits
#

set(RELEASE_NAME "EscarabajoRanchero")
set(VERSION "0.0.1+trunk")
set(VERSION_INFO "Release")

# For packaging.
set(ARCHIVE_BASE_NAME ${CMAKE_PROJECT_NAME}-${VERSION})
set(ARCHIVE_FULL_NAME ${ARCHIVE_BASE_NAME}.tar.xz)

option(DEBUG "Build for debugging." OFF)

if (DEBUG)
    message(STATUS "Debug build")
endif ()

find_package(PkgConfig)

# Dist
# This generates the dist tarballs
add_custom_target(
    dist
    COMMAND 
        git archive --prefix=${ARCHIVE_BASE_NAME}/ HEAD
        | xz -z > ${CMAKE_BINARY_DIR}/${ARCHIVE_FULL_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)


