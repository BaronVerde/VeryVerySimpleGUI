
#version 450 core

in vec2 tex_coords;
out vec4 color;

layout( binding = 0 ) uniform sampler2D texture_atlas;
uniform vec3 pen_color;

void main() {
	color = vec4( 1.0f, 1.0f, 1.0f, texture2D( texture_atlas, tex_coords ).r ) * vec4( pen_color, 1.0f );
}
