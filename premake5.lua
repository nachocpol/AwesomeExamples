workspace "awesome_examples"
	configurations { "Debug", "Release"}
	platforms "x64"
	staticruntime "on"
	language "C++"	
	targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"

filter { "platforms:x64" }
	defines { "PLATFORM_WINDOWS" }
	disablewarnings 
	{
		"4006",
		"4221"
	}
	
filter {"configurations:Debug or Release"}	
	libdirs
	{
		"depen/AwesomeEngine/Build/%{cfg.platform}/%{cfg.buildcfg}",
		"depen/AwesomeEngine/Depen/dxc/lib/x64"
	}
	links
	{
		"Core",
		"Graphics",
		"dxcompiler",
		"dxil"
	}
	includedirs 
	{
		"depen",
		"depen/AwesomeEngine/Depen/GLM",
		"depen/AwesomeEngine/Source",
		"src/shaders"
	}

filter {"configurations:Debug"}
	defines
	{
		"CFG_DEBUG"
	}
	symbols "On"

filter {"configurations:Release"}
	defines
	{
		"CFG_RELEASE"
	}
	optimize "On"

project "triangle"
	location "temp/vs_files"
	kind "WindowedApp"	
	files
	{
		"src/TriangleSample.cpp"
	}

project "model_viewer"
	location "temp/vs_files"
	kind "WindowedApp"
	files
	{
		"src/ModelViewer.cpp"
	}
