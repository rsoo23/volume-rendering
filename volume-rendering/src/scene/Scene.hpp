#pragma once

#include "utils/CustomColor.hpp"
#include "scene_objects/Light.hpp"
#include "scene_objects/SceneObject.hpp"
#include "scene_objects/Atmosphere.hpp"

class Scene {
	public:
		Scene();
		Scene(CustomColor bgColor);
		~Scene();

		void		setAtmosphere(std::unique_ptr<Atmosphere> atmosphere);
		void		addLight(std::unique_ptr<Light> light);
		void		addSceneObject(std::unique_ptr<SceneObject> sceneObj);
		CustomColor getBGColor() const;
		
		const Atmosphere&									getAtmosphere() const;
		const std::vector<std::unique_ptr<SceneObject>>&	getSceneObjects() const;
		const std::vector<std::unique_ptr<Light>>&			getLights() const;

	private:
		CustomColor									bgColor;
		std::unique_ptr<Atmosphere>					atmosphere;
		std::vector<std::unique_ptr<SceneObject>>	sceneObjects;
		std::vector<std::unique_ptr<Light>>			lights;
};