#include "SceneBase.hpp"
#include "SceneManager.hpp"

namespace bm::scene {
void SceneBase::change(const SceneId &sceneId) { m_sceneMgr.change(sceneId); }
} // namespace bm::scene
