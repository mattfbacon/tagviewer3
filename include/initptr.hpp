#pragma once

#define INIT_PTR(MEMBER, ...) MEMBER(new std::remove_pointer_t<decltype(MEMBER)>{ __VA_ARGS__ })
#define INIT_MEMBER(MEMBER, ...) \
	MEMBER { \
		__VA_ARGS__ \
	}
