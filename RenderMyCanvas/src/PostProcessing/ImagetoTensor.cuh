// my_kernels.cuh
#ifndef MY_KERNELS_CUH
#define MY_KERNELS_CUH

__global__ void ConvertImageToTensorKernel(const uint32_t* img_data,float* output_tensor,int width, intheight);


#endif // MY_KERNELS_CUH
