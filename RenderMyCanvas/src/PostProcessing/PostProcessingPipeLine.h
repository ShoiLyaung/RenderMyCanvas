#pragma once
#include<vector>
#include<memory>
#include"PostProcessingBase.h"
namespace RMC
{
	class PostProcessingPipeLine
	{
	private:
		std::vector<std::shared_ptr<PostProcessingBase> > ProcessList; // 存储所有后处理类指针
	public:
		PostProcessingPipeLine();
		~PostProcessingPipeLine() = default;
		PostProcessingPipeLine(std::vector<std::shared_ptr<PostProcessingBase> > list);
		void addProcess(std::shared_ptr<PostProcessingBase> process);
		uint32_t* RMC::PostProcessingPipeLine::process(uint32_t* image_data, int width, int height);
	};
}


