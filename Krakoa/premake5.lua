project "Krakoa"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
	staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "Precompile.hpp"
    pchsource "Source/Precompile.cpp"

    files
    {
        "Source/**.hpp",
        "Source/**.cpp",
        "Tests/**.hpp",
        "Tests/**.cpp",
        "cpp.hint",
        "Vendors/STB Image/**.hpp",
        "Vendors/STB Image/**.cpp",
        "Vendors/ImGuizmo/ImGuizmo.h",
        "Vendors/ImGuizmo/ImGuizmo.cpp",
    }

    includedirs
    {
        "%{IncludeDir.KLIB}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.STBI}",
        "%{IncludeDir.YAML}",
        "%{IncludeDir.IMGUIZMO}",
    }

    defines
	{
        "KLIB_SHORT_NAMESPACE",
        "_CRT_SECURE_NO_WARNINGS"
	}

    links
    {
        "kLibrary",
        "GLFW",
        "GLAD",
        "ImGui",
        "YAML",
        "opengl32.lib",
    }

	filter "files:Vendors/ImGuizmo/**.cpp"
	flags { "NoPCH" }

    filter "system:Windows"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "KRAKOA_OS_WINDOWS",
            -- "KRAKOA_BUILD_DLL",
            "KLIB_LIB",
            "KLIB_SHORT_NAMESPACE",
			"GLFW_INCLUDE_NONE",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
        }
        
        filter "configurations:Debug"
        defines 
        {
            "KRK_ENABLE_ASSERT",
            "KRAKOA_DEBUG",
            "KLIB_DEBUG",
            "KLOG_OPT_DBG_STR",
        }
        removefiles
        {
            "%{prj.name}/Source/**/*test*"
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
        defines 
        {
            "KRAKOA_RELEASE",
        }

        optimize "Full"
        removefiles
        {
            "%{prj.name}/Source/**/*test*",
            "%{prj.name}/Tests/**"
        }
        runtime "Release"

    filter "configurations:Profile"
        defines "KRAKOA_PROFILE"
        optimize "Speed"
        removefiles
        {
            "%{prj.name}/Source/**/*test*",
            "%{prj.name}/Tests/**"
        }
        runtime "Release"