
#version 450 core

// .x/.y = screen- and .z/.w = texture coords
layout( location = 0 ) in vec4 vertex;
out vec2 tex_coords;

uniform mat4 projection;
uniform int buffer_number;

void main() {	
	gl_Position = projection * vec4( vertex.xy, 0.0, 1.0 );
	tex_coords = vertex.zw;
}
