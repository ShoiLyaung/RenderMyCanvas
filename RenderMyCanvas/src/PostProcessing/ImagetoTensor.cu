#include"ImagetoTensor.cuh"
#include<cuda_runtime.h>

// CUDA �˺�������������ͼ������
__global__ void ConvertImageToTensorKernel(const uint32_t* img_data, float* output_tensor, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    // ������ԭʼ�����е�ƫ����
    int index = y * width + x;
    uint32_t pixel = img_data[index];

    // �����豸�˵� ConvertFromRGBA ����
    glm::vec4 color = Utils::ConvertFromRGBA(pixel);

    // �洢�����������
    int tensor_idx = (y * width + x); // �� GPU �ϴ�������Ϊ�������ڴ�ռ�
    output_tensor[tensor_idx] = color.r;           // Red
    output_tensor[tensor_idx + width * height] = color.g; // Green
    output_tensor[tensor_idx + 2 * width * height] = color.b; // Blue
}