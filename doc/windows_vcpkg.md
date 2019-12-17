# Windows: Using vcpkg to simplify source build

The `ifm3d` library takes dependencies on several large projects. While these
libraries are easily installed on Linux systems, they must be compiled from
source on Windows machines.

[`Vcpkg`](https://github.com/microsoft/vcpkg) is a cross platform package
manager developed Microsoft to simplify the acquisition and compilation of
commonly used source C++ libraries (as well as their dependencies).

These instructions detail how to use `vcpkg` to simplify the compilation steps
required for `ifm3d`.

# Vcpkg Setup

1. [Set up a `vcpkg` workspace](https://github.com/microsoft/vcpkg/blob/master/README.md).
2. This guide uses the `IFM3D_VCPKG_PATH` environment variable to indicate the
root path to your `vcpkg` workspace (where `vcpkg.exe` can be found). Set the
following variable accordingly:
    ```
    set IFM3D_VCPKG_PATH=c:\dev\vcpkg
    ```
2. Install the dependencies required for your target modules based on the
following table:

    <table>
      <tr>
        <th>Module</th>
        <th>Packages</th>
      </tr>
      <tr>
        <td>camera (required)</td>
        <td>curl, glog</td>
      </tr>
      <tr>
        <td>swupdater</td>
        <td>curl</td>
      </tr>
      <tr>
        <td>framegrabber</td>
        <td>boost-asio</td>
      </tr>
      <tr>
        <td>opencv</td>
        <td>opencv3</td>
      </tr>
      <tr>
        <td>image**</td>
        <td>opencv3, pcl</td>
      </tr>
      <tr>
        <td>tools</td>
        <td>boost-program-options</td>
      </tr>
      <tr>
        <td>SWUpdater</td>
        <td>curl, glog</td>
      </tr>
     </table>

    ** **NOTE**: The `pcl` library is extremely large and takes multiple hours to
    compile. Since `pcl` is becomming less commonly used, this guide explicitly
    disables `pcl` support.

    For example, to install dependencies for all modules on x64 (again,
    skipping `pcl`) execute the following:

    ```
    %IFM3D_VCPKG_PATH%\vcpkg install curl:x64-windows glog:x64-windows boost-asio:x64-windows boost-program-options:x64-windows opencv3:x64-windows
    ```

3. Export the `vcpkg` workspace so it's accessible to `cmake`:
    ```
    %IFM3D_VCPKG_PATH%\vcpkg integrate install
    ```

# Compile additional dependencies

Not all `ifm3d` dependencies are currently available via `vcpkg`. Compile
additional dependencies as follows:

## Environment

First we need to set some variables, change these according to your installation.
```
set IFM3D_CMAKE_GENERATOR="Visual Studio 15 2017 Win64"
set IFM3D_BUILD_DIR=C:\ifm3d
```

Setting the Configuration of the build
```
set CONFIG=Release
```
For other build change the CONFIG variable accordingly e.g. for Debug build
"set CONFIG=Debug"

## Getting the dependencies
Create the work directory
```
mkdir %IFM3D_BUILD_DIR%
```

Install all the dependencies

### [xmlrpc-c](http://xmlrpc-c.sourceforge.net/)
Download:
```
cd %IFM3D_BUILD_DIR%
git clone --branch fix_cmake https://github.com/theseankelly/xmlrpc-c.git
```

Build:
```
cd %IFM3D_BUILD_DIR%\xmlrpc-c
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_TOOLCHAIN_FILE=%IFM3D_VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install ..
cmake --build . --clean-first --config %CONFIG% --target INSTALL
```

## Building ifm3d
Download:
```
cd %IFM3D_BUILD_DIR%
git clone --branch vcpkg https://github.com/theseankelly/ifm3d.git
```
To build the OpenCV module instead of the PCL use following additional compiler flag
```
-DBUILD_MODULE_OPENCV=ON -DBUILD_MODULE_IMAGE=OFF
```
Build
```
cd %IFM3D_BUILD_DIR%\ifm3d
mkdir build
cd build
cmake -G %IFM3D_CMAKE_GENERATOR% -DCMAKE_TOOLCHAIN_FILE=%IFM3D_VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DBUILD_SDK_PKG=ON -DCMAKE_PREFIX_PATH="%IFM3D_BUILD_DIR%\install" -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_INSTALL_PREFIX=%IFM3D_BUILD_DIR%\install -DBUILD_MODULE_IMAGE=OFF -DBUILD_MODULE_OPENCV=ON -DBUILD_TESTS=OFF ..

#run tests
cmake --build . --config %CONFIG% --target check

#install
cmake --build . --clean-first --config %CONFIG% --target INSTALL
```

# Running ifm3d tool on Windows.

TODO: Need to add things to path just like on regular windows. Need to read up
and find the proper way to add `vcpkg` paths to the system path.
