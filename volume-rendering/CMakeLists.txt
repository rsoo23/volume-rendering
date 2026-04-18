# CMakeList.txt : CMake project for volume-rendering, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (volume-rendering "main.cpp" "pch.h" "sphere.cpp" "utils.cpp" "ray-march.cpp" "custom-color.cpp" "point-light.cpp" "random-number-generator.cpp" "timer.cpp" "noise.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET volume-rendering PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(volume-rendering PRIVATE raylib)
