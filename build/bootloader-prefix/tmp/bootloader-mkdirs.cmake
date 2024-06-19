# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Daniel/espIDF/esp-idf-v5.1.1/components/bootloader/subproject"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/tmp"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/src"
  "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Daniel/Documents/esptest/test_tcp_sec/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
