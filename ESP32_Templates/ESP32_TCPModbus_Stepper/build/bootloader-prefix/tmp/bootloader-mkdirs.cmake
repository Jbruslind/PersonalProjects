# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/GitHub/esp-idf/components/bootloader/subproject"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/tmp"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/src/bootloader-stamp"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/src"
  "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GitHub/PersonalProjects/ESP32_Templates/ESP32_TCPModbus_Stepper/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
