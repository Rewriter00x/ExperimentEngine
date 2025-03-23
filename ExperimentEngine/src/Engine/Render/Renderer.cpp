#include "exppch.h"
#include "Renderer.h"

#include "RenderAPI.h"

#include "RenderData/VertexArray.h"
#include "RenderData/VertexBuffer.h"
#include "RenderData/IndexBuffer.h"
#include "RenderData/Shader.h"

namespace Exp::Renderer
{
    constexpr static uint32 VerticesPerQuad = 4;
    constexpr static uint32 IndicesPerQuad = 6;
    constexpr static uint32 MaxQuads = 10000;
    constexpr static uint32 MaxVertices = MaxQuads * VerticesPerQuad;
    constexpr static uint32 MaxIndices = MaxQuads * IndicesPerQuad;
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };
    
    static Shared<VertexArray> QuadVertexArray;
    static Shared<VertexBuffer> QuadVertexBuffer;
    static Shared<Shader> QuadShader;

    static uint32 QuadIndexCount = 0;
    static QuadVertex* QuadVertexBufferBase = nullptr;
    static QuadVertex* QuadVertexBufferPtr = nullptr;
    
    static glm::vec4 QuadVertexPositions[VerticesPerQuad];

    static void SetQuadData()
    {
        if (QuadIndexCount)
        {
            const unsigned int quadDataSize = (uint32)(QuadVertexBufferPtr - QuadVertexBufferBase) * sizeof(QuadVertex);
            QuadVertexBuffer->SetData(QuadVertexBufferBase, quadDataSize);
        }
    }

    static void ResetQuadData()
    {
        QuadIndexCount = 0;
        QuadVertexBufferPtr = QuadVertexBufferBase;
    }
    
    void Init()
    {
        QuadVertexArray = MakeShared<VertexArray>();

        QuadVertexBuffer = MakeShared<VertexBuffer>(MaxVertices * sizeof(QuadVertex));

        const VertexBufferLayout squareLayout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
        };

        QuadVertexBuffer->SetLayout(squareLayout);
        QuadVertexArray->SetVertexBuffer(QuadVertexBuffer);

        QuadVertexBufferBase = new QuadVertex[MaxVertices];

        uint32* quadIndices = new uint32[MaxIndices];

        uint32 offset = 0;
        for (uint32 i = 0; i < MaxIndices; i += IndicesPerQuad)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += VerticesPerQuad;
        }

        const Shared<IndexBuffer> squareIB = MakeShared<IndexBuffer>(quadIndices, MaxIndices);
        QuadVertexArray->SetIndexBuffer(squareIB);
        delete[] quadIndices;

        static const std::filesystem::path quadShaderPath = g_EngineResourcesDirectory / "Shaders" / "QuadShader.glsl";
        QuadShader = MakeShared<Shader>(quadShaderPath);

        QuadVertexPositions[0] = { -.5f, -.5f, 0.f, 1.f };
        QuadVertexPositions[1] = {  .5f, -.5f, 0.f, 1.f };
        QuadVertexPositions[2] = {  .5f,  .5f, 0.f, 1.f };
        QuadVertexPositions[3] = { -.5f,  .5f, 0.f, 1.f };
    }

    void Shutdown()
    {
        delete[] QuadVertexBufferBase;
    }

    void BeginBatch()
    {
        ResetQuadData();
        QuadShader->SetUniformMat4("u_ViewProjection", glm::mat4(1.f));
    }

    void EndBatch()
    {
        SetQuadData();

        QuadShader->Bind();
        RenderAPI::DrawIndexed(QuadVertexArray, QuadIndexCount);
    }

    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position, 0.f), size, color);
    }

    void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        const glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
            * glm::scale(glm::mat4(1.f), { size.x, size.y, 1.f });

        DrawQuad(transform, color);
    }

    void DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        for (uint32 i = 0; i < VerticesPerQuad; ++i)
        {
            QuadVertexBufferPtr->Position = transform * QuadVertexPositions[i];
            QuadVertexBufferPtr->Color = color;
            ++QuadVertexBufferPtr;
        }

        QuadIndexCount += IndicesPerQuad;
    }
}
