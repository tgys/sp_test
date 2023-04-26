# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest"
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest-build"
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build"
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/tmp"
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest-stamp"
  "/home/tei/projects/cpp/test-questions/Downloads"
  "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/tei/projects/cpp/test-questions/cmake-build-debug/build/src/GTest-stamp${cfgdir}") # cfgdir has leading slash
endif()
