# CMakeList.txt : CMake project for volume-rendering, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (volume-rendering "main.cpp" "pch.h" "Sphere.cpp" "Utils.cpp" "RayMarch.cpp" "CustomColor.cpp" "PointLight.cpp" "RandomNumberGenerator.cpp" "Timer.cpp" "Noise.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET volume-rendering PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(volume-rendering PRIVATE raylib)
