#include "PostProcessingPipeLine.h"
#include <iostream>
RMC::PostProcessingPipeLine::PostProcessingPipeLine()
{
} 

RMC::PostProcessingPipeLine::PostProcessingPipeLine(std::vector<std::shared_ptr<PostProcessingBase> > list)
{
	ProcessList = list;
}

void RMC::PostProcessingPipeLine::addProcess(std::shared_ptr<PostProcessingBase> process)
{
	ProcessList.push_back(process);
}

uint32_t* RMC::PostProcessingPipeLine::process(uint32_t *image_data, int width, int height)
{
	uint32_t* FinalImageData = new uint32_t[width * height];
	for (auto &process : ProcessList)
	{
		FinalImageData = process->process(image_data, width, height);
	}
	return FinalImageData;
}
