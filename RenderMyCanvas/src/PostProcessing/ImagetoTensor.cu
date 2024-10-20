#include"ImagetoTensor.cuh"
#include <glm/glm.hpp>
#include<iostream>
 
void ImageToTensor(const uint32_t* img_data, float* tensor_data, int width, int height) 
{
    uint32_t* d_img_data;
    cudaMalloc(&d_img_data, width * height * sizeof(uint32_t));
    //std::cout << "start convert" << std::endl;
    cudaMemcpy(d_img_data, img_data, width * height * sizeof(uint32_t), cudaMemcpyHostToDevice);

    float *d_tensor_data;
    cudaMalloc(&d_tensor_data, width * height * 3 * sizeof(float));

    // �趨CUDA������߳̿��С
    dim3 block_size(16, 16);
    dim3 grid_size((width + block_size.x - 1) / block_size.x, (height + block_size.y - 1) / block_size.y);

    // ���� CUDA �˺���
    ImageToTensorKernel << <grid_size, block_size >> > (d_img_data, d_tensor_data, width, height);

    // �ȴ�CUDA���
    cudaDeviceSynchronize();

    // ��������豸���ƻ�����
    cudaMemcpy(tensor_data, d_tensor_data, width * height * 3 * sizeof(float), cudaMemcpyDeviceToHost);

    // �ͷŷ�����ڴ�
    cudaFree(d_img_data);
    cudaFree(d_tensor_data);
    //std::cout << "end convert" << std::endl;    

}

// CUDA �˺�������������ͼ������
__global__ void ImageToTensorKernel(const uint32_t* img_data, float* tensor_data, int width, int height) {
    int h = blockIdx.y * blockDim.y + threadIdx.y;
    int w = blockIdx.x * blockDim.x + threadIdx.x;

    if (h < height && w < width) {
        int index = h * width + w;
        uint32_t pixel = img_data[index];

        // RGBAת��Ϊ������ɫֵ
        float r = ((pixel >> 24) & 0xFF) / 255.0f;
        float g = ((pixel >> 16) & 0xFF) / 255.0f;
        float b = ((pixel >> 8) & 0xFF) / 255.0f;

        int base_index = (h * width + w) * 3;
        tensor_data[base_index] = r;
        tensor_data[base_index + 1] = g;
        tensor_data[base_index + 2] = b;
    }
}

void convertTensorToImage(const float* data_ptr, uint32_t* img_data, int width, int height)
{
    // ���豸�Ϸ����ڴ�
       float* d_tensor_data;
       uint32_t* d_img_data;
       cudaMalloc(&d_tensor_data, sizeof(float)* (width* height * 3));
       cudaMalloc(&d_img_data, sizeof(uint32_t)* (width* height));

       // �����ݴ��������Ƶ��豸
       cudaMemcpy(d_tensor_data, data_ptr, sizeof(float)* (width* height * 3), cudaMemcpyHostToDevice);

       // 4. ���� CUDA ����Ϳ��ά��
       dim3 blockSize(16, 16); // ÿ�����е��߳���
       dim3 gridSize((width + blockSize.x - 1) / blockSize.x, (height + blockSize.y - 1) / blockSize.y); // ����ߴ�

       // 5. ���� CUDA �ں�
       convertTensorToImageKernel << <gridSize, blockSize >> > (d_tensor_data, d_img_data, height, width);
       cudaDeviceSynchronize(); // ȷ���ں�ִ�����

       // 6. ���豸�������ݻ�����
       cudaMemcpy(img_data, d_img_data, sizeof(uint32_t)* (width* height), cudaMemcpyDeviceToHost);

       // 8. �ͷ��豸�ڴ�
       cudaFree(d_tensor_data);
       cudaFree(d_img_data);

}

__global__ void convertTensorToImageKernel(const float* tensor_data, uint32_t* img_data, int height, int width) {
    int x = blockIdx.x * blockDim.x + threadIdx.x; // ����ÿ���̵߳� x ����
    int y = blockIdx.y * blockDim.y + threadIdx.y; // ����ÿ���̵߳� y ����

    if (x < width && y < height) {
        // ���㵱ǰ���ص�����
        int index = y * width + x;

        // ��ȡ��ɫͨ��
        float r = tensor_data[index]; // Red
        float g = tensor_data[1 * height * width + index]; // Green
        float b = tensor_data[2 * height * width + index]; // Blue
        float a = 1.0f; // Alpha ֵ

        // ����ɫת��Ϊ uint32_t
        uint32_t uint_r = static_cast<uint32_t>(r * 255.0f);
        uint32_t uint_g = static_cast<uint32_t>(g * 255.0f);
        uint32_t uint_b = static_cast<uint32_t>(b * 255.0f);
        uint32_t uint_a = static_cast<uint32_t>(a * 255.0f);

        img_data[index] = (uint_a << 24) | (uint_b << 16) | (uint_g << 8) | uint_r; // ֱ�����ں������� RGBA ֵ
    }
}
