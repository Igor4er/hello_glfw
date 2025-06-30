#version 410 core
out vec3 color;
in vec3 vertPos;
// uniform vec3 inputColor;

void main(){
    color = vertPos;
}
