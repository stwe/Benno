#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sg::physics
{
    struct Aabb
    {
        glm::vec2 position{ 0.0f };
        glm::vec2 size{ 0.0f };

        static bool PointVsAabb(const glm::vec2& t_point, const Aabb& t_aabb)
        {
            return
                t_point.x >= t_aabb.position.x &&
                t_point.y >= t_aabb.position.y &&
                t_point.x < t_aabb.position.x + t_aabb.size.x &&
                t_point.y < t_aabb.position.y + t_aabb.size.y;
        }

        static bool AabbVsAabb(const Aabb& t_a, const Aabb& t_b)
        {
            return
                t_a.position.x < t_b.position.x + t_b.size.x &&
                t_a.position.x + t_a.size.x > t_b.position.x &&
                t_a.position.y < t_b.position.y + t_b.size.y &&
                t_a.position.y + t_a.size.y > t_b.position.y;
        }

        [[nodiscard]] glm::mat4 GetModelMatrix() const noexcept
        {
            auto modelMatrix{ glm::mat4(1.0f) };
            modelMatrix = translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f));
            modelMatrix = scale(modelMatrix, glm::vec3(size.x, size.y, 1.0f));

            return modelMatrix;
        }
    };
}
