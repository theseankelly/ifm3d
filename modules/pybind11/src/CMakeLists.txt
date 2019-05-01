################################################
## Target: ifm3dpy.so
################################################
add_library(ifm3dpy MODULE main.cpp)

#------------------
# Compiler settings
#------------------
target_include_directories(ifm3dpy
  PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${IFM3D_IMAGE_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_FG_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_CAMERA_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${IFM3D_OPENCV_BINARY_DIR}/include>
    ${OpenCV_INCLUDE_DIRS}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
  )

#target_compile_options(ifm3dpy
#  PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std=c++${CMAKE_CXX_STANDARD}>
#  )

#------------------
# Linker Settings
#------------------
target_link_libraries(
  ifm3dpy
  PUBLIC
    ifm3d_image_shared
    ifm3d_framegrabber_shared
    ifm3d_camera_shared
    ${OpenCV_LIBRARIES}
  PRIVATE
    pybind11::module
  )

#------------------
# Properties
#------------------
set_target_properties(ifm3dpy PROPERTIES
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}"
  )
