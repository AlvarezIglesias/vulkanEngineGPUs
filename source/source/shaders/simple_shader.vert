#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;
layout(push_constant) uniform Push{
 mat2 transform;
 vec2 offset;
 vec3 color;
 mat4 model;
 mat4 view;
 mat4 proj;
} push;

void main(){
    gl_Position = vec4(push.proj * push.view * push.model * vec4((position) ,1));
    fragColor = color;
}