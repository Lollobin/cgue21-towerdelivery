workspace "TowerDelivery"
	architecture "x64"
	startproject "Game"

	configurations{
	"Debug",
	"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TowerDelivery/vendor/GLFW/include"
IncludeDir["Glad"] = "TowerDelivery/vendor/Glad/include"
IncludeDir["glm"] = "TowerDelivery/vendor/glm"
IncludeDir["stb_image"] = "TowerDelivery/vendor/stb_image"
IncludeDir["inih"] = "TowerDelivery/vendor/inih/include"
IncludeDir["spdlog"] = "TowerDelivery/vendor/spdlog/include"
IncludeDir["bullet"] = "TowerDelivery/vendor/bullet"
IncludeDir["assimp"] = "TowerDelivery/vendor/assimp/include"
IncludeDir["freeType"] = "TowerDelivery/vendor/freeType/include"


group "Dependencies"
	include "TowerDelivery/vendor/GLFW"
	include "TowerDelivery/vendor/Glad"
	include "TowerDelivery/vendor/assimp"
	include "TowerDelivery/vendor/inih"
group ""

project "TowerDelivery"
	location "TowerDelivery"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tdpch.h"
	pchsource "TowerDelivery/src/tdpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.inih}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.bullet}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.freeType}"
	}

	libdirs {
	"%{prj.name}/vendor/bulletLibs",
	"%{prj.name}/vendor/freeType/lib"
	}

	links {
		"GLFW",
		"Glad",
		"inih",
		"opengl32.lib",
		"assimp"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"TD_PLATFORM_WINDOWS",
			"TD_BUILD_DLL"
		}
	
	filter "configurations:Debug"
		defines "TD_DEBUG"
		runtime "Debug"
		symbols "on"

		links {
			"BulletCollision_vs2010_x64_debug.lib",
			"BulletDynamics_vs2010_x64_debug.lib",
			"LinearMath_vs2010_x64_debug.lib",
			"freetype.lib"
		}

	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"

		links {
			"BulletCollision_vs2010_x64_release.lib",
			"BulletDynamics_vs2010_x64_release.lib",
			"LinearMath_vs2010_x64_release.lib",
			"freetype.lib"
		}


project	"Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"TowerDelivery/src",
		"TowerDelivery/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.inih}",
		"%{IncludeDir.bullet}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.freeType}",
	}

	links {
		"TowerDelivery"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"TD_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "TD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"