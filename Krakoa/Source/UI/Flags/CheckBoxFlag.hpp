#pragma once

#include <Utility/Enum/kEnum.hpp>

namespace krakoa::ui
{
    ENUM_CLASS(ComboBoxFlag, int,
        None = 0,
        PopupAlignLeft = 1 << 0,   // Align the popup toward the left by default
        HeightSmall = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
        HeightRegular = 1 << 2,   // Max ~8 items visible (default)
        HeightLarge = 1 << 3,   // Max ~20 items visible
        HeightLargest = 1 << 4,   // As many fitting items as possible
        NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
        NoPreview = 1 << 6,   // Display only a square arrow button
        HeightMask_ = HeightSmall | HeightRegular | HeightLarge | HeightLargest
    );
}

