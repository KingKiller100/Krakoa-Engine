project "kLibrary"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("MBCS")
	staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "Source Files/pch.cpp"

    files
    {
        "Source Files/**.hpp",
        "Source Files/**.cpp",
        "cpp.hint"
    }

    prebuildcommands
    {
        ("IF EXIST \"$(ProjectDir)Include Files\\\" ( del /f /s /q \"$(ProjectDir)Include Files\\\" ) ELSE (mkdir \"Include Files\")"),
    }

    postbuildcommands
    {
        ("xcopy /s \"$(ProjectDir)Source Files\" \"$(ProjectDir)Include Files\""),
        ("FOR /R \"$(ProjectDir)Include Files\" %%f in (**.cpp) do (del /f /s /q %%f \"$(ProjectDir)Include Files\")"),
        ("del \"$(ProjectDir)Include Files\\pch.hpp\" /s /f /q"),
        ("del \"$(ProjectDir)Include Files\\pch.cpp\" /s /f /q")
    }

    filter "system:Windows"
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
        