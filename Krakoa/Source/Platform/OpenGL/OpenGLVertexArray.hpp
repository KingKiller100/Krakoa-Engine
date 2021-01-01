#pragma once

#include "../../Graphics/Helpers/AssetID.hpp"
#include "../../Graphics/Resources/iVertexArray.hpp"

#include <vector>


namespace krakoa::graphics
{
	class OpenGLVertexArray : public iVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
		void Bind() const noexcept override;
		void Unbind() const noexcept override;

		void AddVertexBuffer(iVertexBuffer* vertexBuffer) override;
		void SetIndexBuffer(iIndexBuffer* indexBuffer) noexcept override;

		const std::vector<std::shared_ptr<iVertexBuffer>>& GetVertexBuffers() const noexcept override;
		const std::unique_ptr<iIndexBuffer>& GetIndexBuffer() const noexcept override;

	private:
		AssetID rendererID;

		std::vector<std::shared_ptr<iVertexBuffer>> vertexBuffers;
		std::unique_ptr<iIndexBuffer> pIndexBuffer;
	};
}
