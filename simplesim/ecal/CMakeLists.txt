
cmake_minimum_required(VERSION 3.16...3.21)
project(simple-ecal)

#
# find G4 package
option(WITH_GEANT4_UIVIS "Build simulation with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

set(HDF5DIR /usr/local/HDF_Group/HDF5/1.14.5)

#
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${HDF5DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#
# executable and library links
add_executable(simple-ecal simple-ecal.cc ${sources} ${headers})
target_link_libraries(simple-ecal ${Geant4_LIBRARIES})
target_link_directories(simple-ecal PRIVATE ${HDF5DIR}/lib)
target_link_libraries(simple-ecal libhdf5.so)


#
# copy scripts to the build directory
set(SIMPLE_ECAL_SCRIPTS
    run.mac
    )

foreach(_script ${SIMPLE_ECAL_SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
        )
endforeach()


#
# install targets
install(TARGETS simple-ecal DESTINATION bin)

