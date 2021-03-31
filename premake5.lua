workspace "TowerDelivery"
	architecture "x64"

	configurations{
	"Debug",
	"Release",
	"Dist"
	}

outputDirectory = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "TowerDelivery"
	location "TowerDelivery"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"TD_PLATFORM_WINDOWS",
			"TD_BUILD_DLL"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin" .. outputDirectory .. "/Game")
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

	targetdir ("bin/" .. outputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDirectory .. "/%{prj.name}")

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