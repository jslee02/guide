project(guide)

# Find DART
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
find_package(DART REQUIRED CONFIG)

# Search all header and source files
file(GLOB_RECURSE guide_srcs "*.cpp")
file(GLOB_RECURSE guide_hdrs "*.h")

# Setup directory structure for Visual Studio
set(SUBDIRS common)
foreach(SUBDIR ${SUBDIRS})
        file(GLOB_RECURSE dir_files ${SUBDIR}/*.h ${SUBDIR}/*.cpp)
        source_group(${SUBDIR} FILES ${dir_files})
endforeach()

include_directories(${DARTExt_INCLUDE_DIRS})
include_directories(${DART_INCLUDE_DIRS})
include_directories(${GUIDE_INCLUDE_DIRS})
link_directories(${CMAKE_BINARY_DIR}/lib ${DART_LIBRARY_DIRS} ${GUIDE_LIBRARY_DIRS})

# Library
if(MSVC)
    add_executable(guide WIN32 ${guide_srcs} ${guide_hdrs})
    set_target_properties(grip-run PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
else()
    add_executable(guide ${guide_srcs} ${guide_hdrs})
endif()

target_link_libraries(guide ${DART_LIBRARIES} ${DARTExt_LIBRARIES} ${GRIP_LIBRARIES})

install(TARGETS guide DESTINATION bin)
