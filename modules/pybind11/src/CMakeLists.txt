################################################
## Setup
################################################

# Set the module name and installation paths based on target python version
if(PYTHON_VERSION_MAJOR EQUAL 2)
  set(_pythonlibpath ${_lib}/python2.${PYTHON_VERSION_MINOR}/dist-packages)
  set(_component_name "python")
elseif(PYTHON_VERSION_MAJOR EQUAL 3)
  set(_pythonlibpath ${_lib}/python3/dist-packages)
  set(_component_name "python3")
else()
  message(
    FATAL_ERROR "Unexpected PYTHON_VERSION_MAJOR: ${PYTHON_VERSION_MAJOR}"
    )
endif()

if(FORCE_OPENCV3)
  find_package(OpenCV 3 REQUIRED COMPONENTS core)
elseif(FORCE_OPENCV2)
  find_package(OpenCV 2.4 REQUIRED COMPONENTS core)
else()
  find_package(OpenCV REQUIRED COMPONENTS core)
endif()

# needs to be done before the target is declared
link_directories(${OpenCV_LIBRARY_DIRS})

################################################
## Target: ifm3dpy.so
## (Note: Name of the target maps directly to
## the name of the resulting python module, so
## we break from the naming convention found in
## other ifm3d components here.
################################################
add_library(ifm3dpy MODULE main.cpp)

#------------------
# Compiler settings
#------------------
target_include_directories(ifm3dpy
  PRIVATE
    $<BUILD_INTERFACE:${IFM3D_FG_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_CAMERA_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_OPENCV_BINARY_DIR}/include>
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
  )

#------------------
# Linker Settings
#------------------
target_link_libraries(
  ifm3dpy
  PRIVATE
    ifm3d_framegrabber_shared
    ifm3d_camera_shared
    ${OpenCV_LIBRARIES}
    pybind11::module
  )

#------------------
# Properties
#------------------
set_target_properties(
  ifm3dpy PROPERTIES
  EXPORT_NAME ${_component_name}
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}"
  )

#------------------
# Installation
#------------------
install(TARGETS ifm3dpy
  RUNTIME DESTINATION ${_pythonlibpath} COMPONENT ${_component_name}
  LIBRARY DESTINATION ${_pythonlibpath} COMPONENT ${_component_name}
  ARCHIVE DESTINATION ${_pythonlibpath} COMPONENT ${_component_name}
  PUBLIC_HEADER DESTINATION ${_include} COMPONENT ${_component_name}
  )
