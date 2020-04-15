#include "Precompile.hpp"
#include "OpenGLTexture2D.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <stb_image.hpp>

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path, const bool cache)
		: path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(KRK_TRUE);
		buffer = stbi_load(path.data(), &width, &height, &channels, 0);
		KRK_FATAL(buffer, "Failed to load image");

		dimensions = kmaths::Vector2u( width, height );

		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		unsigned internalFormat = 0;
		unsigned fileFormat = 0;

		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			fileFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			fileFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, fileFormat, GL_UNSIGNED_BYTE, buffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		 
		if (!cache && buffer)
		stbi_image_free(buffer);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &rendererID);
	}

	uint32_t OpenGLTexture2D::GetWidth() const noexcept
	{
		return dimensions.X();
	}

	uint32_t OpenGLTexture2D::Geteight() const noexcept
	{
		return dimensions.Y();
	}

	const kmaths::Vector2u& OpenGLTexture2D::GetDimensions() const noexcept
	{
		return dimensions;
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, rendererID);
	}

}
