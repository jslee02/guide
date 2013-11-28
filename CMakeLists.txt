cmake_minimum_required(VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_WARNING_LEVEL 4)
# Disables a warning about a change in Cygwin Cmake

project(guide)
message(STATUS "Generate makefile/project file for GUIDE")

set(GUIDE_MAJOR_VERSION "0")
set(GUIDE_MINOR_VERSION "1")
set(GUIDE_PATCH_VERSION "0")
set(PKG_DESC "Graphical User Interface for DART Engine.")
set(GUIDE_VERSION "${GUIDE_MAJOR_VERSION}.${GUIDE_MINOR_VERSION}.${GUIDE_PATCH_VERSION}")
set(PKG_EXTERNAL_DEPS "dart")

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Profile."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# GCCFilter, if appliciable
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCPP)
   option(COLOR_GCC "Use GCCFilter to color compiler output messages" OFF)
   set(COLOR_GCC_OPTIONS "-c -r -w" CACHE STRING "Arguments that are passed to gccfilter when output coloring is switchend on. Defaults to -c -r -w.")
   if(COLOR_GCC)
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "gccfilter ${COLOR_GCC_OPTIONS}")
   endif()
endif()

if(MSVC)
  message(STATUS "Setup Visual Studio Specific Flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Gy /W1 /EHsc")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi /GL /Gy /W1 /EHsc /arch:SSE2 /openmp")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG")
elseif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-msse2 -fPIC")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -fno-omit-frame-pointer -fno-inline-functions -fno-inline-functions-called-once -fno-optimize-sibling-calls")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_DEBUG} -pg")
else()
  message(SEND_ERROR "Compiler not supported.")
endif()

set(CMAKE_DEBUG_POSTFIX "d")

# System Install paths
if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "C:/Guide" CACHE PATH "Install prefix" FORCE)
endif()

###############
# Find dependencies
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Find DART
find_package(DART REQUIRED CONFIG)

# Find OpenGL
find_package(OpenGL)
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARY}")
link_libraries(guide ${OPENGL_LIBRARIES})

# Boost
find_package(Boost REQUIRED system filesystem)
if(MSVC OR MSVC90 OR MSVC10)
    add_definitions(-DBOOST_ALL_NO_LIB)
endif()
add_definitions(-DBOOST_TEST_DYN_LINK)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Fine Qt4 (http://qt-project.org/quarterly/view/using_cmake_to_build_qt_projects)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXMLL TRUE)
set(QT_USE_QTTEST TRUE)

# Find OSG
find_package(OpenThreads REQUIRED)
find_package(osg REQUIRED)
find_package(osgQt REQUIRED)
find_package(osgViewer REQUIRED)
find_package(osgGA REQUIRED)
find_package(osgDB REQUIRED)
find_package(osgUtil REQUIRED)

include_directories(${DART_INCLUDE_DIRS})
include_directories(${GUIDE_INCLUDE_DIRS})
include_directories(${OPENTHREADS_INCLUDE_DIR})
include_directories(${OSG_INCLUDE_DIR})
include_directories(${OSGQT_INCLUDE_DIR})
include_directories(${OSGVIEWER_INCLUDE_DIR})
include_directories(${OSGGA_INCLUDE_DIR})
include_directories(${OSGDB_INCLUDE_DIR})
include_directories(${OSGUTIL_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR})
link_directories(
    "${CMAKE_BINARY_DIR}/lib"
    "${CMAKE_INSTALL_PREFIX}/lib"
    ${Boost_LIBRARY_DIRS}
    ${DART_LIBRARY_DIRS}
    ${GUIDE_LIBRARY_DIRS}
    ${OPENTHREADS_LIBRARY_DIRS}
    ${OSG_LIBRARY_DIRS}
    ${OSGQT_LIBRARY_DIRS}
    ${OSGVIEWER_LIBRARY_DIRS}
    ${OSGGA_LIBRARY_DIRS}
    ${OSGDB_LIBRARY_DIRS}
    ${OSGUTIL_LIBRARY_DIRS})

set(GUIDE_DEPENDENCIES
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

###############
# Generate the GUIDE CMake Config file
configure_file(cmake/GUIDEConfig.cmake.in "${CMAKE_BINARY_DIR}/cmake/GUIDEConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/cmake/GUIDEConfig.cmake DESTINATION share/guide)

set(PC_CONFIG_IN ${CMAKE_SOURCE_DIR}/cmake/guide.pc.in)
set(PC_CONFIG_OUT ${CMAKE_BINARY_DIR}/cmake/guide.pc)
configure_file(${PC_CONFIG_IN} ${PC_CONFIG_OUT} @only)
install(FILES ${PC_CONFIG_OUT} DESTINATION lib/pkgconfig)

###############
# Build source
add_subdirectory(src)
enable_testing()
add_subdirectory(unittests)

install(EXPORT GUIDETargets DESTINATION share/guide)

###############
# Add an "uninstall" target
configure_file ("${PROJECT_SOURCE_DIR}/cmake/uninstall_target.cmake.in" "${PROJECT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
add_custom_target (uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/uninstall_target.cmake")

###############
# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_INPUT "Doxyfile")
  set(DOXYGEN_OUTPUT "doc")

  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${DOXYGEN_INPUT}
    )

  add_custom_target(docs DEPENDS ${DOXYGEN_OUTPUT})
  # add_custom_target(apidoc ALL DEPENDS ${DOXYGEN_OUTPUT})

  add_custom_target(docs_forced
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

###############
# Package Installer
set(CPACK_PACKAGE_NAME "guide")
set(CPACK_SYSTEM_NAME "i386")
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(CPACK_SYSTEM_NAME "amd64")
endif()
set(CPACK_PACKAGE_VERSION_MAJOR ${GUIDE_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${GUIDE_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${GUIDE_PATCH_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "dart (>=2.6.0) qt4 openscenegraph")

set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CPACK_SYSTEM_NAME}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jeongseok Lee and Karen Liu (www.cc.gatech.edu/~karenliu/)")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PKG_DESC}")
set(CPACK_PACKAGE_VENDOR "Computer Graphics Lab at GT")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)
