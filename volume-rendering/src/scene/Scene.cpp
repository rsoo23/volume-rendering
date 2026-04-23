#include "pch.h"
#include "Scene.hpp"

Scene::Scene(): bgColor(BLACK) {};

Scene::Scene(CustomColor bgColor): bgColor(bgColor) {}

Scene::~Scene() {};

void Scene::addLight(std::unique_ptr<Light> light) {
	this->lights.push_back(std::move(light));
};

void Scene::addSceneObject(std::unique_ptr<SceneObject> sceneObj) {
	this->sceneObjects.push_back(std::move(sceneObj));
}

CustomColor Scene::getBGColor() const {
	return this->bgColor;
}

const std::vector<std::unique_ptr<SceneObject>>& Scene::getSceneObjects() const {
	return this->sceneObjects;
}

const std::vector<std::unique_ptr<Light>>& Scene::getLights() const {
	return this->lights;
}
