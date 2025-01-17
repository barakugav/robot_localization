# Add source files
set(FDML_CLI_SOURCE_FILES ${FDML_CLI_SOURCE_FILES} fdml_cli.cpp)

###############################################################################

add_executable(fdml_cli ${FDML_CLI_SOURCE_FILES})

###############################################################################

# Find packages

if (FDML_USE_STATIC_LIBS)
  set_target_properties(fdml_cli PROPERTIES LINK_SEARCH_START_STATIC 1)
endif()

################################################################################
######## Add Packages
# Find required Boost components
find_package(Boost ${FDML_BOOST_MIN_VERSION} REQUIRED COMPONENTS
  system program_options json)

if (FDML_USE_STATIC_LIBS)
  set_target_properties(fdml_cli PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()

################################################################################

# Add definitions

if (BUILD_SHARED_LIBS)
  add_definitions(-DFDML_ALL_DYN_LINK)
endif()

# if (NOT WIN32)
#   add_definitions(-DGL_GLEXT_PROTOTYPES)
# endif (NOT WIN32)

# Add include dirs

# Add some compiler options
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(-W3)
  add_compile_options(-WX)
else ()
  add_compile_options(-Wall)
  add_compile_options(-Wextra)
  add_compile_options(-Wpedantic)
  add_compile_options(-Werror)
endif()

include_directories(../../fdml/include)
include_directories(${CMAKE_BINARY_DIR}/fdml/include)
include_directories(${Boost_INCLUDE_DIR})

# Link
target_link_directories(fdml_cli PRIVATE ${Boost_LIBRARY_DIR})
if (FDML_USE_STATIC_LIBS)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()
target_link_libraries(fdml_cli PRIVATE
  fdml
  ${Boost_LIBRARIES})

if (NOT FDML_USE_STATIC_LIBS)
  set_property(TARGET fdml_cli PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_SKIP_BUILD_RPATH TRUE)
endif()

set_target_properties(fdml_cli PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)
set_target_properties(fdml_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)
set_target_properties(fdml_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/$<0:>)
set_target_properties(fdml_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/$<0:>)

install(TARGETS fdml_cli
  EXPORT FDMLTargets
  RUNTIME DESTINATION ${FDML_INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${FDML_INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${FDML_INSTALL_LIB_DIR})
