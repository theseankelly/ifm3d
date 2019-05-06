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

namespace ifm3d
{
  template<typename T>
  py::array_t<T> cloud_to_array(const cv::Mat& cld)
  {
    std::cout << "Converting array of type " << cld.type() << std::endl;

    // Alloc a new cv::Mat_<T> and tie its lifecycle to the Python object
    // via a capsule. The resulting numpy.ndarray will not own the memory, but
    // the memory will remain valid for the lifecycle of the object.
    auto mat = new cv::Mat_<cv::Vec<T, 3>>(cld);
    auto capsule = py::capsule(
      mat,
      [](void *m)
      {
        std::cout << "Releasing cloud capsule memory" << std::endl;
        delete reinterpret_cast<cv::Mat_<cv::Vec<T, 3>>*>(m);
      });

    return py::array_t<T>(
      { mat->rows, mat->cols, mat->channels() },
      { sizeof(T) * mat->channels() * mat->cols,
        sizeof(T) * mat->channels(),
        sizeof(T)},
      reinterpret_cast<T*>(mat->ptr(0)),
      capsule);
  }

  template<typename T>
  py::array_t<T> image_to_array(const cv::Mat& img)
  {
    std::cout << "Converting array of type " << img.type() << std::endl;

    // Alloc a new cv::Mat_<T> and tie its lifecycle to the Python object
    // via a capsule. The resulting numpy.ndarray will not own the memory, but
    // the memory will remain valid for the lifecycle of the object.
    auto mat = new cv::Mat_<T>(img);
    auto capsule = py::capsule(
      mat,
      [](void *m)
      {
        std::cout << "Releasing image capsule memory" << std::endl;
        delete reinterpret_cast<cv::Mat_<cv::Vec<T, 3>>*>(m);
      });

    return py::array_t<T>(
      { mat->rows, mat->cols },
      { sizeof(T) * mat->cols, sizeof(T)},
      reinterpret_cast<T*>(mat->ptr(0)),
      capsule);
  }
}

#endif // IFM3D_PY_UTIL_HPP
