#pragma once

#include <Utility/Enum/kEnum.hpp>

namespace krakoa::ui
{
    ENUM_CLASS(InputTextFlags, int,
        None = 0,
        CharsDecimal = 1 << 0,   // Allow 0123456789.+-*/
        CharsHexadecimal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
        CharsUppercase = 1 << 2,   // Turn a..z into A..Z
        CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
        AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
        EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
        CallbackCompletion = 1 << 6,   // Callback on pressing TAB (for completion handling)
        CallbackHistory = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
        CallbackAlways = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
        CallbackCharFilter = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
        AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
        CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
        NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
        AlwaysInsertMode = 1 << 13,  // Insert mode
        ReadOnly = 1 << 14,  // Read-only mode
        Password = 1 << 15,  // Password mode, display all characters as '*'
        NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
        CharsScientific = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
        CallbackResize = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
        
        Multiline = 1 << 20,  // For internal use by InputTextMultiline()
        NoMarkEdited = 1 << 21   // For internal use by functions using InputText() before reformatting data
    );
}
