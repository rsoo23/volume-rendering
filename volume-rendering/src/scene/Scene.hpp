#pragma once

#include "utils/CustomColor.hpp"
#include "scene_objects/Light.hpp"
#include "scene_objects/SceneObject.hpp"

class Scene {
	public:
		Scene();
		Scene(CustomColor bgColor);
		~Scene();

		void		addLight(std::unique_ptr<Light> light);
		void		addSceneObject(std::unique_ptr<SceneObject> sceneObj);
		CustomColor getBGColor() const;

		const std::vector<std::unique_ptr<SceneObject>>&	getSceneObjects() const;
		const std::vector<std::unique_ptr<Light>>&			getLights() const;

	private:
		CustomColor									bgColor;
		std::vector<std::unique_ptr<SceneObject>>	sceneObjects;
		std::vector<std::unique_ptr<Light>>			lights;
};