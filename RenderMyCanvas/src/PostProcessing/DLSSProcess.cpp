#include "DLSSProcess.h"
#include <chrono>
#include <cuda_runtime.h>

namespace Utils {
    static glm::vec4 ConvertFromRGBA(uint32_t color)
    {
        // 提取 RGBA 各通道
        uint32_t r = (color & 0x000000FF); // 提取红色通道
        uint32_t g = (color & 0x0000FF00) >> 8; // 提取绿色通道
        uint32_t b = (color & 0x00FF0000) >> 16; // 提取蓝色通道
        uint32_t a = (color & 0xFF000000) >> 24; // 提取透明度通道

        // 将每个通道转换回 [0.0f, 1.0f] 范围
        return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    static uint32_t ConvertToRGBA(const glm::vec4& color)
    {
        uint32_t r = (uint32_t)(color.r * 255.0f);
        uint32_t g = (uint32_t)(color.g * 255.0f);
        uint32_t b = (uint32_t)(color.b * 255.0f);
        uint32_t a = (uint32_t)(color.a * 255.0f);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }


    torch::Tensor WalnutImageToTensor(std::shared_ptr<Walnut::Image> img)
    {
        int width = img->GetWidth();
        int height = img->GetHeight();
        int channels = 3;

        torch::Tensor tensor;

        // 从 Walnut::Image 获取原始数据
        const uint32_t* img_data = reinterpret_cast<const uint32_t*>(img->GetDescriptorSet());

        // 为张量的数据分配GPU内存
        float* d_tensor_data;

        cudaMalloc(&d_tensor_data, width  * height * channels * sizeof(float));

        ImageToTensor(img_data, d_tensor_data, width, height);

        torch::TensorOptions options = torch::TensorOptions().device(torch::kCUDA).dtype(torch::kFloat32);


        auto deleter = [](void* ptr) { cudaFree(ptr); };

        tensor = torch::from_blob(d_tensor_data, { 1, channels, height, width }, deleter, options);

        return tensor;
    }

    std::shared_ptr<Walnut::Image> tensorToWalnutImage(const torch::Tensor& tensor) {
        // 1. 获取张量的尺寸
        auto sizes = tensor.sizes();
        //std::cout << sizes << std::endl;
        if (sizes.size() != 4 || sizes[0] != 1 || sizes[1] != 3) {
            throw std::invalid_argument("Tensor must be of shape [1, 3, height, width]");
        }

        int height = sizes[2];
        int width = sizes[3];

        // 2. 创建 Walnut::Image 对象
        auto img = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA); // 4 表示 RGBA 通道

        // 3. 将张量数据复制到 Walnut::Image
        float* data_ptr = tensor.data_ptr<float>();

        uint32_t* img_data = new uint32_t[width * height];

        convertTensorToImage(data_ptr, img_data, width, height);

        // 7. 将图像数据设置到 Walnut::Image
        img->SetData(img_data);

        delete[] img_data;

        return img;
    }
}

RMC::DLSSProcess::DLSSProcess()
{
    try {
        std::cout << "E:\\Documents\\UniversityFiles\\Forth\\CG\\RenderMyCanvas\\SRCNN.pt" << std::endl;
        model = torch::jit::load("E:\\Documents\\UniversityFiles\\Forth\\CG\\RenderMyCanvas\\SRCNN.pt");
        model.eval();
        model.to(torch::kCUDA);
    }
    catch (const c10::Error& e) {
        std::cerr << "Error loading the model: " << e.what() << std::endl;
    }
}

std::shared_ptr<Walnut::Image> RMC::DLSSProcess::process(std::shared_ptr<Walnut::Image> img)
{
    //std::cout << "DLSSProcess" << std::endl;
    //std::cout << "img" << img->GetWidth() << img->GetHeight() << std::endl;
    torch::Tensor inputTensor = Utils::WalnutImageToTensor(img).toType(torch::kFloat32);
    //std::cout<<"inputTensor"<<inputTensor.sizes()<<std::endl;
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(inputTensor);
    torch::Tensor outputTensor = model.forward(inputs).toTensor();
    //std::cout << "outputTensor" << outputTensor.sizes() << std::endl;
    std::shared_ptr<Walnut::Image> outputImage = Utils::tensorToWalnutImage(outputTensor);
    //std::cout << "outputImage" << outputImage->GetWidth() << outputImage->GetHeight() << std::endl;

    return img;
}


