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
IncludeDir["Glad"] = "TowerDelivery/vendor/Glad/include"
IncludeDir["ImGui"] = "TowerDelivery/vendor/imgui"
IncludeDir["glm"] = "TowerDelivery/vendor/glm"

group "Dependencies"
	include "TowerDelivery/vendor/GLFW"
	include "TowerDelivery/vendor/Glad"
	include "TowerDelivery/vendor/imgui"
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
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/bullet",
		"%{prj.name}/vendor/assimp",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	libdirs {
	"%{prj.name}/vendor/bulletLibs",
	"%{prj.name}/vendor/assimpLibs"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
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
			"assimp-vc142-mtd.lib",
			"IrrXMLd.lib",
			"zlibd.lib",
			"zlibstaticd.lib"
		}

	filter "configurations:Release"
		defines "TD_RELEASE"
		runtime "Release"
		optimize "on"

		links {
			"BulletCollision_vs2010_x64_release.lib",
			"BulletDynamics_vs2010_x64_release.lib",
			"LinearMath_vs2010_x64_release.lib",
			"assimp-vc142-mt.lib",
			"IrrXML.lib",
			"zlib.lib",
			"zlibstatic.lib"
		}

	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"

		links {
			"BulletCollision_vs2010_x64_release.lib",
			"BulletDynamics_vs2010_x64_release.lib",
			"LinearMath_vs2010_x64_release.lib",
			"assimp-vc142-mt.lib",
			"IrrXML.lib",
			"zlib.lib",
			"zlibstatic.lib"
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
		"TowerDelivery/vendor/bullet",
		"TowerDelivery/vendor/assimp",
		"TowerDelivery/vendor/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}"
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

	filter "configurations:Dist"
		defines "TD_DIST"
		runtime "Release"
		optimize "on"
