project "GLAD"
	kind "StaticLib"
	language "C"
    characterset ("default")
    -- characterset ("MBCS")
	staticruntime "on"

	targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"include/glad/**.h",
		"include/KHR/**.h",
		"src/**.c"
	}
	
	includedirs
	{
		"include",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Test"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Profile"
		runtime "Release"
		optimize "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "Full"
		
