CUDA_SDK_DIR = os.getenv("CUDA_PATH")
LibTorchDir = os.getenv("LIBTORCH_DIR")

project "RenderMyCanvas"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files {
      "src/**.h",
      "src/**.cpp",
      "src/**.cuh", "src/**.cu",
      "shaders/**.rgen",
      "shaders/**.rchit",
      "shaders/**.rmiss",
      "shaders/**.rahit",
      "shaders/**.rint",
   }
   includedirs
   {
      "src",
      "vendor/entt",
      "vendor/tinyobjloader",
      
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
      "cudnn", -- CUDA Deep Neural Network library (if needed, optional)
      "c10",
      "torch_cpu",
      "torch_cuda"
   }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      defines { "TORCH_USE_CUDA" }

   vpaths {
      ["Shaders"] = { "shaders/**.rgen", "shaders/**.rchit", "shaders/**.rmiss", "shaders/**.rahit", "shaders/**.rint" },
   }

   -- 设置着色器文件的处理方式为“None”
   filter { "files:**.rgen or files:**.rchit or files:**.rmiss or files:**.rahit or files:**.rint" }
      buildaction "None"

      -- 定义着色器编译的输出目录
      shaderOutputDir = "%{cfg.targetdir}/shaders/compiled"

      -- 为着色器文件添加自定义构建步骤
      buildcommands {
         "{MKDIR} \"" .. shaderOutputDir .. "\"",
         "glslangValidator -V \"%{file.relpath}\" -o \"" .. shaderOutputDir .. "/%{file.basename}.spv\""
      }
      buildoutputs { shaderOutputDir .. "/%{file.basename}.spv" }
      buildmessage "Compiling shader %{file.name}"

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
