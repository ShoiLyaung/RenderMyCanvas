### 图元绘制文档

#### 1. 图元基础框架介绍
在 `DrawBoard` 类中，所有的图形绘制操作由不同的图元（例如直线、圆、椭圆等）来实现。每个图元都是 `Primitive` 基类的派生类。为了实现各种图形，大家需要定义各自负责的图元类，继承并实现 `Primitive` 的纯虚函数 `Draw()`，这个函数负责将图形绘制到指定的图像数据中。

每个图元类需要实现的主要功能包括：
- **构造函数**：初始化图元的参数（如起始点、半径等）。
- **Draw() 函数**：该函数的主要职责是将图元绘制到图像数据（`uint32_t* imageData`）中。

#### 2. Draw() 函数的定义
每个图元的 `Draw()` 函数需要具备以下签名：
```cpp
void Draw(uint32_t* imageData, uint32_t width, uint32_t height) const override;
```
- **参数说明**：
  - `imageData`：图像数据的指针，图元需要在此图像缓冲区中进行绘制。
  - `width` 和 `height`：图像的宽度和高度，用于确保绘制在图像边界内。
- **返回数据**：`Draw()` 函数不返回数据，而是直接在 `imageData` 中修改对应的像素值，以实现图形的绘制。

#### 3. 数据交互要求
每个图元类在绘制时，需要更新 `imageData`，将对应位置的像素填充为指定的颜色（当前默认使用白色）。以下是一个简单的示例：
```cpp
void Line::Draw(uint32_t* imageData, uint32_t width, uint32_t height) const
{
    int x0 = static_cast<int>(m_Start.x);
    int y0 = static_cast<int>(m_Start.y);
    int x1 = static_cast<int>(m_End.x);
    int y1 = static_cast<int>(m_End.y);

    // 使用 Bresenham 算法绘制直线
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        if (x0 >= 0 && x0 < static_cast<int>(width) && y0 >= 0 && y0 < static_cast<int>(height))
        {
            imageData[y0 * width + x0] = 0xffffffff; // 设置为白色
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
```
每个小组成员可以参考这个示例来实现自己负责的图元类。

#### 4. 实现要求
- **边界检查**：确保绘制时不会越界（例如，`x` 和 `y` 的坐标应在 `0` 到 `width/height` 范围内）。
- **图元种类**：可以根据分工实现不同的图元类，例如 `Line`（直线）、`Circle`（圆）、`Ellipse`（椭圆）等。
- **灵活性**：建议使用灵活的参数，例如中心点、半径等，使得图元的创建和变换更加方便。

#### 5. 工具实现规范
除了图元绘制之外，还有填充、裁剪与变换工具的实现。每个工具的主要职责是对现有的图元进行操作或修改。

##### 5.1 填充工具
- **目标**：填充多边形的内部区域。
- **方法函数**：实现一个 `Fill()` 函数，对选定的多边形进行填充。
- **参数**：
  - `imageData`：图像数据指针。
  - `width` 和 `height`：图像的宽度和高度。
- **返回数据**：`Fill()` 函数不返回数据，而是直接在 `imageData` 中更新填充区域的像素值。
- **算法建议**：可以使用扫描线填充算法或种子填充算法（四连通或八连通）。

##### 5.2 裁剪工具
- **目标**：对现有的图元进行裁剪，使其只显示在指定的窗口范围内。
- **方法函数**：实现一个 `Clip()` 函数，对选定的图元进行裁剪。
- **参数**：
  - `clipWindow`：裁剪窗口的范围（例如，矩形的边界）。
  - `imageData`、`width` 和 `height`：用于更新图像数据。
- **返回数据**：`Clip()` 函数不返回数据，而是修改 `imageData` 以反映裁剪后的图元。
- **算法建议**：可以使用 Cohen-Sutherland 裁剪算法或中点分割裁剪算法。

##### 5.3 变换工具
- **目标**：对图元进行平移、缩放或旋转操作。
- **方法函数**：实现以下变换函数：
  - `Translate()`：对图元进行平移。
  - `Scale()`：对图元进行缩放。
  - `Rotate()`：对图元进行旋转。
- **参数**：
  - `transformMatrix`：用于变换的矩阵（例如 3x3 矩阵）。
  - `imageData`、`width` 和 `height`：用于更新图像数据。
- **返回数据**：每个变换函数不直接返回数据，而是更新图元的内部数据（例如坐标位置），并在 `Draw()` 函数中反映变换后的效果。

#### 6. 其他注意事项
- **图元添加到 DrawBoard**：使用 `DrawBoard::AddPrimitive()` 函数将实现的图元添加到 `DrawBoard` 中，这样就可以在 `Render()` 中调用各个图元的 `Draw()` 方法进行渲染。
- **临时图元绘制**：绘制过程中，可以使用 `SetTemporaryPrimitive()` 方法来设置临时图元，用于实现例如拖拽绘制时的实时反馈。
