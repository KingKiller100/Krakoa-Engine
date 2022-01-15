#include "Precompile.hpp"
#include "MessageBox.hpp"

#include "../Windows/MessageBoxWindows.hpp"

namespace krakoa::os
{
	void MessageBoxDisplay::Show(
		std::string_view title, std::string_view text
		, const std::function<MessageBoxCallBack>& cb
		, MessageBoxButtonSet buttons
		, MessageBoxIcon icon
		, MessageBoxDefaultButton defaultButton
		, MessageBoxModality modality
		, MessageBoxPosition positioning
	)
	{
		const auto response = MessageBoxDisplayWindows::Show( title, text
			, buttons
			, icon
			, defaultButton
			, modality
			, positioning );

		if ( cb )
			cb( response );
	}
}
