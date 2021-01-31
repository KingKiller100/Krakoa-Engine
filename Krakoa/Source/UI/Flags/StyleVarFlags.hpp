#pragma once

#include <Utility/Enum/kEnum.hpp>

namespace krakoa::ui
{
	ENUM_CLASS(StyleVarFlags, int,
        Alpha,               // float     Alpha
        WindowPadding,       // ImVec2    WindowPadding
        WindowRounding,      // float     WindowRounding
        WindowBorderSize,    // float     WindowBorderSize
        WindowMinSize,       // ImVec2    WindowMinSize
        WindowTitleAlign,    // ImVec2    WindowTitleAlign
        ChildRounding,       // float     ChildRounding
        ChildBorderSize,     // float     ChildBorderSize
        PopupRounding,       // float     PopupRounding
        PopupBorderSize,     // float     PopupBorderSize
        FramePadding,        // ImVec2    FramePadding
        FrameRounding,       // float     FrameRounding
        FrameBorderSize,     // float     FrameBorderSize
        ItemSpacing,         // ImVec2    ItemSpacing
        ItemInnerSpacing,    // ImVec2    ItemInnerSpacing
        IndentSpacing,       // float     IndentSpacing
        ScrollbarSize,       // float     ScrollbarSize
        ScrollbarRounding,   // float     ScrollbarRounding
        GrabMinSize,         // float     GrabMinSize
        GrabRounding,        // float     GrabRounding
        TabRounding,         // float     TabRounding
        ButtonTextAlign,     // ImVec2    ButtonTextAlign
        SelectableTextAlign  // ImVec2    SelectableTextAlign
	);
}