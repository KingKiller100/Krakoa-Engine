workspace "KrakoaEngine"
    architecture "x64"
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
    
include "Krakoa/Vendors/GLFW/"


project "kLibrary"
    location "kLibrary"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    toolset "v141"
    characterset ("MBCS")

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "kLibrary/Source Files/pch.cpp"

    files
    {
        "%{prj.name}/Source Files/**.h",
        "%{prj.name}/Source Files/**.hpp",
        "%{prj.name}/Source Files/**.cpp",
        "%{prj.name}/cpp.hint"
    }

    includedirs
    {
        "%{prj.name}/Source Files",
        "%{IncludeDir.GLFW}"
    }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "KLIB_LIB",
            "KLIB_WINDOWS_OS",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
        }

    filter "configurations:Debug"
        defines "KLIB_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        defines "KLIB_TEST"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "KLIB_RELEASE"
        optimize "Full"
        runtime "Release"

    filter "configurations:Profile"
        defines "KLIB_PROFILE"
        optimize "Debug"
        runtime "Release"



group "Dependencies"
    include "Krakoa/Vendors/GLFW"
group ""


project "Krakoa"
    location "Krakoa"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    toolset "v141"
    characterset ("MBCS")

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "Precompile.hpp"
    pchsource "Krakoa/Source Files/Precompile.cpp"


    files
    {
        "%{prj.name}/Source Files/**.h",
        "%{prj.name}/Source Files/**.hpp",
        "%{prj.name}/Source Files/**.cpp",
        "%{prj.name}/cpp.hint"
    }

    includedirs
    {
        "kLibrary/Source Files",
        "%{prj.name}/Source Files",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "kLibrary",
        "GLFW"
    }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "KRAKOA_OS_WINDOWS",
            "KRAKOA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "KRAKOA_DEBUG"
        symbols "On"

    filter "configurations:Test"
        defines "KRAKOA_TEST"
        symbols "On"

    filter "configurations:Release"
        defines "KRAKOA_RELEASE"
        optimize "On"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Debug"

        --filters { "system:windows", "configuration:Release" }

project "Hooper2"
    location "ExampleGames/Hooper2/"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    toolset "v141"
    characterset ("MBCS")

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")
    
    files
    {
        "ExampleGames/%{prj.name}/Source Files/**.h",
        "ExampleGames/%{prj.name}/Source Files/**.hpp",
        "ExampleGames/%{prj.name}/Source Files/**.cpp",
    }

    includedirs
    {
        "kLibrary/Source Files",
        "Krakoa/Source Files",
        "Krakoa/Vendors/"
    }


    links
    {
        "Krakoa"
    }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        
        defines
        {
            "KRAKOA_OS_WINDOWS"
        }

        prebuildcommands
        {
            ("IF EXIST \"$(SolutionDir)bin\\" .. OutputDir .. "Hooper2\\Krakoa.dll\" ( del \"$(SolutionDir)bin\\" .. OutputDir .. "Hooper2\\Krakoa.dll\" /f /q)")
        }

        postbuildcommands
        {
            ("xcopy /y \"$(SolutionDir)bin\\" .. OutputDir .. "Krakoa\\Krakoa.dll\" \"$(OutDir)\" /q")
        }

    filter "configurations:Debug"
        defines "KRAKOA_DEBUG"
        symbols "On"

    filter "configurations:Test"
        defines "KRAKOA_TEST"
        symbols "On"

    filter "configurations:Release"
        defines "KRAKOA_RELEASE"
        optimize "On"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Debug"