#include "Precompile.hpp"
#include "OpenGLTexture2D.hpp"

#include "../../Instrumentor.hpp"
#include "../../Core/Logging/CoreLogger.hpp"

#include <stb_image.hpp>

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height, const bool cache)
		: path("N/A"), dimensions(kmaths::Vector2u(width, height)), internalFormat(GL_RGBA8), fileFormat(GL_RGBA)
	{
		KRK_PROFILE_FUNCTION();

		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path, const bool cache)
		: path(path), internalFormat(0), fileFormat(0)
	{
		KRK_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(KRK_TRUE);
		buffer = stbi_load(path.data(), &width, &height, &channels, 4);
		KRK_FATAL(buffer != nullptr, "Failed to load image");

		dimensions = kmaths::Vector2u(width, height);

		glGenTextures(1, &rendererID);
		glBindTexture(GL_TEXTURE_2D, rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

		KRK_FATAL(internalFormat && fileFormat, "Texture Format give is not supported by Krakoa");

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, fileFormat, GL_UNSIGNED_BYTE, buffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (!cache && buffer)
			stbi_image_free(buffer);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		KRK_PROFILE_FUNCTION();

		glDeleteTextures(1, &rendererID);
	}

	uint32_t OpenGLTexture2D::GetWidth() const noexcept
	{
		return dimensions.X();
	}

	uint32_t OpenGLTexture2D::GetHeight() const noexcept
	{
		return dimensions.Y();
	}

	const kmaths::Vector2u& OpenGLTexture2D::GetDimensions() const noexcept
	{
		KRK_PROFILE_FUNCTION();
		return dimensions;
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const
	{
		KRK_PROFILE_FUNCTION();

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, rendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
		KRK_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture2D::SetData(const void* data, const uint32_t size)
	{
		KRK_PROFILE_FUNCTION();

		const auto bytes_per_pixel = fileFormat == GL_RGBA ? 4 : 3;
		const auto textureSize = GetWidth() * GetHeight() * bytes_per_pixel;
		KRK_FATAL(size == textureSize, "Texture data must be entire texture");
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, GetWidth(), GetHeight(), 0, fileFormat, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
