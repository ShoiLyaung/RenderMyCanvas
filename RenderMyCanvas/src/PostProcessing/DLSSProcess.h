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
        std::shared_ptr<Walnut::Image> process(std::shared_ptr<Walnut::Image> img);
    private:
        torch::jit::script::Module model;
    };

}

