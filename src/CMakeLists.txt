project(guide)

# Find DART
find_package(DART REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")

# Search all header and source files
file(GLOB_RECURSE guide_srcs *.cpp)
file(GLOB_RECURSE guide_hdrs *.h)

# Setup directory structure for Visual Studio
set(SUBDIRS common)
foreach(SUBDIR ${SUBDIRS})
        file(GLOB_RECURSE dir_files ${SUBDIR}/*.h ${SUBDIR}/*.cpp)
        source_group(${SUBDIR} FILES ${dir_files})
endforeach()

# Library
#add_library(guide ${guide_srcs} ${guide_hdrs})
add_executable(guide ${guide_srcs} ${guide_hdrs})

target_link_libraries(guide ${DART_LIBRARIES})
set_target_properties(guide PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
if(MSVC)
    set_target_properties(${target} PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE "/LTCG")
endif()
link_directories(${DART_LIBRARY_DIRS})

install(DIRECTORY ./ DESTINATION include/guide
        FILES_MATCHING PATTERN "*.h*"
        PATTERN "*CMake*" EXCLUDE)

install(TARGETS guide DESTINATION lib)
