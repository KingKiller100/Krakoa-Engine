include "Dependencies.lua"

workspace "KrakoaEngine"
    architecture "x86_64"
    startproject "Keditor"

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

group "Dependencies"
    include "Krakoa/Vendors/GLFW"
    include "Krakoa/Vendors/GLAD"
    include "Krakoa/Vendors/imgui"
    include "Krakoa/Vendors/YAML"
    include "../kLibrary/"
    include "../kLibrary/Tests"
group ""

include "Krakoa"
include "Keditor"