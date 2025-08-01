#include "main.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void LeveledInfoLayer::loadLevelStep() $override {
	auto last = std::chrono::steady_clock::now();
	auto interval = std::chrono::nanoseconds(static_cast<long long>(CCDirector::get()->getAnimationInterval() * 1'000'000'000));

	auto play = static_cast<PlayLayer*>(m_playScene->getChildren()->objectAtIndex(0)); // original
	do {
		play->processCreateObjectsFromSetup(); // original
	} while (std::chrono::steady_clock::now() - last <= interval && play->m_loadingProgress < 1.f);

	// rest is original
	m_progressTimer->setPercentage(play->m_loadingProgress * 100.f);

	runAction(CCSequence::create(
		CCDelayTime::create(0.f),
		CCCallFunc::create(this, static_cast<SEL_CallFunc>(
			play->m_loadingProgress < 1.f
			? &LevelInfoLayer::loadLevelStep
			: &LevelInfoLayer::playStep4
		)),
		nullptr
	));
}
