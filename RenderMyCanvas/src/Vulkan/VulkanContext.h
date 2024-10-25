// VulkanContext.h
#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace Walnut {
    class Application; // 前向声明
}

namespace RMC {

    class VulkanContext
    {
    public:
        VulkanContext(Walnut::Application* app);
        ~VulkanContext();

        void InitVulkan();
        void Cleanup();

        // 添加获取Vulkan对象的方法
        VkInstance GetInstance() const { return m_Instance; }
        VkDevice GetDevice() const { return m_Device; }
        VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        VkQueue GetPresentQueue() const { return m_PresentQueue; }
        VkCommandPool GetCommandPool() const { return m_CommandPool; }
        VkSurfaceKHR GetSurface() const { return m_Surface; }

        // 添加获取光线追踪属性的方法
        const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& GetRayTracingProperties() const { return m_RayTracingProperties; }

    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface();
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();

        bool IsDeviceSuitable(VkPhysicalDevice device);
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete()
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

        std::vector<const char*> GetRequiredExtensions();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* /*pUserData*/);

        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        void DestroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);

    private:
        Walnut::Application* m_Application; // 修改为Application*

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkSurfaceKHR m_Surface;

        VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
        VkDevice m_Device;

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;

        VkCommandPool m_CommandPool;

        VkPhysicalDeviceProperties m_PhysicalDeviceProperties{};
        VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures{};

        // 光线追踪属性
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_RayTracingProperties{};

        const std::vector<const char*> m_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        const std::vector<const char*> m_DeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
            VK_KHR_SPIRV_1_4_EXTENSION_NAME,
            VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME,
        };

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
    };

} // namespace RMC