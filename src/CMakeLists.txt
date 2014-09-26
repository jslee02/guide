# Enable multi-threaded compilation.
# We do this here and not in the root folder since the example apps
# do not have enough source files to benefit from this.
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Search all header and source files
file(GLOB_RECURSE guide_srcs  "*.cpp")
file(GLOB_RECURSE guide_hdrs  "*.h")
file(GLOB_RECURSE guide_forms "*.ui")
file(GLOB_RECURSE guide_res   "*.qrc")
#qt5_wrap_cpp(guide_hdrs_moc ${guide_hdrs})
qt5_wrap_ui(guide_forms_hdrs ${guide_forms})
qt5_add_resources(guide_res_rcc ${guide_res})

#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Setup directory structure for Visual Studio
set(SUBDIRS common widgets)
foreach(SUBDIR ${SUBDIRS})
  file(GLOB_RECURSE dir_files ${SUBDIR}/*.h ${SUBDIR}/*.cpp)
  source_group(${SUBDIR} FILES ${dir_files})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Library
if(MSVC)
  add_executable(guide WIN32 ${guide_srcs} ${guide_hdrs} ${guide_res_rcc})
  set_target_properties(guide PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
else()
  add_executable(guide ${guide_srcs} ${guide_hdrs} ${guide_res_rcc})
endif()

if(UNIX)
  #qt5_use_modules(guide OpenGL Widgets X11Extras)
  qt5_use_modules(guide OpenGL Widgets)
elseif(WIN32)
  qt5_use_modules(guide OpenGL Widgets)
endif()

target_link_libraries(guide
    ${DART_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${Boost_LIBRARIES}
)

set_target_properties(guide PROPERTIES
    OUTPUT_NAME "guide${GUIDE_MAJOR_VERSION}"
    SOVERSION ${GUIDE_MINOR_VERSION}
    VERSION "${GUIDE_MINOR_VERSION}.${GUIDE_PATCH_VERSION}.0"
)

if(MSVC)
  set_target_properties(${target} PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE "/LTCG")
endif()

set(OGRE_RESOURCE_FOLDER ${CMAKE_SOURCE_DIR}/src/resources)

if(UNIX)
  include_directories(/usr/include/OGRE)
  set(OGRE_PLUGIN_FOLDER "${OGRE_LIBRARY_DIR_DBG}/OGRE-1.9.0")
elseif(WIN32)
  include_directories(
      "${OGRE_HOME}\\include"
      "${OGRE_HOME}\\include\\OGRE"
      "${OGRE_HOME}\\include\\OGRE\\RenderSystems\\GL"
      "${OGRE_HOME}\\boost"
  )
  link_directories(
      ${LINK_DIRECORIES}
      "${OGRE_HOME}\\lib\\Release"
      "${OGRE_HOME}\\boost\\lib"
  )
  set(OGRE_PLUGIN_FOLDER "${OGRE_HOME}\\bin\\Release")
endif()

configure_file(
  "${CMAKE_SOURCE_DIR}/src/plugins.cfg.in"
  "${CMAKE_BINARY_DIR}/src/plugins.cfg"
  )

configure_file(
  "${CMAKE_SOURCE_DIR}/src/resources.cfg.in"
  "${CMAKE_BINARY_DIR}/src/resources.cfg"
)

install(DIRECTORY ./ DESTINATION include/guide FILES_MATCHING PATTERN "*.h")
install(TARGETS guide EXPORT GUIDETargets DESTINATION lib)

