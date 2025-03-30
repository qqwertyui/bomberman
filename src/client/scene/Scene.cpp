#include "Scene.hpp"
#include "SceneManager.hpp"

namespace bm::scene {
void Scene::change(const SceneId &sceneId) { m_sceneMgr.change(sceneId); }
} // namespace bm::scene
