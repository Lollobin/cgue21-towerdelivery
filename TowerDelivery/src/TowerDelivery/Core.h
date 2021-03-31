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