workspace "KrakoaEngine"
    architecture "x86_64"
    startproject "Hooper2"

    configurations
    {
        "Debug",
        "Test",
        "Profile",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    OutputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}\\"

    -- Include Libraries
    IncludeDir = {}
    IncludeDir["GLFW"] = "Krakoa/Vendors/GLFW/include/"
    IncludeDir["KLIB"] = "kLibrary/Include Files/"
    IncludeDir["GLAD"] = "Krakoa/Vendors/GLAD/include/"
    IncludeDir["ImGUI"] = "Krakoa/Vendors/imgui/"
    IncludeDir["STBI"] = "Krakoa/Vendors/STB Image/"

group "Dependencies"
    include "Krakoa/Vendors/GLFW"
    include "Krakoa/Vendors/GLAD"
    include "Krakoa/Vendors/imgui"
    include "kLibrary/"
group ""

group "Engine" 

project "Krakoa"
    location "Krakoa"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
	staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "Precompile.hpp"
    pchsource "Krakoa/Source Files/Precompile.cpp"

    files
    {
        "%{prj.name}/Source Files/**.hpp",
        "%{prj.name}/Source Files/**.cpp",
        "%{prj.name}/Vendors/STB Image/**.hpp",
        "%{prj.name}/Vendors/STB Image/**.cpp",
        "%{prj.name}/cpp.hint"
    }

    includedirs
    {
        -- "%{IncludeDir.KLIB}",
        "kLibrary/Source Files/",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.STBI}",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    links
    {
        "kLibrary",
        "GLFW",
        "GLAD",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "KRAKOA_OS_WINDOWS",
            "KRAKOA_BUILD_DLL",
            "KLIB_LIB",
			"GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines 
        {
            "KRAKOA_DEBUG",
            "KLIB_DEBUG"
        }
        removefiles
        {
            "%{prj.name}/Source Files/**/*test*",
            "%{prj.name}/Tests/**"
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        defines 
        {
            "KRAKOA_TEST",
            "KLIB_TEST"
        }
        files
        {
            "%{prj.name}/Tests/**",
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "KRAKOA_RELEASE"
        optimize "Full"
        removefiles
        {
            "%{prj.name}/Source Files/**/*test*",
            "%{prj.name}/Tests/**"
        }
        runtime "Release"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Speed"
        removefiles
        {
            "%{prj.name}/Source Files/**/*test*",
            "%{prj.name}/Tests/**"
        }
        runtime "Release"
group ""

group "Games"
project "Hooper2"
    location "ExampleGames/Hooper2/"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
    staticruntime "On"

    targetdir ("bin/".. OutputDir .. "/Game/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")
    
    files
    {
        "ExampleGames/%{prj.name}/Source Files/**.hpp",
        "ExampleGames/%{prj.name}/Source Files/**.cpp",
    }

    includedirs
    {
        "kLibrary/Source Files",
        "Krakoa/Source Files",
        "Krakoa/Vendors/",
    }

    links
    {
        "Krakoa"
    }

    filter "system:Windows"
        systemversion "latest"
        
        defines
        {
            "KRAKOA_OS_WINDOWS"
        }

    filter "configurations:Debug"
        defines 
        {
            "KRAKOA_DEBUG",
            "KLIB_DEBUG"
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        targetdir ("bin/" .. OutputDir ..  "/Component Tests")
        targetname ("Test")
        defines 
        {
            "KRAKOA_TEST",
            "KLIB_TEST"
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "KRAKOA_RELEASE"
        optimize "Full"
        runtime "Release"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Speed"
        runtime "Release"

group ""