#pragma once

#include "Vertex.h"
#include "Algorithm/AlgorithmType.h"

namespace RMC {
	struct LineComponent {
		Vertex start;
		Vertex end;
		float lineWidth;
		AlgorithmType algorithm;
	};
} // namespace RMC