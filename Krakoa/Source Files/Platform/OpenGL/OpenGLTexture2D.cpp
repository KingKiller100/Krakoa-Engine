#include "Precompile.hpp"
#include "OpenGLTexture2D.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <stb_image.hpp>

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string_view& path)
		: path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(KRK_TRUE);
		auto data = stbi_load(path.data(), &width, &height, &channels, 0);
		KRK_FATAL(data, "Failed to load image");

		dimensions.X() = width;
		dimensions.Y() = height;

		glGenTextures(1, &rendererID);
		glTextureStorage2D(rendererID, 1, GL_RGB8, width, height);

		glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(rendererID, 0, 0, 0, width, height, channels == 3 ? GL_RGB8 : GL_RGBA8, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		 
		stbi_image_free(data);
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
		glBindTextureUnit(slot, rendererID);
	}

}
