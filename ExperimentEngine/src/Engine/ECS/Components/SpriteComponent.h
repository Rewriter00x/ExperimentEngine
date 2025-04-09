#pragma once

namespace Exp
{
    class Texture;
}

namespace Exp
{
    struct SpriteComponent
    {
        glm::vec4 Color = glm::vec4(1.f);
        Shared<Texture> SpriteTexture = nullptr;

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color, const Shared<Texture>& texture)
            : Color(color), SpriteTexture(texture) {}
    };
}
