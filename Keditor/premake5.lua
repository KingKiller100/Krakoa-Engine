project "Keditor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
    staticruntime "On"
    locale "en-GB"

    targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")
    
    files
    {
        "Source/**.hpp",
        "Source/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/Krakoa/Source",
        "%{wks.location}/Krakoa/Vendors/",
        "%{IncludeDir.KLIB}",
        "%{IncludeDir.IMGUIZMO}",
    }

    links
    {
        "kLibrary",
        "Krakoa"
    }

    defines
    {
        "KLIB_LIB",
        "KLIB_SHORT_NAMESPACE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:Windows"
        systemversion "latest"
        
        defines
        {
            "KLIB_WINDOWS_OS",
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
        -- targetdir ("bin/" .. OutputDir ..  "/Component Tests")
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
        
