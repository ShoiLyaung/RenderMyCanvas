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

std::shared_ptr<Walnut::Image> RMC::PostProcessingPipeLine::process(std::shared_ptr<Walnut::Image> img)
{
	std::shared_ptr<Walnut::Image> FinalImage = std::make_shared<Walnut::Image>(img->GetWidth(), img->GetHeight(), Walnut::ImageFormat::RGBA);
	for (auto &process : ProcessList)
	{
		FinalImage = process->process(img);
		img = FinalImage;
	}
	return FinalImage;
}
