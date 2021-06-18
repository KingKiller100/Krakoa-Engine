project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
    characterset ("default")
    -- characterset ("MBCS")
	staticruntime "on"

	targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp"
	}

	defines
	{
		"IMGUI_API=__declspec(dllexport)"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "system:linux"
		pic "On"
		systemversion "latest"
		staticruntime "On"


		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Test"
		runtime "Debug"
		symbols "on"

	filter "configurations:Profile"
		runtime "Release"
        optimize "Speed"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "Full"

