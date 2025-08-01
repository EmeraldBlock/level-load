#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define CHECK(expr) if (!expr) { log::warn("Failed: " #expr); }

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(BasedGameLayer, GJBaseGameLayer) {
	// to be honest it's probably already over if for some reason some other mod wanted to modify these methods
	// but hey, this probably won't make it worse
	static void onModify(auto& self) {
		// CHECK(self.setHookPriorityPre("GJBaseGameLayer::moveObjectToStaticGroup", Priority::Last))
		CHECK(self.setHookPriorityPost("GJBaseGameLayer::optimizeMoveGroups", Priority::First))
	}

	void _moveObjectToStaticGroup(GameObject* obj); // override
	void optimizeMoveGroups() $override;

	struct Fields {
		std::map<int, std::set<GameObject*>> m_toMoveToStaticGroup;
	};
};

$execute {
	Mod::get()->hook(
		reinterpret_cast<void*>(geode::base::get() + 0x22c060),
		+[](BasedGameLayer* self, GameObject* obj) {
			return self->_moveObjectToStaticGroup(obj);
		},
		"GJBaseGameLayer::moveObjectToStaticGroup",
		tulip::hook::TulipConvention::Thiscall
	).unwrap()->setPriority(Priority::Last);
}

#endif

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(LeveledInfoLayer, LevelInfoLayer) {
	static void onModify(auto& self) {
		CHECK(self.setHookPriorityPre("LevelInfoLayer::loadLevelStep", Priority::Last))
	}

	void loadLevelStep() $override;
};

struct PropsCache;

#include <Geode/modify/GameObject.hpp>
class $modify(GamedObject, GameObject) {
	static GameObject* newObjectFromVector(gd::vector<gd::string>& strs, gd::vector<void*>& games, GJBaseGameLayer* game, bool ldm, PropsCache const& cache);
};

#include <Geode/modify/PlayLayer.hpp>
class $modify(PlayedLayer, PlayLayer) {
	static void onModify(auto& self) {
		CHECK(self.setHookPriorityPost("PlayLayer::prepareCreateObjectsFromSetup", Priority::Last))
		CHECK(self.setHookPriorityPre("PlayLayer::processCreateObjectsFromSetup", Priority::Last))
	}

	void prepareCreateObjectsFromSetup(gd::string& p0) $override;
	void processCreateObjectsFromSetup() $override;
};

#undef CHECK

// a guess
inline bool GameObject::canRotateFree() {
	auto type = m_objectType;
	return (
		type != GameObjectType::Solid
		&& type != GameObjectType::Breakable
		&& type != GameObjectType::Slope
	) || m_isNoTouch;
}
