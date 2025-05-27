#include "Engine.h"
#include "OpenGLCamera.h"

#include "Engine/Game/Chunk.h"

namespace Engine::Render::OpenGL
{
    void OpenGLCamera::UpdateTransform(const Vector3& position, const Vector3& rotation)
    {
        // Projection matrix (this can often be set once unless window resizes)
        const float fov = 45.0f;
        const float aspectRatio = 16.0f / 9.0f;
        const float nearPlane = 0.1f;
        const float farPlane = 1000.0f;

        m_ProjectMatrix = glm::perspective(
            glm::radians(fov),
            aspectRatio,
            nearPlane,
            farPlane
        );

        // View matrix
        glm::vec3 cameraPos = glm::vec3(position.X, position.Y, position.Z);

        // Calculate direction based on rotation (pitch, yaw, roll)
        glm::vec3 direction;
        direction.x = cos(glm::radians(rotation.Y)) * cos(glm::radians(rotation.X));
        direction.y = sin(glm::radians(rotation.X));
        direction.z = sin(glm::radians(rotation.Y)) * cos(glm::radians(rotation.X));
        glm::vec3 cameraTarget = cameraPos + glm::normalize(direction);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        m_ViewMatrix = glm::lookAt(
            cameraPos,
            cameraTarget,
            up
        );
    }
}//namespace Engine::Render::OpenGL