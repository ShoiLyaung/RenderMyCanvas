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
		PostProcessingPipeLine(std::vector<std::shared_ptr<PostProcessingBase> > list);
		void addProcess(std::shared_ptr<PostProcessingBase> process);
		std::shared_ptr<Walnut::Image> process(std::shared_ptr<Walnut::Image> img);
	};
}


