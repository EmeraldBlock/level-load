#include "main.hpp"
#include "PropsCache.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

// original behavior
// #define GET_INT(i) (games[i] ? atoi(strs[i].c_str()) : 0)
// #define GET_BOOL(i) (GET_INT(i) != 0)
// #define GET_SHORT(i) static_cast<short>(GET_INT(i))
// #define GET_DOUBLE(i) (games[i] ? atof(strs[i].c_str()) : 0.)
// #define GET_FLOAT(i) static_cast<float>(GET_DOUBLE(i))
// #define GET_STRING(i) (games[i] ? strs[i] : gd::string{})

#define GET_TYPE(t, i) ([] { static_assert(std::is_same_v<t, decltype(cache.m_##i)>); }, cache.m_##i)
#define GET_INT(i) GET_TYPE(int, i)
#define GET_BOOL(i) GET_TYPE(bool, i)
#define GET_SHORT(i) GET_TYPE(short, i)
#define GET_DOUBLE(i) GET_TYPE(double, i)
#define GET_FLOAT(i) GET_TYPE(float, i)
#define GET_STRING(i) GET_TYPE(gd::string, i)

// should be the same as original when using "original behavior"
GameObject* GamedObject::newObjectFromVector(gd::vector<gd::string>& strs, gd::vector<void*>& games, GJBaseGameLayer* game, bool ldm, PropsCache const& cache) {
	auto isHighDetail = GET_BOOL(103);
	if (ldm && isHighDetail) {
		return nullptr;
	}
	auto key = GET_INT(1);
	int key_orig = -1;
	switch (key) {
		case 104:
			key_orig = key;
			key = 915;
			break;
		case 221:
		case 717:
		case 718:
		case 743:
			key_orig = key;
			key = 899;
			break;
		case 675:
			key = 1734;
			break;
		case 676:
			key = 1735;
			break;
		case 677:
			key = 1736;
			break;
		case 1008:
			key = 1292;
			break;
		default:
			if (1964 <= key && key < 2012) {
				key = 1964;
			}
			break;
	}
	auto obj = createWithKey(key);
	if (!obj) return nullptr;
	gd::string filename = ObjectToolbox::sharedState()->intKeyToFrame(key);
	// android caseworks 2,3 and weaves a bunch of the branching
	auto x = GET_FLOAT(2);
	float y = GET_DOUBLE(3) + 90.;
	if (isnan(x) != 0) x = 0.f;
	if (isnan(y) != 0) y = 0.f;
	auto flipX = GET_BOOL(4);
	auto flipY = GET_BOOL(5);
	auto rotation = GET_FLOAT(6);
	auto editorLayer = GET_SHORT(20);
	auto editorLayer2 = GET_SHORT(61);
	obj->m_objectID = key;
	if (key == 9 || key == 1715) {
		obj->m_defaultZOrder = 2;
	}
	obj->setCustomZLayer(GET_INT(24));
	// android swaps the below two
	obj->m_zOrder = GET_INT(25);
	obj->m_isHighDetail = isHighDetail;
	obj->addToGroup(GET_INT(26));
	obj->addToGroup(GET_INT(33));
	obj->m_linkedGroup = GET_INT(108);
	obj->m_isNoTouch = GET_BOOL(121);
	// windows has a bunch of redundant checks
	obj->m_enterChannel = std::clamp(GET_INT(343), 0, 100);
	obj->m_objectMaterial = GET_SHORT(446);
	obj->loadGroupsFromString(GET_STRING(57));
	obj->m_hasGroupParent = GET_BOOL(34);
	obj->m_hasAreaParent = GET_BOOL(279);
	auto parents = GET_STRING(274);
	// android swaps the two conditions
	if (parents.size() != 0 && game) {
		game->loadGroupParentsFromString(obj, parents);
	} else {
		obj->m_hasGroupParentsString = false;
	}
	auto scaleX = GET_FLOAT(128);
	auto scaleY = GET_FLOAT(129);
	if (scaleX != 0.f) {
		obj->updateCustomScaleX(scaleX);
	}
	if (scaleY != 0.f) {
		obj->updateCustomScaleY(scaleY);
	}
	if (scaleX == 0.f && scaleY == 0.f) {
		auto scale = GET_FLOAT(32);
		if (scale != 0.f) {
			obj->updateCustomScaleX(scale);
			obj->updateCustomScaleY(scale);
		}
	}
	obj->m_startScaleX = obj->m_scaleX;
	obj->m_startScaleY = obj->m_scaleY;
	obj->m_isDontFade = GET_BOOL(64);
	obj->m_isDontEnter = GET_BOOL(67);
	obj->m_hasNoEffects = GET_BOOL(116);
	obj->m_hasNoParticles = GET_BOOL(507);
	obj->m_mainColorKeyIndex = GET_INT(155);
	obj->m_detailColorKeyIndex = GET_INT(156);
	obj->m_isPassable = GET_BOOL(134);
	obj->m_isHide = GET_BOOL(135);
	obj->m_isNonStickX = GET_BOOL(136);
	obj->m_isNonStickY = GET_BOOL(289);
	obj->m_isExtraSticky = GET_BOOL(495);
	obj->m_isDontBoostY = GET_BOOL(496);
	obj->m_isDontBoostX = GET_BOOL(509);
	obj->m_isScaleStick = GET_BOOL(356);
	obj->m_isIceBlock = GET_BOOL(137);
	obj->m_isGripSlope = GET_BOOL(193);
	obj->m_hasNoAudioScale = GET_BOOL(372);
	obj->m_customColorType = GET_SHORT(497);
	obj->m_hasExtendedCollision = GET_BOOL(511);
	obj->customSetup();
	obj->customObjectSetup(strs, games);
	obj->addGlow(filename);
	obj->addColorSprite(filename);
	obj->setupCustomSprites(filename);
	obj->setFlipX(flipX);
	obj->setFlipY(flipY);
	obj->m_startFlipX = flipX;
	// android swaps the below two
	obj->m_startFlipY = flipY;
	obj->m_editorLayer = editorLayer;
	obj->m_editorLayer2 = editorLayer2;
	obj->m_hasNoGlow = GET_BOOL(96);
	// inlined on windows, eliminating the redundant check
	// the windows decomp simplifies the double->float->int conversion to double->int for some reason
	if (!obj->canRotateFree() && static_cast<int>(rotation) % 90 != 0 && !obj->m_isNoTouch) {
		rotation = 0.f;
	}
	obj->setRotation(rotation);
	obj->m_startRotationX = rotation;
	obj->m_startRotationY = rotation;
	auto rotationX = GET_FLOAT(131);
	auto rotationY = GET_FLOAT(132);
	if (rotationX != rotationY && obj->canRotateFree()) {
		obj->setRotationX(rotationX);
		obj->m_startRotationX = obj->getRotationX();
		obj->setRotationY(rotationY);
		obj->m_startRotationY = obj->getRotationY();
	}
	if (key == 142) {
		static_cast<EnhancedGameObject*>(obj)->updateUserCoin();
	}
	obj->setStartPos({x, y});
	obj->getObjectTextureRect();
	if (key_orig != -1) {
		switch (key_orig) {
			case 104:
				static_cast<EffectGameObject*>(obj)->m_usesBlending = true;
				break;
			case 221: obj->m_targetColor = 1; break;
			case 717: obj->m_targetColor = 2; break;
			case 718: obj->m_targetColor = 3; break;
			case 743: obj->m_targetColor = 4; break;
		}
	}
	if (GET_BOOL(41)) {
		// windows: inlined, and note second arg is unused because it's baked in)
		auto hsv = GameToolbox::hsvFromString(GET_STRING(43), "a");
		if (hsv != ccHSVValue{0.f, 1.f, 1.f, false, false}) {
			// android swaps the below two
			obj->m_baseColor->m_usesHSV = true;
			obj->m_baseColor->m_hsv = hsv;
		}
	}
	if (GET_BOOL(42) && obj->m_detailColor) {
		// not inlined! and ditto
		auto hsv = GameToolbox::hsvFromString(GET_STRING(44), "a");
		if (hsv != ccHSVValue{0.f, 1.f, 1.f, false, false}) {
			// ditto
			obj->m_detailColor->m_usesHSV = true;
			obj->m_detailColor->m_hsv = hsv;
		}
	}
	auto oldColorID = GET_INT(19);
	if (oldColorID != 0) {
		int colorID;
		// assigned from a lookup table on android
		switch (oldColorID) {
			case 1: colorID = 1005; break;
			case 2: colorID = 1006; break;
			case 3: colorID =    1; break;
			case 4: colorID =    2; break;
			case 5: colorID = 1007; break;
			case 6: colorID =    3; break;
			case 7: colorID =    4; break;
			case 8: colorID = 1003; break;
			default: goto out;
		}
		// android redundantly takes the max with 0
		(obj->m_detailColor ? obj->m_detailColor : obj->m_baseColor)->m_colorID = colorID;
	} else {
		// the !games[i] case is optimized for.
		// on windows, there's some weird unreachable code that assigns 1011, IDK what that's about
		auto baseColorID = std::clamp(GET_INT(21), 0, 1101);
		if (baseColorID != 0) {
			obj->m_baseColor->m_colorID = baseColorID;
		}
		// ditto
		auto detailColorID = std::clamp(GET_INT(22), 0, 1101);
		if (detailColorID != 0 && obj->m_detailColor) {
			obj->m_detailColor->m_colorID = detailColorID;
		}
	}
	out:;
	obj->saveActiveColors();
	obj->resetRScaleForced();
	return obj;
}

#undef GET_TYPE
#undef GET_INT
#undef GET_BOOL
#undef GET_SHORT
#undef GET_DOUBLE
#undef GET_FLOAT
#undef GET_STRING
