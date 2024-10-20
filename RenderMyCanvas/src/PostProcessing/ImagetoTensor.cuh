#include<cuda_runtime.h>
// my_kernels.cuh
#ifndef MY_KERNELS_CUH
#define MY_KERNELS_CUH

void ImageToTensor(const uint32_t* img_data, float* tensor_data, int width, int height);

__global__ void ImageToTensorKernel(const uint32_t* img_data,float* output_tensor,int width, int height);

__global__ void convertTensorToImageKernel(const float* tensor_data, uint32_t* img_data, int height, int width);
#endif // MY_KERNELS_CUH
