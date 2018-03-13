/*
 * Copyright (C) 2018 ifm electronic, gmbh
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

#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>
#include <ifm3d/camera.h>
#include <ifm3d/fg.h>
#include <ifm3d/image.h>

int main_test_sensor1()
{
    int retval = 0;

    ifm3d::Camera::Ptr base = std::make_shared<ifm3d::Camera>
      (ifm3d::DEFAULT_IP, 80, ifm3d::DEFAULT_PASSWORD);

    std::cout << "Launch the sensor O3X." << std::endl;
    sleep(6);
    std::cout << "Verification of the sensor type." << std::endl;

    try
    {
        if (base->IsO3X())
        {
            json dump = base->ToJSON();
            std::string sFrameRate =
              dump["ifm3d"]["Apps"][0]["Imager"]["FrameRate"];
            std::cout << "Framerate is : " << sFrameRate << std::endl;
        }
        else
        {
            std::cout << "Sensor is not an O3X." << std::endl;
        }
    }
    catch (const ifm3d::error_t& ex)
    {
        if (ex.code() == IFM3D_XMLRPC_TIMEOUT)
        {
            std::cout << "Timeout." << std::endl;
            retval = -1;
        }
        else
        {
            std::cout << "Sensor is not connected." << std::endl;
            retval = -2;
        }
    }

    return retval;
}

int main_test_sensor2()
{
    int retval = 0;

    ifm3d::Camera::Ptr cam;
    ifm3d::FrameGrabber::Ptr fg;
    ifm3d::ImageBuffer::Ptr im;

    try
    {
        std::cout << "Launch the driver O3X." << std::endl;

        cam = ifm3d::Camera::MakeShared(ifm3d::DEFAULT_IP,
                                        80, ifm3d::DEFAULT_PASSWORD);
        sleep(6);
        fg = std::make_shared<ifm3d::FrameGrabber>(
          cam, ifm3d::DEFAULT_SCHEMA_MASK);
        im = std::make_shared<ifm3d::ImageBuffer>();

        std::cout << "Verification of the sensor type." << std::endl;
    }
    catch (const ifm3d::error_t& ex)
    {
        std::cout << "Sensor is not connected." << std::endl;
    }

    sleep(10);

    try
    {
        if (cam->IsO3X())
        {
            json dump = cam->ToJSON();
            std::string sFrameRate =
              dump["ifm3d"]["Apps"][0]["Imager"]["FrameRate"];
            std::cout << "Framerate is : " << sFrameRate << std::endl;
        }
        else
        {
            std::cout << "Sensor is not an O3X." << std::endl;
        }
    }
    catch (const ifm3d::error_t& ex)
    {
        if (ex.code() == IFM3D_XMLRPC_TIMEOUT)
        {
            std::cout << "Timeout." << std::endl;
            retval = -1;
        }
        else
        {
            std::cout << "Sensor is not connected." << std::endl;
            retval = -2;
        }
    }

    return retval;
}

int main(int argc, const char **argv)
{
  //return 0;
  //return main_test_sensor1();
  //return main_test_sensor2();

  int retval = 0;
  for (int i = 0; i < 10; ++i)
    {
      retval = main_test_sensor2();
    }
  return retval;
}
