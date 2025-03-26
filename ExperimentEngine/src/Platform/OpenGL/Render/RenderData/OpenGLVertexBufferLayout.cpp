#include "exppch.h"
#include "OpenGLVertexBufferLayout.h"

#include "glad/glad.h"

namespace Exp
{
    uint32 ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::None:
                return GL_NONE;
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return GL_FLOAT;
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
        }

        EXP_ASSERT_MSG(false, "Unknown ShaderDataType!");
        return 0;
    }

    void VertexBufferLayout::ApplyLayoutToRenderAPI() const
    {
        EXP_ASSERT_MSG(!m_Elements.empty(), "Vertex buffer layout has no elements!");

        uint32 vertexBufferIndex = 0;
        for (const BufferLayoutElement& element : m_Elements)
        {
            switch (element.Type)
            {
            case ShaderDataType::None:
                EXP_ASSERT_MSG(false, "Trying to apply layout with ShaderDataType::None!");
                break;
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(vertexBufferIndex);
                    glVertexAttribPointer(vertexBufferIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        m_Stride,
                        (const void*)element.Offset);
                    ++vertexBufferIndex;
                    break;
                }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(vertexBufferIndex);
                    glVertexAttribIPointer(vertexBufferIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLBaseType(element.Type),
                        m_Stride,
                        (const void*)element.Offset);
                    ++vertexBufferIndex;
                    break;
                }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                {
                    const uint32 count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(vertexBufferIndex);
                        glVertexAttribPointer(vertexBufferIndex,
                            count,
                            ShaderDataTypeToOpenGLBaseType(element.Type),
                            element.Normalized ? GL_TRUE : GL_FALSE,
                            m_Stride,
                            (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(vertexBufferIndex, 1);
                        ++vertexBufferIndex;
                    }
                    break;
                }

            default:
                EXP_ASSERT_MSG(false, "Unknown ShaderDataType!");
            }
        }
    }

}