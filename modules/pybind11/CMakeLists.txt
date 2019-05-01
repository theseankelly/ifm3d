project(IFM3D_PYBIND11 CXX)
set(PDS_CLIENT_PYBIND11_NAME "libifm3d-pybind11")

file(COPY ${IFM3D_PYBIND11_SOURCE_DIR}/include
  DESTINATION ${IFM3D_PYBIND11_BINARY_DIR}
  )

################################################
## Process child CMakeLists.txt files
################################################

# This pulls in pybind11 CMake macros
add_subdirectory(include/contrib/pybind11)

add_subdirectory(src)