#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size); // size is total size in bytes of all the data, not the number or data points
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};