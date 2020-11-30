#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sg::physics
{
    struct Aabb
    {
        glm::vec2 min{ 0.0f };
        glm::vec2 max{ 0.0f };

        glm::vec2 size{ 0.0f };

        static bool Intersect(const Aabb t_a, const Aabb t_b)
        {
            if (t_a.max.x < t_b.min.x || t_a.min.x > t_b.max.x) return false;
            if (t_a.max.y < t_b.min.y || t_a.min.y > t_b.max.y) return false;

            return true;
        }

        [[nodiscard]] glm::mat4 GetModelMatrix() const noexcept
        {
            auto modelMatrix{ glm::mat4(1.0f) };
            modelMatrix = translate(modelMatrix, glm::vec3(min.x, min.y, 0.0f));
            modelMatrix = scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

            return modelMatrix;
        }
    };
}
