#include "Scene.hpp"
#include "SceneManager.hpp"

namespace SimpleSnake::scene {
void Scene::change(const SceneId &sceneId) { m_sceneMgr.change(sceneId); }
} // namespace SimpleSnake::scene