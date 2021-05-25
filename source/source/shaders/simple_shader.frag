#version 450


layout (location = 0) out vec4 outColor;
layout (location = 0) in vec3 inColor;

layout(push_constant) uniform Push{
 mat2 transform;
 vec2 offset;
 vec3 color;
} push;

void main(){
    outColor = vec4(inColor, 1.0f);
}