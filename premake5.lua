workspace "Krakoa"
    architecture "x64"

    configurations
    {
        "Debug",
        "Test",
        "Profile",
        "Release"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "KrakoaEngine"
    location "Krakoa"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.hpp",
        "%{prj.name}/source/**.cpp"
    }

    includedirs
    {
        "kLibrary/Source Files",
        "%{prj.name}/Source Files",
        "%{prj.name}/Vendors/GLFW/include"
    }

    links
    {
        "kLibrary"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "10.0.17763.0"

        defines
        {
            "KRAKOA_OS_WINDOWS",
            "KRAKOA_BUILD_DLL"
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

    filter "configurations:PROFILE"
        defines "KRAKOA_PROFILE"
        optimize "On"

        --filters { "system:windows", "configuration:Release" }

project "Hooper2"
    location "ExampleGames"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.hpp",
        "%{prj.name}/source/**.cpp"
    }

    includedirs
    {
        "kLibrary/Source Files",
        "Krakoa/Source Files",
        "%{prj.name}/Vendors/GLFW/include"
    }


    links
    {
        "KrakoaEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "10.0.17763.0"

        defines
        {
            "KRAKOA_OS_WINDOWS"
        }

        postbuildcommands
        {
            ("{COPY} ../bin/" .. outputDir .. "/KrakoaEngine.dll %{cfg.buildtarget.relpath}")
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

    filter "configurations:PROFILE"
        defines "KRAKOA_PROFILE"
        optimize "On"