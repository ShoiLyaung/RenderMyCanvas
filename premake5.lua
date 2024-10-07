-- premake5.lua
workspace "RenderMyCanvas"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "RenderMyCanvas"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "RenderMyCanvas"