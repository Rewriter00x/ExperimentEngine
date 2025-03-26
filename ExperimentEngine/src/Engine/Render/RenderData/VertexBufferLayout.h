#pragma once

namespace Exp
{
    enum class ShaderDataType : uint8
    {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool,
    };
    
    uint32 ShaderDataTypeSize(ShaderDataType type);
    uint32 ShaderDataTypeComponentCount(ShaderDataType type);
    
    struct BufferLayoutElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32 Size;
        uint32 Offset;
        bool Normalized;

        BufferLayoutElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        inline uint32 GetComponentCount() const { return ShaderDataTypeComponentCount(Type); }
    };
    
    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(const std::initializer_list<BufferLayoutElement>& elements);

        void ApplyLayoutToRenderAPI() const;
        
    private:
        void CalculateOffsetsAndStride();
        
        std::vector<BufferLayoutElement> m_Elements;
        uint32 m_Stride = 0;
        
    };
}
