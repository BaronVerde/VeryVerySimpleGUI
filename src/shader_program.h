
#pragma once

#include "glad/glad.h"
#include <stdbool.h>

bool shader_program_create( GLuint* out_program, const char* vertex_shader_file, const char* fragment_shader_file );

void shader_program_delete( GLuint program );
