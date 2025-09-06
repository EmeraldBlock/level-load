#include "main.hpp"
#include "PropsCache.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace {

class PropsComputer {
	struct Job {
		std::atomic<bool> ready;
		// input
		gd::string const* objStr;
		// output
		gd::vector<gd::string> strs{}; // needed because of an explicit constructor error on Android
		gd::vector<void*> games{}; // ditto
		PropsCache cache;
	};
	std::thread computeThread; // poor support for `std::jthread` :(
	GJBaseGameLayer* gameLayer;
	bool active{false};
	std::array<Job, 2> jobs{};
	int jq;
	int jf;

public:
	void process() {
		for (int j = 0;; j = 1 - j) {
			auto& job = jobs[j];
			while (!job.ready) ;
			if (!active) break;
			auto str = *job.objStr;
			// this is a lot different than the decompilation, hope it's the same
			std::ranges::fill(job.games, nullptr);
			job.cache = {};
			auto begin = str.data();
			auto end = begin + str.size();
			for (auto it = begin; it != end; ++it) {
				if (*it == ',') *it = '\0';
			}
			auto it = begin;
			while (true) {
				auto pos = atoi(it);
				while (*it != '\0') ++it;
				if (it == end) break;
				++it;
				if (0 < pos && pos < 600) {
					#ifdef GEODE_IS_ANDROID
					// otherwise the old string's refcount gets decremented twice due to a Geode bug, I think.
					// this seems to only manifest with the assignment of `m_particleData` in `ParticleGameObject::customObjectSetup`,
					// because the copy-on-write policy causes the string to be shared, and it isn't immediately processed
					job.strs[pos].clear();
					#endif
					job.strs[pos] = it;
					job.games[pos] = gameLayer;
					job.cache.cacheProp(pos, it);
				}
				while (*it != '\0') ++it;
				if (it == end) break;
				++it;
			}
			job.ready = false;
		}
	}

	void start(GJBaseGameLayer* layer) {
		gameLayer = layer;
		active = true;
		for (auto& job : jobs) {
			job.ready = false;
			job.strs.resize(600);
			job.games.resize(600);
		}
		jq = 0;
		jf = 0;
		computeThread = std::thread{&PropsComputer::process, this};
	}

	// user's responsibility to not over-call
	void queue(gd::string const& str) {
		jobs[jq].objStr = &str;
		jobs[jq].ready = true;
		jq = 1 - jq;
	}

	// user's responsibility to not over-call
	Job& fetch() {
		while (jobs[jf].ready) ;
		auto& job = jobs[jf];
		jf = 1 - jf;
		return job;
	}

	void finish() {
		active = false;
		jobs[jq].ready = true;
		computeThread.join();
		computeThread = {};
		for (auto& job : jobs) {
			job.strs.clear();
			job.games.clear();
		}
	}

	// user's responsibility to deal with unfetched jobs
	void tryStart(GJBaseGameLayer* layer) {
		if (active) return;
		start(layer);
	}

	void tryFinish() {
		if (!active) return;
		finish();
	}
};

PropsComputer pc; // global, whatever

}

// should be the same as original, besides the usage of `PropsCache` and `PropsComputer`
void PlayedLayer::processCreateObjectsFromSetup() {
	auto n = m_objectStrings.size();
	auto kilos = n / 1000; // this took a bit to reverse
	auto frac = std::min(100.f, std::roundf(kilos) + 10.f);
	int numToCreate = std::ceil(n / frac);

	auto inRange = [&](int i) {
		// ull and int comparison, can't merge in some cases (said cases raise bigger questions though)
		return i < n && i < m_objectsCreated + numToCreate;
	};

	// this should never be false (except empty levels maybe), but what if it is!
	if (inRange(m_objectsCreated)) {

		pc.tryStart(this);
		pc.queue(m_objectStrings[m_objectsCreated]);

		for (auto i = m_objectsCreated; inRange(i); ++i) {

			if (inRange(i + 1)) {
				pc.queue(m_objectStrings[i + 1]);
			}

			auto& job = pc.fetch();

			auto obj = GamedObject::newObjectFromVector(job.strs, job.games, this, m_level->m_lowDetailModeToggled, job.cache);
			if (!obj) continue;

			if (obj->m_objectID == 31) {
				static_cast<StartPosObject*>(obj)->loadSettingsFromString(*job.objStr);
			} else if (obj->m_objectID == 2065) {
				static_cast<ParticleGameObject*>(obj)->updateParticleStruct();
			}
			if (obj->getType() == GameObjectType::SecretCoin && m_level->m_levelType != GJLevelType::Main) continue;
			if (obj->m_mainColorKeyIndex < 1) {
				auto colors = 1 + obj->hasSecondaryColor();
				for (int i = 0; i < colors; ++i) {
					auto key = obj->getColorKey(i == 0, false);
					auto color = static_cast<CCInteger*>(m_colorKeyDict->objectForKey(key));
					int value;
					if (color) {
						value = color->m_nValue;
					} else {
						value = m_nextColorKey;
						m_colorKeyDict->setObject(CCInteger::create(value), key);
						++m_nextColorKey;
					}
					(i == 0 ? obj->m_mainColorKeyIndex : obj->m_detailColorKeyIndex) = value;
				}
			} else {
				m_nextColorKey = std::max({
					m_nextColorKey,
					obj->m_mainColorKeyIndex,
					obj->m_detailColorKeyIndex,
				});
			}
			if (obj->getType() == GameObjectType::UserCoin) {
				if (m_coinArray->count() < 3) {
					m_coinArray->addObject(obj);
					addObject(obj);
				}
			} else {
				addObject(obj);
			}
		}
	}

	m_objectsCreated += numToCreate;
	m_loadingProgress = std::min(1.f, (m_objectsCreated - 1.f) / m_objectStrings.size());
	if (m_objectsCreated < m_objectStrings.size()) {
		return;
	}
	pc.tryFinish();
	createObjectsFromSetupFinished();
	m_loadingProgress = 1.f;
	setupHasCompleted();
}
