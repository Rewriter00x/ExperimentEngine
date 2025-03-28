#include "exppch.h"
#include "Renderer.h"

#include "Camera.h"
#include "RenderAPI.h"

#include "RenderData/VertexArray.h"
#include "RenderData/VertexBuffer.h"
#include "RenderData/IndexBuffer.h"
#include "RenderData/Shader.h"
#include "RenderData/Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Exp::Renderer
{
    constexpr static uint32 VerticesPerQuad = 4;
    constexpr static uint32 IndicesPerQuad = 6;
    constexpr static uint32 MaxQuads = 10000;
    constexpr static uint32 MaxVertices = MaxQuads * VerticesPerQuad;
    constexpr static uint32 MaxIndices = MaxQuads * IndicesPerQuad;
    constexpr static uint32 MaxTextureSlots = 16;
    
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        int TexIndex;
    };
    
    static Shared<VertexArray> QuadVertexArray;
    static Shared<VertexBuffer> QuadVertexBuffer;
    static Shared<Shader> QuadShader;

    static uint32 QuadIndexCount = 0;
    static QuadVertex* QuadVertexBufferBase = nullptr;
    static QuadVertex* QuadVertexBufferPtr = nullptr;
    
    static glm::vec4 QuadVertexPositions[VerticesPerQuad];
    static glm::vec2 QuadTexturePositions[VerticesPerQuad];
    
    static Shared<Texture> WhiteTexture;
    static constexpr uint32 WhiteTextureIndex = 0;

    static std::array<Shared<Texture>, MaxTextureSlots> TextureSlots;
    static uint32 TextureSlotIndex = 1;

    static void SetTextureData()
    {
        for (uint32 i = 0; i < TextureSlotIndex; ++i)
        {
            TextureSlots[i]->Bind(i);
        }
    }

    static void ResetTextureData()
    {
        TextureSlotIndex = 1;
    }

    static void SetQuadData()
    {
        if (QuadIndexCount)
        {
            const uint32 quadDataSize = (uint32)(QuadVertexBufferPtr - QuadVertexBufferBase) * sizeof(QuadVertex);
            QuadVertexBuffer->SetData(QuadVertexBufferBase, quadDataSize);
        }
    }

    static void ResetQuadData()
    {
        QuadIndexCount = 0;
        QuadVertexBufferPtr = QuadVertexBufferBase;
    }

    static void SetBatchData()
    {
        SetQuadData();
        SetTextureData();
    }

    static void ResetBatchData()
    {
        ResetQuadData();
        ResetTextureData();
    }

    static void FlushAndReset()
    {
        EndBatch();
        ResetBatchData();
    }

    static bool CheckBatchIndicesSize()
    {
        if (QuadIndexCount + IndicesPerQuad > MaxIndices)
        {
            FlushAndReset();
            return true;
        }
        return false;
    }

    static bool CheckBatchTexturesSize()
    {
        if (TextureSlotIndex + 1 > MaxTextureSlots)
        {
            FlushAndReset();
            return true;
        }
        return false;
    }

    static uint32 FindOrAddTextureSlot(const Shared<Texture>& texture)
    {
        uint32 textureIndex = 0;

        for (uint32 i = 1; i < TextureSlotIndex; i++)
        {
            if (*TextureSlots[i] == *texture)
            {
                textureIndex = i;
                break;
            }
        }

        if (textureIndex == 0)
        {
            CheckBatchTexturesSize();
            textureIndex = TextureSlotIndex++;
            TextureSlots[textureIndex] = texture;
        }

        return textureIndex;
    }
    
    void Init()
    {
        QuadVertexArray = MakeShared<VertexArray>();

        QuadVertexBuffer = MakeShared<VertexBuffer>(MaxVertices * sizeof(QuadVertex));

        const VertexBufferLayout squareLayout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Int, "a_TexIndex" },
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
        AssetManager::ReleaseAssetData(quadShaderPath);
        
        WhiteTexture = MakeShared<Texture>(1, 1);
        constexpr uint32 whiteTextureData = 0xffffffff;
        WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));
        TextureSlots[WhiteTextureIndex] = WhiteTexture;

        int32 samplers[MaxTextureSlots];
        for (int32 i = 0; i < (int32)MaxTextureSlots; ++i)
        {
            samplers[i] = i;
        }
        QuadShader->Bind();
        QuadShader->SetUniformIntArray("u_Textures", samplers, MaxTextureSlots);

        QuadVertexPositions[0] = { -.5f, -.5f, 0.f, 1.f };
        QuadVertexPositions[1] = {  .5f, -.5f, 0.f, 1.f };
        QuadVertexPositions[2] = {  .5f,  .5f, 0.f, 1.f };
        QuadVertexPositions[3] = { -.5f,  .5f, 0.f, 1.f };

        QuadTexturePositions[0] = { 0.f, 0.f };
        QuadTexturePositions[1] = { 1.f, 0.f };
        QuadTexturePositions[2] = { 1.f, 1.f };
        QuadTexturePositions[3] = { 0.f, 1.f };
    }

    void Shutdown()
    {
        delete[] QuadVertexBufferBase;
    }

    void BeginBatch(const Camera& camera)
    {
        ResetBatchData();
        
        QuadShader->Bind();
        QuadShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjection());
    }

    void EndBatch()
    {
        SetBatchData();

        QuadShader->Bind();
        RenderAPI::DrawIndexed(QuadVertexArray, QuadIndexCount);
    }

    void DrawQuad(const QuadData& data)
    {
        CheckBatchIndicesSize();

        glm::mat4 transform = glm::translate(glm::mat4(1.f), data.Position);

        if (data.Rotation != glm::vec3(0.f))
        {
            transform *= glm::toMat4(glm::quat(glm::radians(data.Rotation)));
        }

        transform *= glm::scale(glm::mat4(1.f), { data.Size.x, data.Size.y, 1.f });

        const uint32 textureIndex = data.Texture ? FindOrAddTextureSlot(data.Texture) : WhiteTextureIndex;

        for (uint32 i = 0; i < VerticesPerQuad; ++i)
        {
            QuadVertexBufferPtr->Position = transform * QuadVertexPositions[i];
            QuadVertexBufferPtr->Color = data.Color;
            QuadVertexBufferPtr->TexCoord = QuadTexturePositions[i];
            QuadVertexBufferPtr->TexIndex = textureIndex;
            ++QuadVertexBufferPtr;
        }

        QuadIndexCount += IndicesPerQuad;
    }
}
