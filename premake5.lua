project "AwesomeTriangle"
	kind "WindowedApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/TriangleSample.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Graphics", "Core"
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Graphics", "Core"
		}

project "AwesomeCube"
	kind "WindowedApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/CubeSample.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
		}

project "AwesomeModelViewer"
	kind "WindowedApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Samples/**.h",
		"Source/Samples/ModelViewer.cpp"
	}
	filter "configurations:Debug"
		symbols "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
		}
	filter "configurations:Release"
		optimize "On"
		links
		{
			"Graphics", "Core"
		}
		postbuildcommands 
		{
		}