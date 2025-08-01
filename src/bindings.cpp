#include <Geode/Geode.hpp>

using namespace geode::prelude;

// copied from `GeneratedSource.cpp`

static auto wrapFunction(uintptr_t address, tulip::hook::WrapperMetadata const& metadata) {
	auto wrapped = geode::hook::createWrapper(reinterpret_cast<void*>(address), metadata);
	if (wrapped.isErr()) {{
		throw std::runtime_error(wrapped.unwrapErr());
	}}
	return wrapped.unwrap();
}

#ifdef GEODE_IS_WINDOWS

auto GJBaseGameLayer::loadGroupParentsFromString(GameObject* p0, gd::string p1) -> decltype(loadGroupParentsFromString(p0, p1)) {
	using FunctionType = decltype(loadGroupParentsFromString(p0, p1))(*)(GJBaseGameLayer*, GameObject*, gd::string);
	static auto func = wrapFunction(base::get() + 0x21f4b0, tulip::hook::WrapperMetadata{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::Thiscall),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	});
	return reinterpret_cast<FunctionType>(func)(this, p0, p1);
}

auto GJBaseGameLayer::moveObjectToStaticGroup(GameObject* p0) -> decltype(moveObjectToStaticGroup(p0)) {
	using FunctionType = decltype(moveObjectToStaticGroup(p0))(*)(GJBaseGameLayer*, GameObject*);
	static auto func = wrapFunction(base::get() + 0x22c060, tulip::hook::WrapperMetadata{
		.m_convention = geode::hook::createConvention(tulip::hook::TulipConvention::Thiscall),
		.m_abstract = tulip::hook::AbstractFunction::from(FunctionType(nullptr)),
	});
	return reinterpret_cast<FunctionType>(func)(this, p0);
}

#endif
