workspace "contacts-cli"
    configurations { "Debug", "Release" }

project "contacts"
    kind "ConsoleApp"
    language "C"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.c", "src/**.h"}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "On"