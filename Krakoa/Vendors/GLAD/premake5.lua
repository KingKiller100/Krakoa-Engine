project "GLAD"
	kind "StaticLib"
	language "C"
    characterset ("MBCS")

	targetdir ("../../../bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../../bin-int/" .. OutputDir .. "/%{prj.name}")

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
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
        buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
        buildoptions "/MD"
		optimize "Full"
		
