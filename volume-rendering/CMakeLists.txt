# CMakeList.txt : CMake project for volume-rendering, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(volume-rendering
	"main.cpp"
	"pch.h"
	"src/core/Application.cpp"
	"src/math/Noise.cpp"
	"src/math/RandomNumberGenerator.cpp"
	"src/render/RayMarch.cpp"
	"src/render/Renderer.cpp"
	"src/render/SimpleCalcVolumeColor.cpp"
	"src/scene/Scene.cpp"
	"src/scene_objects/CustomCamera.cpp"
	"src/scene_objects/Light.cpp"
	"src/scene_objects/Sphere.cpp"
	"src/shared/Types.h"
	"src/utils/CustomColor.cpp"
	"src/utils/Timer.cpp"
)

target_include_directories(volume-rendering PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET volume-rendering PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_link_libraries(volume-rendering PRIVATE raylib)
