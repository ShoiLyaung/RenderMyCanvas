#pragma once

class ToolManager
{
public:
    enum class Tool
    {
        None,
        Line,
        Circle,
        Fill,
        Clip,
        Transform
    };

    void SetCurrentTool(Tool tool) { m_CurrentTool = tool; }
    Tool GetCurrentTool() const { return m_CurrentTool; }

private:
    Tool m_CurrentTool = Tool::None;
};