CUDA_SDK_DIR = os.getenv("CUDA_PATH")
LibTorchDir = os.getenv("LIBTORCH_DIR")
CprDir = os.getenv("CPR_DIR")

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
      "vendor/tinyobjloader",
      "lib/websocketpp",
      "lib/asio/include",

      
      "../Walnut/vendor/imgui",
      "../Walnut/vendor/glfw/include",
      "../Walnut/vendor/glm",

      "../Walnut/Walnut/src",

      "%{IncludeDir.VulkanSDK}",
      
      "%{LibTorchDir}/include",
      "%{LibTorchDir}/include/torch/csrc/api/include",
      
      "%{CUDA_SDK_DIR}/include", -- CUDA include path
      "%{CprDir}/include"

   }

   links
   {
       "Walnut"
   }

   libdirs 
   { 
      "%{LibTorchDir}/lib",
      "%{CUDA_SDK_DIR}/lib/x64", -- CUDA library path for 64-bit systems
      "%{CprDir}/lib"

   }

   links 
   {
      "cudart", -- CUDA runtime library
      "cublas", -- CUDA Basic Linear Algebra Subroutines (if needed)
      "c10",
      "torch_cpu",
      "torch_cuda",
      "cpr"
   }

   defines { "_WEBSOCKETPP_CPP11_STL_", "ASIO_STANDALONE" }

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
