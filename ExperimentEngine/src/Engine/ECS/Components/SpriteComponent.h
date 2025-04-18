#pragma once

namespace Exp
{
    class Texture;
}

namespace Exp
{
    //c
    struct SpriteComponent
    {
        //p draw, save
        glm::vec4 Color = glm::vec4(1.f);
        
        //p draw, save
        Shared<Texture> SpriteTexture = nullptr;

        SpriteComponent() = default;
        SpriteComponent(const glm::vec4& color, const Shared<Texture>& texture)
            : Color(color), SpriteTexture(texture) {}
    };
}
