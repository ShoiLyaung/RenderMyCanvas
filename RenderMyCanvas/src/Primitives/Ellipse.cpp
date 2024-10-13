#include "Circle.h"
#include "Algorithm/EllipseAlgorithm.h"

namespace RMC {

    Ellipse::Ellipse(const glm::vec3& F1, const glm::vec3& F2)
		: F1(F1), F2(F2) {
    }

    void Ellipse::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        //EllipseAlgorithm
    }

} // namespace RMC