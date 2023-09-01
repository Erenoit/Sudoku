workspace "Sudoku"
  toolset "clang"
  debugger "gdb"
  cppdialect "C++20"
  cdialect "C17"
  configurations { "Debug", "OptimizedDebug", "Release" }
  platforms { "Linux", "Windows" }
  architecture "x86_64"

project "Sudoku"
  kind "WindowedApp"
  language "C++"
  targetdir "bin/%{cfg.buildcfg}"
  buildoptions { "-Wall", "-Wextra", "-Wpedantic" }

  externalincludedirs { "lib/glad/include", "lib/glm", "/usr/include/freetype2" }

  links { "glfw", "freetype" }

  files { "src/**.hpp", "src/**.cpp", "lib/glad/src/glad.c" }

  filter "platforms:Linux"
      libdirs { "/usr/lib" }

  filter "platforms:Windows"
      libdirs { "" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:OptimizedDebug"
    defines { "DEBUG" }
    optimize "On"
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
