# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/hz7/Documents/c++/sfml/game/_deps/sfml-src")
  file(MAKE_DIRECTORY "/home/hz7/Documents/c++/sfml/game/_deps/sfml-src")
endif()
file(MAKE_DIRECTORY
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-build"
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix"
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/tmp"
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/src"
  "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/hz7/Documents/c++/sfml/game/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
