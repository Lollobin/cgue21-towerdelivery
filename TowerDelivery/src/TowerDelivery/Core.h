#pragma once

#ifdef TD_PLATFORM_WINDOWS
	#ifdef TD_BUILD_DLL
		#define TOWERDELIVERY_API __declspec(dllexport)
	#else
		#define TOWERDELIVERY_API __declspec(dllimport)
	#endif
#else
	#error Tower Delivery only suppports Windows :)
#endif

#ifdef TD_DEBUG
	#define TD_ENABLE_ASSERTS
#endif

#ifdef TD_ENABLE_ASSERTS
	#define TD_ASSERT(x, ...) {if(!(x)) {TD_ERROR("Assertion Failed: {0}", __VA_ARGS; __debugbreak();}}
	#define TD_CORE_ASSERT(x, ...) {if(!(x)) {TD_ERROR("Assertion Failed: {0}", __VA_ARGS; __debugbreak();}}
#else
	#define TD_ASSERT(x, ...)
	#define TD_CORE_ASSERT(x, ...)
#endif // TD_ENABLE_ASSERTS




#define BIT(x) (1 << x)