#include "SceneBase.hpp"
#include "SceneManager.hpp"
#include "SharedData.hpp"

namespace bm::scene {
SceneBase::SceneBase(SceneManager &sceneManager) : sceneManager(sceneManager) {}

SceneBase::~SceneBase() {}

SharedData &SceneBase::shared() { return sceneManager.shared(); }

void SceneBase::change(const SceneId &sceneId) { sceneManager.change(sceneId); }

} // namespace bm::scene
