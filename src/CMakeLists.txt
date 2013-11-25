project(guide)

# Find DART
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_INSTALL_PREFIX}/share/dart")
find_package(DART REQUIRED CONFIG)

# Find OpenGL
find_package(OpenGL)
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARY}")
link_libraries(guide ${OPENGL_LIBRARIES})

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

# Search all header and source files
file(GLOB_RECURSE guide_srcs "*.cpp")
file(GLOB_RECURSE guide_hdrs "*.h")
file(GLOB_RECURSE guide_forms "*.ui")
file(GLOB_RECURSE guide_res "*.qrc")
qt4_wrap_cpp(guide_hdrs_moc ${guide_hdrs})
qt4_wrap_ui(guide_forms_hdrs ${guide_forms})
qt4_add_resources(guide_res_rcc ${guide_res})

# Setup directory structure for Visual Studio
set(SUBDIRS common)
foreach(SUBDIR ${SUBDIRS})
    file(GLOB_RECURSE dir_files ${SUBDIR}/*.h ${SUBDIR}/*.cpp)
    source_group(${SUBDIR} FILES ${dir_files})
endforeach()

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

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
link_directories(${CMAKE_BINARY_DIR}/lib
    ${DART_LIBRARY_DIRS}
    ${GUIDE_LIBRARY_DIRS}
    ${OPENTHREADS_LIBRARY_DIRS}
    ${OSG_LIBRARY_DIRS}
    ${OSGQT_LIBRARY_DIRS}
    ${OSGVIEWER_LIBRARY_DIRS}
    ${OSGGA_LIBRARY_DIRS}
    ${OSGDB_LIBRARY_DIRS}
    ${OSGUTIL_LIBRARY_DIRS})

# Library
if(MSVC)
    add_executable(guide WIN32 ${guide_srcs} ${guide_hdrs_moc} ${guide_forms_hdrs} ${guide_res_rcc})
    set_target_properties(guide PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
else()
    add_executable(guide ${guide_srcs} ${guide_hdrs_moc} ${guide_forms_hdrs} ${guide_res_rcc})
endif()

target_link_libraries(guide ${DART_LIBRARIES} ${QT_LIBRARIES}
    ${OPENTHREADS_LIBRARIES} ${OSG_LIBRARIES} ${OSGQT_LIBRARIES}
    ${OSGVIEWER_LIBRARIES} ${OSGGA_LIBRARIES} ${OSGDB_LIBRARIES}
    ${OSGUTIL_LIBRARIES})

install(TARGETS guide DESTINATION bin)
