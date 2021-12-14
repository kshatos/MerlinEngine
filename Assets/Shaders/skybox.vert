#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    TexCoords = aPosition;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(aPosition, 1.0);
}  