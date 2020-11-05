workspace "Prism"
	architecture "x64"
	startproject "PrismEditor"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Prism/vendor/GLFW/include"
IncludeDir["Glad"] = "Prism/vendor/Glad/include"
IncludeDir["ImGui"] = "Prism/vendor/imgui"
IncludeDir["glm"] = "Prism/vendor/glm"
IncludeDir["stb_image"] = "Prism/vendor/stb_image"
IncludeDir["entt"] = "Prism/vendor/entt/include"

group "Dependencies"
	include "Prism/vendor/GLFW"
	include "Prism/vendor/Glad"
	include "Prism/vendor/imgui"

group ""

project "Prism"
	location "Prism"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "PrismPrecompiledHeader.h"
	pchsource "Prism/src/PrismPrecompiledHeader.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PRISM_PLATFORM_WINDOWS",
			"PRISM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "PRISM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PRISM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "PRISM_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Prism/src",
		"Prism/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Prism"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PRISM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PRISM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PRISM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "PRISM_DIST"
		runtime "Release"
		optimize "on"


project "PrismEditor"
	location "PrismEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Prism/src",
		"Prism/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Prism"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PRISM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "PRISM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PRISM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "PRISM_DIST"
		runtime "Release"
		optimize "on"