project "MINIAUDIO"
kind "StaticLib"
language "C"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
    "miniaudio/**.h",
    "miniaudio/**.c",
}

includedirs
{
    "miniaudio/",
}

filter "system:linux"
    pic "On"

    systemversion "latest"
    staticruntime "On"

filter "system:windows"
    systemversion "latest"
    staticruntime "On"
    
    -- buildoptions{
    --     "/MT"
    -- }
filter "configurations:Debug"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    runtime "Release"
    optimize "On"
    symbols "off"