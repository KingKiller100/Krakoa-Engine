#pragma once

#include "../../Graphics/Helpers/AssetID.hpp"
#include "../../Graphics/2D/Textures/iTexture2D.hpp"

#include <filesystem>

namespace krakoa::gfx
{
	class OpenGLTexture2D : public iTexture2D
	{
	public:
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		OpenGLTexture2D(const std::filesystem::path& path, const bool cache = false);
		~OpenGLTexture2D();

		uint32_t GetWidth() const noexcept override;
		uint32_t GetHeight() const noexcept override;
		const kmaths::Vector2u& GetDimensions() const noexcept override;

		void Bind(const uint32_t slot = 0) const override;
		void Unbind() const override;

		void SetData(const void* data, const uint32_t size) override;

		bool operator ==(const iTexture& other) const noexcept override;


		AssetID GetAssetID() const noexcept override;

	private:
		std::filesystem::path path;
		kmaths::Vector2u dimensions;

		std::uint32_t rendererID;
		unsigned char* buffer;

		uint32_t internalFormat;
		uint32_t fileFormat;
	};
}

