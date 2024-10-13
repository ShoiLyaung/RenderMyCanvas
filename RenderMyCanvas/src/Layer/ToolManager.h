#pragma once

namespace RMC {
    class ToolManager
    {
    public:
        enum class Tool
        {
            None,
            Line,
            Circle,
            Ellipse,
            Fill,
            Clip,
            Transform
        };

        void SetCurrentTool(Tool tool) { m_CurrentTool = tool; }
        Tool GetCurrentTool() const { return m_CurrentTool; }

    private:
        Tool m_CurrentTool = Tool::None;
    };
} // namespace RMC