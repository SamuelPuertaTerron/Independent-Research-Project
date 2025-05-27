// --- Vertex Shader Start ---
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out vec4 ourColor;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    ourColor = aColor;
}
// --- Vertex Shader End ---

// --- Fragment Shader Start ---
#version 330 core
in vec4 ourColor;
out vec4 FragColor;

void main() {
    FragColor = ourColor;
}
// --- Fragment Shader End ---