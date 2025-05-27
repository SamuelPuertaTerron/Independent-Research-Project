project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "Engine.h"
    pchsource "src/Engine.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src/",
        "%{IncludeDir.THIRDPARTY}",
    }

    links
    {
        "STBIMAGE",
        "GLM",
        "IMGUI",
        "FASTGLTF",
        "Glad",
        "MINIAUDIO",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "E_PLATFORM_WINDOWS",
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
		runtime "Release"
        optimize "On"
        symbols "off"