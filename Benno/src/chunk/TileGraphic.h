#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sg::chunk
{
    struct TileGraphic
    {
        int gfxIndex{ 0 };
        int groundHeight{ 0 };

        glm::vec2 screenPosition{ 0.0f };
        glm::vec2 size{ 1.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TileGraphic() = default;

        TileGraphic(const int t_gfxIndex, const int t_groundHeight)
            : gfxIndex{ t_gfxIndex }
            , groundHeight{ t_groundHeight }
        {}

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] glm::mat4 GetModelMatrix() const
        {
            auto modelMatrix{ glm::mat4(1.0f) };

            modelMatrix = translate(modelMatrix, glm::vec3(screenPosition.x, screenPosition.y, 0.0f));
            modelMatrix = scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

            return modelMatrix;
        }
    };
}
