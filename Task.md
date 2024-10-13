## DrawBoard 协作指南

本指南为协作者提供了实现绘图图元（例如直线、圆、椭圆）及其绘制逻辑的详细说明。下面是框架的详细信息、必要的概念，以及如何将您的工作集成到现有系统中。

### DrawBoard 概述
`DrawBoard` 类管理画布上的绘图操作，并通过鼠标事件与用户交互。它是绘制各种图元并实时渲染的主要接口。

您的任务是定义和实现用户将在画布上绘制的不同几何图元。这些图元包括但不限于直线、圆和椭圆。每个图元必须定义其绘制方法，给定一些点或属性。

### 需要了解的关键组件

所有源代码位于 `src/` 目录中，在 `#include` 的时候应该以 `src/` 下的路径开始（不包括 `src/`），其中包含以下关键组件：

1. **图元基类**
   - 所有二维图元应继承自基类 `Primitive2D`，这将为所有类型的形状提供一致的接口。
   - 通过重载 `Draw()` 函数来实现绘制，该函数接收图像数据缓冲区、宽度和高度参数。

   `Primitives/Line.h`：
   ```cpp
   #pragma once

    #include <glm/glm.hpp>
    #include "Primitive.h"

    namespace RMC {
        class Line : public Primitive2D {
        public:
            Line(const glm::vec3& start, const glm::vec3& end);
            void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
        private:
            glm::vec3 Start;
            glm::vec3 End;
            float LineWidth;
        };
    } // namespace RMC
   ```

2. **需要实现的图元**
   - **直线**：由两个点（起点和终点）定义。
   - **圆**：不同的模式，如“中心和半径”或“直径”。(可以通过重载构造函数来实现)
   - **椭圆**：例如“中心和轴长”或“焦点和轴长”模式。

   每个类都应实现 `Draw()` 函数，以将自身渲染到画布上。

3. **DrawBoard 类中的函数**
   - **`CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points)`**：使用收集的点来创建实际的图元对象（例如直线或圆）。
   - **`AddPrimitive(std::shared_ptr<Primitive> primitive)`**：将已完成的图元添加到已绘制的形状列表（`m_Primitives`）中，这些形状会被永久渲染。
   - **`SetTemporaryPrimitive(std::shared_ptr<Primitive> primitive)`**：更新 `m_TemporaryPrimitive`，用于显示正在绘制的图元预览。
   - **`ClearTemporaryPrimitive()`**：在确认或放弃图元后，清除临时图元。

### 需要修改的部分和实现方法

1. **DrawBoard 中的绘制逻辑**
   - 协作者需要完善 **`CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points)`** 函数。该函数通过鼠标的记录点来创建实际的图元对象（例如直线或圆）。
   `DrawBoard/DrawBoard.cpp`：
   ```cpp
    std::shared_ptr<Primitive> DrawBoard::CreatePrimitiveFromPoints(const std::vector<glm::vec2>& points) {
        switch (m_CurrentDrawingMode) {
        case DrawingMode::Line:
            return std::make_shared<Line>(glm::vec3(points[0], 0.0f), glm::vec3(points[1], 0.0f));
        case DrawingMode::Circle_CenterRadius:
            return std::make_shared<Circle>(glm::vec3(points[0], 0.0f), glm::distance(points[0], points[1]));
        //case DrawingMode::Circle_Diameter:
        //    return std::make_shared<Circle>(points[0], points[1]);
        //case DrawingMode::Ellipse_Foci:
        //    return std::make_shared<Ellipse>(points[0], points[1], points[2]);
        //case DrawingMode::Ellipse_CenterAxes:
        //    return std::make_shared<Ellipse>(points[0], points[1], points[2]);
        }
    }
   ```

    - 但所选图形和所选画法都可能影响所需要的记录点数量。所以协作者也需要完善 **`RequiredPointsForCurrentShape() const`** 中的返回值，以便在鼠标事件中正确处理记录点。
   `DrawBoard/DrawBoard.cpp`：
   ```cpp
    int DrawBoard::RequiredPointsForCurrentShape() const {
        switch (m_CurrentDrawingMode) {
        case DrawingMode::Line:
            return 2;
        case DrawingMode::Circle_CenterRadius:
            return 2;
        //case DrawingMode::Circle_Diameter:
        //    return 2;
        //case DrawingMode::Ellipse_Foci:
        //    return 3;
        //case DrawingMode::Ellipse_CenterAxes:
        //    return 3;
        }
    }
   ```

2. **具体图元的实现及调用算法**
   - 每个图元类需要实现自己的 `Draw()` 函数，用于将图元绘制到画布上。
   - 例如，直线可以使用 Bresenham 算法，圆可以使用中点圆算法。
   `Primitives/Line.cpp`：
   ```cpp
    void Line::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
        // Example: use Bresenham's algorithm to draw the line
        LineAlgorithm::Bresenham(Start, End, imageData, width, height);
    }
   ```
    `Primitives/Circle.cpp`：
    ```cpp
     void Circle::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const {
          // Example: use Midpoint Circle algorithm to draw the circle
          CircleAlgorithm::Midpoint(Center, Radius, imageData, width, height);
     }
    ```
    - 图元的绘制算法在 `Algorithms/LineAlgorithm` 和 `Algorithms/CircleAlgorithm` 目录下创建对应的头文件，用于实现具体的绘图算法。
    `Algorithms/LineAlgorithm.h`：
    ```cpp
    #pragma once
    #include <glm/glm.hpp>

    namespace RMC {

        namespace LineAlgorithm {

            void Bresenham(const glm::vec3& start, const glm::vec3& end, uint32_t* imageData, uint32_t width, uint32_t height) {
                // Bresenham's algorithm implementation
            }

        } // namespace LineAlgorithm

    } // namespace RMC
    ```

3. **文件结构**
   - 建议每个图元有单独的头文件和源文件。例如：
     - `Primitives/Line.h` 和 `Primitives/Line.cpp`
     - `Primitives/Circle.h` 和 `Primitives/Circle.cpp`
     - `Primitives/Ellipse.h` 和 `Primitives/Ellipse.cpp`
   - 图元的绘制算法只有头文件（实现放在头文件中）。例如：
     - `Algorithms/LineAlgorithm.h`
     - `Algorithms/CircleAlgorithm.h`
   - 在 `DrawBoard.h` 中包含所有必要的头文件，以便在 `DrawBoard` 中使用这些图元。
   - 使用统一的命名空间 `RMC` 来组织所有代码，确保命名的一致性和避免命名冲突。

### 实现要求

1. **构造函数和初始化**
   - 每个图元应具有至少一个构造函数，该构造函数接受必要的参数（例如直线的起点和终点）。

2. **绘图逻辑**
   - 在每个图元的 `Draw()` 方法中实现绘图逻辑。
   - 使用提供的缓冲区（`imageData`）和画布尺寸（`width`、`height`）来绘制像素。

3. **用于实时反馈的临时图元**
   - 临时图元（`m_TemporaryPrimitive`）将为用户提供交互时的反馈。
   - 重要的是，每个图元的 `Draw()` 函数能够高效地处理重绘，因为在用户交互过程中它会被持续调用。

4. **坐标系统**
   - 画布使用二维坐标系统，原点位于左上角。
   - x 轴向右延伸，y 轴向下延伸。

### 集成
- **头文件**：定义每个图元（例如 `Line.h`、`Circle.h`），并在 DrawBoard（`DrawBoard.h`）中包含它们。
- **命名空间**：所有类都应在 `RMC` 命名空间下实现。
- **测试**：确保每个图元能够处理边界情况，例如部分绘制在屏幕外或具有无效参数（例如长度为零的直线）。

### 示例工作流程
1. **用户选择绘图模式**（例如直线、圆）。
2. **鼠标事件触发 `OnMouseEvent`**，它收集点并更新临时形状。
3. 一旦收集到足够的点，调用 **`CreatePrimitiveFromPoints`** 生成最终形状，并通过 **`AddPrimitive`** 添加到画布中。
4. **渲染** 通过 `Render()` 函数处理，该函数调用所有已完成的图元以及任何临时图元的 `Draw()`。

### 协作注意事项
- 保持绘图函数高效，因为它们可能在鼠标交互过程中频繁调用。
- 确保图元足够通用，以支持各种绘图模式（例如定义圆的不同方式）。
- 使用清晰且一致的命名约定，以便于集成。
- 记录您实现的任何特定绘图算法，例如 Bresenham 直线算法或中点圆算法。