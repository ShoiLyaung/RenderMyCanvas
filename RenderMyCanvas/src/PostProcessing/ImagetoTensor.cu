#include"ImagetoTensor.cuh"
#include<cuda_runtime.h>

// CUDA 核函数，用来处理图像数据
__global__ void ConvertImageToTensorKernel(const uint32_t* img_data, float* output_tensor, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    // 计算在原始数据中的偏移量
    int index = y * width + x;
    uint32_t pixel = img_data[index];

    // 调用设备端的 ConvertFromRGBA 函数
    glm::vec4 color = Utils::ConvertFromRGBA(pixel);

    // 存储到输出张量中
    int tensor_idx = (y * width + x); // 在 GPU 上处理张量为连续的内存空间
    output_tensor[tensor_idx] = color.r;           // Red
    output_tensor[tensor_idx + width * height] = color.g; // Green
    output_tensor[tensor_idx + 2 * width * height] = color.b; // Blue
}