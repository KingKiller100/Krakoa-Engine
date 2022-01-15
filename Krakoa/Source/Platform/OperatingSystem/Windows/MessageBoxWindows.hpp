#pragma once

#include "../MessageBox/MessageBox.hpp"
#include <string>

namespace krakoa::os
{
	class MessageBoxDisplayWindows
	{
	public:
		static MessageBoxResponse Show(
			std::string_view title
			, std::string_view text
			, MessageBoxButtonSet buttons
			, MessageBoxIcon icon
			, MessageBoxDefaultButton defaultButton
			, MessageBoxModality modal
			, MessageBoxPosition positioning
		);
		static MessageBoxResponse ResolveResponse( long response );
		static long ResolveButttonSet(MessageBoxButtonSet btnSet );
		static long ResolveIcon(MessageBoxIcon icon );
		static long ResolveDefaultButton(MessageBoxDefaultButton defBtn );
		static long ResolveModal(MessageBoxModality modal );
		static long ResolvePositioning(MessageBoxPosition positioning );
	};
}
