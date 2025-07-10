#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define CHECK(expr) if (!expr) { log::warn("Failed: " #expr); }

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(BasedGameLayer, GJBaseGameLayer) {
	// to be honest it's probably already over if for some reason some other mod wanted to modify these methods
	// but hey, this probably won't make it worse
	static void onModify(auto& self) {
		CHECK(self.setHookPriorityPre("GJBaseGameLayer::moveObjectToStaticGroup", Priority::Last))
		CHECK(self.setHookPriorityPost("GJBaseGameLayer::optimizeMoveGroups", Priority::First))
	}

	void moveObjectToStaticGroup(GameObject* obj) $override;
	void optimizeMoveGroups() $override;

	struct Fields {
		std::map<int, std::set<GameObject*>> m_toMoveToStaticGroup;
	};
};

#undef CHECK
