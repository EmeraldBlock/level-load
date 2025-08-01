#include "main.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void LeveledInfoLayer::loadLevelStep() $override {
	LARGE_INTEGER last, now;
	QueryPerformanceCounter(&last);
	auto interval = CCApplication::get()->m_nAnimationInterval.QuadPart;

	auto play = static_cast<PlayLayer*>(m_playScene->getChildren()->objectAtIndex(0)); // original
	do {
		play->processCreateObjectsFromSetup(); // original
		QueryPerformanceCounter(&now);
	} while (now.QuadPart - last.QuadPart <= interval && play->m_loadingProgress < 1.f);

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
