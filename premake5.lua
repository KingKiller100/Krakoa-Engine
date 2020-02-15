workspace "Krakoa"
    architecture "x64"

    configurations
    {
        "Debug",
        "Test",
        "Profile",
        "Release"
    }

outputDir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

project "kLib"
    location "kLib"
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
        "%{prj.name}/Source Files",
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "10.0.17763.0"

        defines
        {
            "KLIB_LIB",
            "KLIB_WINDOWS_OS",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
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




project "Krakoa"
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
        "Krakoa"
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
            ("{COPY} ../bin/" .. outputDir .. "/Krakoa.dll %{cfg.buildtarget.relpath}")
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