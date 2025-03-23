#include "exppch.h"
#include "VertexBufferLayout.h"

namespace Exp
{
    uint32 ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None:      return 0;
            case ShaderDataType::Float:		return 4;
            case ShaderDataType::Float2:	return 4 * 2;
            case ShaderDataType::Float3:	return 4 * 3;
            case ShaderDataType::Float4:	return 4 * 4;
            case ShaderDataType::Mat3:		return 4 * 3 * 3;
            case ShaderDataType::Mat4:		return 4 * 4 * 4;
            case ShaderDataType::Int:		return 4;
            case ShaderDataType::Int2:		return 4 * 2;
            case ShaderDataType::Int3:		return 4 * 3;
            case ShaderDataType::Int4:		return 4 * 4;
            case ShaderDataType::Bool:		return 1;
        }
        
        EXP_ASSERT_MSG(false, "Unknown ShaderDataType!");
        return 0;
    }

    uint32 ShaderDataTypeComponentCount(ShaderDataType type)
    {
        switch (type) {
            case ShaderDataType::None:      return 0;
            case ShaderDataType::Float:		return 1;
            case ShaderDataType::Float2:	return 2;
            case ShaderDataType::Float3:	return 3;
            case ShaderDataType::Float4:	return 4;
            case ShaderDataType::Mat3:		return 3 * 3; // TODO check
            case ShaderDataType::Mat4:		return 4 * 4;
            case ShaderDataType::Int:		return 1;
            case ShaderDataType::Int2:		return 2;
            case ShaderDataType::Int3:		return 3;
            case ShaderDataType::Int4:		return 4;
            case ShaderDataType::Bool:		return 1;
        }

        EXP_ASSERT_MSG(false, "Unknown ShaderDataType!");
        return 0;
    }
    
    VertexBufferLayout::VertexBufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
        : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    void VertexBufferLayout::CalculateOffsetsAndStride()
    {
        uint32 offset = 0;
        m_Stride = 0;

        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }
}
