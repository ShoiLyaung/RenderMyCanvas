LibTorchDir = "G:\\libtorch" -- 替换为实际路径
CUDA_SDK_DIR = "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v12.0" -- 替换为你的CUDA安装路径

project "RenderMyCanvas"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp", "src/**.cuh", "src/**.cu" }

   includedirs
   {
      "src",
      "vendor/entt/single_include",
      
      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",

      "../Walnut/Walnut/src",

      "%{IncludeDir.VulkanSDK}",
      
      "%{LibTorchDir}/include",
      "%{LibTorchDir}/include/torch/csrc/api/include",
      
      "%{CUDA_SDK_DIR}/include" -- CUDA include path
   }

   links
   {
       "Walnut"
   }

   libdirs 
   { 
      "%{LibTorchDir}/lib",
      "%{CUDA_SDK_DIR}/lib/x64" -- CUDA library path for 64-bit systems
   }

   links 
   {
      "cudart", -- CUDA runtime library
      "cublas", -- CUDA Basic Linear Algebra Subroutines (if needed)
      "cudnn" -- CUDA Deep Neural Network library (if needed, optional)
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      defines { "TORCH_USE_CUDA" }
      
   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
