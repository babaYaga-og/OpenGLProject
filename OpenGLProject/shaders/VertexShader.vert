#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 modelOffset;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos + modelOffset, 1.0);
}