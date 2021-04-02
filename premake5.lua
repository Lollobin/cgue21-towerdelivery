workspace "TowerDelivery"
	architecture "x64"
	startproject "Game"

	configurations{
	"Debug",
	"Release",
	"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TowerDelivery/vendor/GLFW/include"

include "TowerDelivery/vendor/GLFW"

project "TowerDelivery"
	location "TowerDelivery"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tdpch.h"
	pchsource "TowerDelivery/src/tdpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"TD_PLATFORM_WINDOWS",
			"TD_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
		}
	
	filter "configurations:Debug"
		defines "TD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TD_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "TD_DIST"
		optimize "On"

project	"Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tdpch.h"
	pchsource "Game/src/tdpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"TowerDelivery/vendor/spdlog/include",
		"TowerDelivery/src"
	}

	links{
		"TowerDelivery"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"TD_PLATFORM_WINDOWS",
		}
	
	filter "configurations:Debug"
		defines "TD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TD_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "TD_DIST"
		optimize "On"