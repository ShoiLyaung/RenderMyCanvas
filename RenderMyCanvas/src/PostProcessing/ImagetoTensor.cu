#include"ImagetoTensor.cuh"
#include <glm/glm.hpp>
#include<iostream>


void ImageToTensor(const uint32_t* img_data, float* tensor_data, int width, int height) 
{
    uint32_t* d_img_data;
    cudaMalloc(&d_img_data, width * height * sizeof(uint32_t));
    //std::cout << "start convert" << std::endl;
    cudaMemcpy(d_img_data, img_data, width * height * sizeof(uint32_t), cudaMemcpyHostToDevice);


    // 设定CUDA网格和线程块大小
    dim3 block_size(16, 16);
    dim3 grid_size((width*2 + block_size.x - 1) / block_size.x, (height*2 + block_size.y - 1) / block_size.y);

    // 启动 CUDA 核函数
    ImageToTensorKernel << <grid_size, block_size >> > (d_img_data, tensor_data, width, height);

    // 等待CUDA完成
    cudaDeviceSynchronize();

    // 释放分配的内存
    cudaFree(d_img_data);

}

// CUDA 核函数，用来处理图像数据
__global__ void ImageToTensorKernel(const uint32_t* img_data, float* tensor_data, int width, int height) {
    int h = blockIdx.y * blockDim.y + threadIdx.y;
    int w = blockIdx.x * blockDim.x + threadIdx.x;

    if (h < height && w < width) {
        int index = h * width + w;
        uint32_t pixel = img_data[index];

        // AABBGGRR 转换为浮点颜色值
        float r = 1 - ((pixel & 0x000000FF) / 255.0f);
        float g = 1 - ((pixel & 0x0000FF00) >> 8) / 255.0f;
        float b = 1 - ((pixel & 0x00FF0000) >> 16) / 255.0f;
        float a = 1 - ((pixel & 0xFF000000) >> 24) / 255.0f;

        int base_index_r = index;
        int base_index_b = height * width + index;
        int base_index_g = 2 * height * width + index;
        tensor_data[base_index_r] = r;
        tensor_data[base_index_g] = g;
        tensor_data[base_index_b] = b;
    }
}

void convertTensorToImage(const float* data_ptr, uint32_t* img_data, int width, int height)
{
    // 在设备上分配内存
       //float* d_tensor_data;
       uint32_t* d_img_data;
       //cudaMalloc(&d_tensor_data, sizeof(float)* (width* height * 3));
       cudaMalloc(&d_img_data, sizeof(uint32_t)* (width* height));

       // 将数据从主机复制到设备
       //cudaMemcpy(d_tensor_data, data_ptr, sizeof(float)* (width* height * 3), cudaMemcpyHostToDevice);

       // 4. 配置 CUDA 网格和块的维度
       dim3 blockSize(16, 16); // 每个块中的线程数
       dim3 gridSize((width + blockSize.x - 1) / blockSize.x, (height + blockSize.y - 1) / blockSize.y); // 网格尺寸

       // 5. 调用 CUDA 内核
       convertTensorToImageKernel << <gridSize, blockSize >> > (data_ptr, d_img_data, height, width);
       cudaDeviceSynchronize(); // 确保内核执行完成

       // 6. 从设备复制数据回主机
       cudaMemcpy(img_data, d_img_data, sizeof(uint32_t)* (width* height), cudaMemcpyDeviceToHost);

       // 8. 释放设备内存
       //cudaFree(d_tensor_data);
       cudaFree(d_img_data);

}

__global__ void convertTensorToImageKernel(const float* tensor_data, uint32_t* img_data, int height, int width) {
    int h = blockIdx.y * blockDim.y + threadIdx.y;
    int w = blockIdx.x * blockDim.x + threadIdx.x;

    if (h < height && w < width) {
        int index = h * width + w; 
        int base_index_r = index;
        int base_index_b = height * width + index;
        int base_index_g = 2 * height * width + index;

        // 浮点颜色值转换为整数
        uint32_t r = static_cast<uint32_t>((1 - tensor_data[base_index_r]) * 255.0f);
        uint32_t g = static_cast<uint32_t>((1 - tensor_data[base_index_g]) * 255.0f);
        uint32_t b = static_cast<uint32_t>((1 - tensor_data[base_index_b]) * 255.0f);

        // 组合成一个32位的像素值（AABBGGRR）
        uint32_t pixel = (0xFF << 24) | (b << 16) | (g << 8) | r; // 假设A通道总是255（完全不透明）

        img_data[index] = pixel;
    }
}
