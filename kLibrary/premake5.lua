project "kLibrary"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    toolset "v142"
    characterset ("MBCS")

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "Source Files/pch.cpp"

    files
    {
      --  "Source Files/**.h",
        "Source Files/**.hpp",
        "Source Files/**.cpp",
        "cpp.hint"
    }

    includedirs
    {
        "Source Files",
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
        removefiles
        {
            "Source Files/**/*Test*"
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        defines "KLIB_TEST"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "KLIB_RELEASE"
        removefiles
        {
            "Source Files/**/*Test*"
        }
        optimize "Full"
        runtime "Release"

    filter "configurations:Profile"
        defines "KLIB_PROFILE"
        removefiles
        {
            "Source Files/**/*Test*"
        }
        optimize "Debug"
        runtime "Release"
        