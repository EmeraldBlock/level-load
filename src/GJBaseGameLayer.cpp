#include "main.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

/**
 * In place of `m_toMoveToStaticGroup`, normally adds to static group (unless already contained)
 * and removes (with release) from optimized group, creating the arrays for said groups if they don't exist.
 */
void BasedGameLayer::_moveObjectToStaticGroup(GameObject* obj) {
	if (!obj->m_isDecoration2) return;
	obj->m_isDecoration2 = false;
	for (int i = 0; i < obj->m_groupCount; ++i) {
		auto gid = i < 10 && obj->m_groups ? std::clamp(+(*obj->m_groups)[i], 0, 9999) : 0;
		m_fields->m_toMoveToStaticGroup[gid].insert(obj);
	}
}

void BasedGameLayer::optimizeMoveGroups() {
	GJBaseGameLayer::optimizeMoveGroups();

	for (auto& [gid, objs] : m_fields->m_toMoveToStaticGroup) {

		auto& usGroup = m_staticGroups[gid];
		if (!usGroup) {
			usGroup = CCArray::create();
			m_staticGroupDict->setObject(usGroup, gid);
		}
		auto sGroup = CCArrayExt<GameObject*>{usGroup};

		auto& uoGroup = m_optimizedGroups[gid];
		if (!uoGroup) {
			uoGroup = CCArray::create();
			m_optimizedGroupDict->setObject(uoGroup, gid);
		}
		auto oGroup = CCArrayExt<GameObject*>{uoGroup};

		int j = 0;
		for (int i = 0; i < oGroup.size(); ++i) {
			if (objs.contains(oGroup[i])) continue;
			oGroup.inner()->exchangeObjectAtIndex(i, j);
			++j;
		}
		for (auto obj : sGroup) {
			objs.erase(obj);
		}
		for (auto obj : objs) {
			sGroup.push_back(obj);
		}
		objs.clear();
		while (oGroup.size() > j) {
			oGroup.inner()->removeLastObject(true);
		}
	}
	m_fields->m_toMoveToStaticGroup.clear();
}

#endif
