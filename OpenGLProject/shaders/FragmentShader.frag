#version 330 core
out vec4 FragColour;

uniform vec4 modelColour;

void main() {
    FragColour = modelColour;
}