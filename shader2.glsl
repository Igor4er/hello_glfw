#version 330 core

layout(location = 0) in vec3 inPos;
out vec3 vertPos;
uniform vec2 offs_xy;



void main(){
  gl_Position.x = inPos.x + offs_xy.x;
  gl_Position.y = inPos.y + offs_xy.y;
  gl_Position.z = inPos.z;
  gl_Position.w = 1.0f;
  vertPos = gl_Position.xyz;
}

