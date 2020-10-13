workspace "Prism"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include Directories Relative To Root Folder (Solution Directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Prism/vendor/GLFW/include"
IncludeDir["Glad"] = "Prism/vendor/Glad/include"
IncludeDir["ImGui"] = "Prism/vendor/imgui"

include "Prism/vendor/GLFW"
include "Prism/vendor/Glad"
include "Prism/vendor/imgui"

startproject = "Sandbox"

project "Prism"
	location "Prism"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "PrismPrecompiledHeader.h"
	pchsource "Prism/src/PrismPrecompiledHeader.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PRISM_PLATFORM_WINDOWS",
			"PRISM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PRISM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PRISM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "PRISM_DISTRIBUTION"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Prism/vendor/spdlog/include",
		"Prism/src"
	}

	links
	{
		"Prism"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PRISM_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "PRISM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PRISM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "PRISM_DISTRIBUTION"
		runtime "Release"
		optimize "On"