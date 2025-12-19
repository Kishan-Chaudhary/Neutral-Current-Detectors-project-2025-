#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(SNOLABNCD)

#Setup UI and Vis
find_package(Geant4 REQUIRED ui_all vis_all)

#Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(NCD NCD.cc ${sources} ${headers})
target_link_libraries(NCD ${Geant4_LIBRARIES} )

add_custom_target(SNOLABNCD DEPENDS NCD)
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build NeutronSource. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(NCD_SCRIPTS
    FluxNeutrons.mac
  )
#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS NCD DESTINATION bin)

# Install macro files to the build directory
foreach(script ${NCD_SCRIPTS})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${script}
            DESTINATION ${CMAKE_BINARY_DIR})
endforeach()
