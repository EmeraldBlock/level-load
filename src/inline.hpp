#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// these are inlined on windows but are missing some bindings elsewhere
// this is probably UB but whatever

inline bool GameObject::canRotateFree() {
	auto type = m_objectType;
	return (
		type != GameObjectType::Solid
		&& type != GameObjectType::Breakable
		&& type != GameObjectType::Slope
	) || m_isNoTouch;
}

inline bool GameObject::hasSecondaryColor() {
	return m_colorSprite;
}

inline void GameObject::resetRScaleForced() {
	m_fScaleX = 0.f;
	m_fScaleY = 0.f;
	setRScaleX(1.f);
	setRScaleY(1.f);
}

inline void GameObject::setCustomZLayer(int zLayer) {
	if (m_zFixedZLayer) return;
	m_zLayer = static_cast<ZLayer>(zLayer);
}

inline void ParticleGameObject::updateParticleStruct() {
	if (!m_updatedParticleData) return;
	m_updatedParticleData = false;
	GameToolbox::particleStringToStruct(m_particleData, m_particleStruct);
}
