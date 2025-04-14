workspace "MooMooEngine"
	architecture "x64"
	startproject "MooMooEngine"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root folider (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MooMooEngine/Middleware/GLFW/include"
IncludeDir["spdlog"] = "MooMooEngine/Middleware/spdlog/include"
IncludeDir["Glad"] = "MooMooEngine/Middleware/Glad/include"
IncludeDir["ImGui"] = "MooMooEngine/Middleware/imgui"
IncludeDir["glm"] = "MooMooEngine/Middleware/glm"
IncludeDir["stb_image"] = "MooMooEngine/Middleware/stb_image"

IncludeDir["assimp"] = "MooMooEngine/Middleware/assimp"
IncludeDir["entt"] = "MooMooEngine/Middleware/EnTT/include"
IncludeDir["jsoncpp"] = "MooMooEngine/Middleware/jsoncpp"
IncludeDir["PhysX"] = "MooMooEngine/Middleware/PhysX"
IncludeDir["Enet"] = "MooMooEngine/Middleware/Enet"
IncludeDir["FMOD"] = "MooMooEngine/Middleware/FMOD"


--includes the premake5.lua file for middleware, effectively bringing it into this premake file
include "MooMooEngine/Middleware/GLFW"
include "MooMooEngine/Middleware/Glad"
include "MooMooEngine/Middleware/imgui"

project "MooMooEngine"
	location "MooMooEngine"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "MooMooEngine_PCH.h"
	pchsource "%{prj.name}/src/MooMooEngine_PCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Middleware/stb_image/**.h",
		"%{prj.name}/Middleware/stb_image/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{	
		"%{prj.name}/src/",
		"%{prj.name}/Middleware",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}/include",
		"%{IncludeDir.entt}",
		"%{IncludeDir.jsoncpp}/include",
		"%{IncludeDir.PhysX}/include",
		"%{IncludeDir.Enet}/include",
		"%{IncludeDir.FMOD}/inc",

	}
	libdirs
	{
		
		"%{IncludeDir.assimp}/bin/windows/%{cfg.buildcfg}/",
		"%{IncludeDir.assimp}/lib/windows/%{cfg.buildcfg}/",
		"%{IncludeDir.jsoncpp}/lib",
		"%{IncludeDir.PhysX}/lib",
		"%{IncludeDir.Enet}/lib",
		"%{IncludeDir.FMOD}/lib",	
		
	}


	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"assimp-vc142-mtd",
		"jsoncpp.lib",
		"PhysX_64.lib",
		"PhysXCommon_64.lib",
		"PhysXCooking_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXFoundation_64.lib",
		--"enet.lib",
		"enet64.lib",
		"Ws2_32.lib",  -- Add this line
        "Winmm.lib",  -- Add this line
		"fmodL_vc.lib", 
		"fmod_vc.lib",   
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
		--this is required to avoid errors with the OpenGL header already being included
		--being displayed in visual studio.
		--this explicitly prevents the GLFW header from including the OpenGL header
			"GLFW_INCLUDE_NONE"
		}
		postbuildcommands {  --if the json and physX don't run then I need to do this
		'{COPY} "../MooMooEngine/Middleware/assimp/bin/windows/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/jsoncpp.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/PhysX_64.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/PhysXCommon_64.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/PhysXCooking.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/PhysXFoundation_64.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/fmod.dll" "%{cfg.targetdir}"',
		'{COPY} "../MooMooEngine/Middleware/dlls/fmodL.dll" "%{cfg.targetdir}"',

		}

	filter "configurations:Debug"
		defines { "_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG"}
		optimize "On"
postbuildcommands {  --if the json and physX dont runn then i need to do this

	}

