#include "Renderer.h"
#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderedID);
	glBindVertexArray(m_RenderedID);

	Unbind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RenderedID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{

	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast <const void*> (offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}	
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RenderedID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
