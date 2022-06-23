#include "Scene.hpp"
#include "SceneManager.hpp"

namespace bomberman::scene {
void Scene::change(const SceneId &sceneId) { m_sceneMgr.change(sceneId); }
} // namespace bomberman::scene