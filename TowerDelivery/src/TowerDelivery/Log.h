#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace TowerDelivery {
	class TOWERDELIVERY_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

//Core log macros
#define TD_CORE_TRACE(...)   ::TowerDelivery::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TD_CORE_INFO(...)    ::TowerDelivery::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TD_CORE_WARN(...)    ::TowerDelivery::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TD_CORE_ERROR(...)   ::TowerDelivery::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TD_CORE_FATAL(...)   ::TowerDelivery::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define TD_TRACE(...)   ::TowerDelivery::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TD_INFO(...)    ::TowerDelivery::Log::GetClientLogger()->info(__VA_ARGS__)
#define TD_WARN(...)    ::TowerDelivery::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TD_ERROR(...)   ::TowerDelivery::Log::GetClientLogger()->error(__VA_ARGS__)
#define TD_FATAL(...)   ::TowerDelivery::Log::GetClientLogger()->fatal(__VA_ARGS__)