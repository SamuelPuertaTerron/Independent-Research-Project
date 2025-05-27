project "GameObjectGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Game.h"
    pchsource "src/Game.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"../../%{IncludeDir.ENGINE}",
        "../../%{IncludeDir.THIRDPARTY}",
	}

	links
	{
		"Engine"	
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"E_PLATFORM_WINDOWS",
		}

		postbuildcommands 
		{
			"{COPYDIR} %[Resources] %[%{cfg.targetdir}/Resources]"
		}

	filter "configurations:Debug"
        defines
        {
            "E_DEBUG"
        }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
        defines
        {
            "E_RELEASE"
        }
		kind "WindowedApp"
		runtime "Release"
        optimize "on"
		symbols "off"