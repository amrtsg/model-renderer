#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 clipPlane;

uniform vec3 lightPosition;

uniform float density;
uniform float gradient;

void main( )
{
    vec4 worldPosition = model * vec4(position, 1.0f);
    vec4 positionRelativeToCam = view * worldPosition;
    gl_ClipDistance[0] = dot(worldPosition, clipPlane);
    gl_Position = projection * positionRelativeToCam;

    float distance = length(positionRelativeToCam.xyz);
    TexCoords = texCoords;
}