/*
 * Copyright (C) 2019 ifm electronic, gmbh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distribted on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IFM3D_PY_UTIL_HPP
#define IFM3D_PY_UTIL_HPP

#include <algorithm>
#include <typeindex>
#include <vector>

#include <opencv2/core/core.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

template<typename T>
void declare_image_class(py::module &m, const std::string &typestr)
{
  std::string pyclass_name = std::string("_Image_") + typestr;
  py::class_<cv::Mat_<T>>(m, pyclass_name.c_str(), py::buffer_protocol())
    .def_buffer([](cv::Mat_<T> &mat) -> py::buffer_info
    {
      if (mat.channels() == 1)
        {
          std::cout << "n channels = " << mat.channels() << std::endl;
          std::cout << "addr = " << std::hex << mat.ptr(0) << std::endl;
          return py::buffer_info(
            mat.ptr(0),
            sizeof(T),
            py::format_descriptor<T>::format(),
            2,
            { mat.rows, mat.cols },
            { sizeof(T) * mat.cols, sizeof(T)});
        }
      else
        {
          std::cout << "n channels = " << mat.channels() << std::endl;
          return py::buffer_info(
            mat.ptr(0),
            sizeof(T),
            py::format_descriptor<T>::format(),
            mat.channels(),
            { mat.rows, mat.cols, mat.channels() },
            { sizeof(T) * mat.channels() * mat.cols,
              sizeof(T) * mat.channels(),
              sizeof(T)});

        }
    });
}

template<typename T>
void declare_cloud_class(py::module &m, const std::string &typestr)
{
  std::string pyclass_name = std::string("_Cloud_") + typestr;
  py::class_<cv::Mat_<cv::Vec<T,3>>>(m, pyclass_name.c_str(), py::buffer_protocol())
    .def_buffer([](cv::Mat_<cv::Vec<T,3>> &mat) -> py::buffer_info
    {
      if (mat.channels() == 1)
        {
          std::cout << "n channels = " << mat.channels() << std::endl;
          return py::buffer_info(
            mat.ptr(0),
            sizeof(T),
            py::format_descriptor<T>::format(),
            2,
            { mat.rows, mat.cols },
            { sizeof(T) * mat.cols, sizeof(T)});
        }
      else
        {
          std::cout << "n channels = " << mat.channels() << std::endl;
          return py::buffer_info(
            mat.ptr(0),
            sizeof(T),
            py::format_descriptor<T>::format(),
            mat.channels(),
            { mat.rows, mat.cols, mat.channels() },
            { sizeof(T) * mat.channels() * mat.cols,
              sizeof(T) * mat.channels(),
              sizeof(T)});

        }
    });
}

#endif // IFM3D_PY_UTIL_HPP
