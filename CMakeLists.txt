cmake_minimum_required (VERSION 2.6)
project (libEdimaxcpp)
# The version number.
set (LibEximax_VERSION_MAJOR 1)
set (LibEdimax_VERSION_MINOR 0)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

ADD_SUBDIRECTORY(tinyXML2)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
     "${PROJECT_SOURCE_DIR}/LibEdimaxConfig.h.in"
     "${PROJECT_BINARY_DIR}/LibEdimaxConfig.h"
)

find_package(CURL REQUIRED)


SET(CMAKE_RPATH 1)
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
       SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
   ENDIF("${isSystemDir}" STREQUAL "-1")

SET(sourcefiles
    libEdimax.h
    libEdimax.cpp
    )

ADD_LIBRARY(libEdimax SHARED ${sourcefiles})
TARGET_INCLUDE_DIRECTORIES(libEdimax PUBLIC ${CURL_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(libEdimax ${CURL_LIBRARIES})
TARGET_LINK_LIBRARIES(libEdimax tinyxml2)

ADD_EXECUTABLE(libEdimaxTest "test.cpp")
TARGET_LINK_LIBRARIES(libEdimaxTest libEdimax)