#pragma once
#include "PostProcessingBase.h"
#include "torch/torch.h"
#include <torch/script.h>
#include <glm/glm.hpp>
#include "Postprocessing/ImagetoTensor.cuh"

namespace RMC
{
    class DLSSProcess :
        public PostProcessingBase
    {
    public:
        DLSSProcess();
        ~DLSSProcess() = default;
        uint32_t* process(uint32_t* image_data, int width, int height);
    private:
        torch::jit::script::Module model;
    };

}

