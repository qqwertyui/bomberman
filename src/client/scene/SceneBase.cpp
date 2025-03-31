#include "SceneBase.hpp"
#include "SceneManager.hpp"

namespace bm::scene {
SceneBase::SceneBase(SceneManager &sceneManager) : sceneManager(sceneManager) {}

SceneBase::~SceneBase() {}

sf::RenderWindow &SceneBase::getWindow() { return sceneManager.getWindow(); }

void SceneBase::change(const SceneId &sceneId) { sceneManager.change(sceneId); }

} // namespace bm::scene
