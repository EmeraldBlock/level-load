#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

template<typename T>
static T gd_cast(char const* str) = delete;

#define CAST(t, e) \
	template<> \
	t gd_cast<t>(char const* str) { return e; }

CAST(int, atoi(str))
CAST(short, atoi(str))
CAST(bool, atoi(str))
CAST(double, atof(str))
CAST(float, atof(str))
CAST(gd::string, str)

#undef CAST

#define PROPS_CACHE \
	M(int, 1) \
	M(float, 2) \
	M(double, 3) \
	M(int, 24) \
	M(int, 25) \
	M(bool, 103) \
	M(int, 26) \
	M(int, 33) \
	M(int, 108) \
	M(int, 121) \
	M(int, 343) \
	M(short, 446) \
	M(gd::string, 57) \
	M(bool, 34) \
	M(bool, 279) \
	M(gd::string, 274) \
	M(float, 128) \
	M(float, 129) \
	M(float, 32) \
	M(bool, 64) \
	M(bool, 67) \
	M(bool, 116) \
	M(bool, 507) \
	M(int, 155) \
	M(int, 156) \
	M(bool, 134) \
	M(bool, 135) \
	M(bool, 136) \
	M(bool, 289) \
	M(bool, 495) \
	M(bool, 496) \
	M(bool, 509) \
	M(bool, 356) \
	M(bool, 137) \
	M(bool, 193) \
	M(bool, 372) \
	M(short, 497) \
	M(bool, 511) \
	M(bool, 4) \
	M(bool, 5) \
	M(short, 20) \
	M(short, 61) \
	M(bool, 96) \
	M(double, 6) \
	M(float, 131) \
	M(float, 132) \
	M(bool, 41) \
	M(gd::string, 43) \
	M(bool, 42) \
	M(gd::string, 44) \
	M(int, 19) \
	M(int, 21) \
	M(int, 22)

struct PropsCache {
	void cacheProp(int k, char const* str) {
	    switch (k) {
            #define M(t, i) case i: m_##i = gd_cast<t>(str); break;
            PROPS_CACHE
            #undef M
        }
    }

    #define M(t, i) t m_##i{};
	PROPS_CACHE
    #undef M
};

#undef PROPS_CACHE
