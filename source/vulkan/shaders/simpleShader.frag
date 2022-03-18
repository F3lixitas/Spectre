#version 450

//layout(location = 0) in float index;
layout(location = 0) in vec3 color; 
layout(location = 1) in vec2 UV;
layout (location = 0) out vec4 outColor;
//layout(binding = 0) uniform sampler2D tex;
void main(){
    
    //outColor = texture(tex, UV);
    outColor = vec4(color, 1.0f);
}