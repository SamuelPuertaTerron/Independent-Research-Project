workspace("IRP")
	configurations { "Debug", "Release" }
	architecture("x64")

  flags
	{
		"MultiProcessorCompile"
	}

  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  IncludeDir = {}
  IncludeDir["ENGINE"] =     "Engine/src/"
  IncludeDir["THIRDPARTY"] =     "ThirdParty/"

  group "Dependencies"
    include "ThirdParty/fastgltf"
    include "ThirdParty/FastNoise"
    include "ThirdParty/glad"
    include "ThirdParty/glm"
    include "ThirdParty/ImGui"
    include "ThirdParty/nlohmann"
    include "ThirdParty/miniaudio"
    include "ThirdParty/stb_image"
  group ""
  
  group "Engine"
    include "Engine"
  group ""

  group "Games"
    include "Games/EntityGame"
    include "Games/GameObjectGame"
  group ""
	
	filter "Debug"
		symbols "on"

  filter "Release"
		optimize "on"
    symbols "off"

    include "Engine"
    include "Games/EntityGame"
    include "Games/GameObjectGame"
