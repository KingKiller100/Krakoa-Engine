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
    
include "Krakoa/Vendors/GLFW/"

group "Dependencies"
    include "Krakoa/Vendors/GLFW"
group ""

include "kLibrary/"

project "Krakoa"
    location "Krakoa"
    kind "SharedLib"
    language "C++"
    cppdialect "C++latest"
    toolset "v142"
    characterset ("MBCS")

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "Precompile.hpp"
    pchsource "Krakoa/Source Files/Precompile.cpp"


    files
    {
        -- "%{prj.name}/Source Files/**.h",
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
        "GLFW",
        "opengl32.lib"
    }

    prebuildcommands
    {
        ("IF EXIST \"$(SolutionDir)bin\\" .. OutputDir .. "Hooper2\\Krakoa.dll\" ( del \"$(SolutionDir)bin\\" .. OutputDir .. "Hooper2\\Krakoa.dll\" /f /q)")
    }

    postbuildcommands
    {
        --("xcopy /y \"$(SolutionDir)bin\\" .. OutputDir .. "Krakoa\\Krakoa.dll\" \"$(OutDir)\" /q")
        ("xcopy /y \"$(SolutionDir)bin\\" .. OutputDir .. "Krakoa\\Krakoa.dll\" \"$(SolutionDir)bin\\" .. OutputDir .. "Hooper2\\\" /q")
    }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "KRAKOA_OS_WINDOWS",
            "KRAKOA_BUILD_DLL",
			--"GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "KRAKOA_DEBUG"
        removefiles
        {
            "%{prj.name}/Source Files/**/test*"
        }
        buildoptions "/MDd"
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        defines 
        {
            "KRAKOA_TEST",
            "KLIB_TEST"
        }
        symbols "On"
        buildoptions "/MDd"
        runtime "Debug"

    filter "configurations:Release"
        defines "KRAKOA_RELEASE"
        optimize "Full"
        removefiles
        {
            "%{prj.name}/Source Files/**/test*"
        }
        buildoptions "/MD"
        runtime "Release"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Debug"
        removefiles
        {
            "%{prj.name}/Source Files/**/test*"
        }
        buildoptions "/MD"
        runtime "Release"

        --filters { "system:windows", "configuration:Release" }

project "Hooper2"
    location "ExampleGames/Hooper2/"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    toolset "v142"
    characterset ("MBCS")

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")
    
    files
    {
        -- "ExampleGames/%{prj.name}/Source Files/**.h",
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

    filter "configurations:Debug"
        defines "KRAKOA_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
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
        optimize "Debug"
        runtime "Release"
