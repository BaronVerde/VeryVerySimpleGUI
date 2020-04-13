
#include "shader_program.h"
#include <stdlib.h>
#include <stdio.h>

static bool shader_read_source_file( GLchar** out_source, const char* filename );
static bool shader_compile( const GLuint shader, const GLchar* shader_source );

bool shader_program_create( GLuint* out_program, const char* vertex_shader_file, const char* fragment_shader_file ) {
	// loading and compilation
	GLchar *source_code = NULL;
	if( !shader_read_source_file( &source_code, vertex_shader_file ) ) {
		fprintf( stderr, "Error reading vertex shader source file '%s'\n", vertex_shader_file );
		return false;
	}
	GLuint vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	printf( "Compiling shader '%s'\n", vertex_shader_file );
	if( !shader_compile( vertex_shader, source_code ) ) {
		free( source_code );
		glDeleteShader( vertex_shader );
		return false;
	}
	free( source_code );
	source_code = NULL;
	// Source assumed to be null-terminated, see read function below
	if( !shader_read_source_file( &source_code, fragment_shader_file ) ) {
		fprintf( stderr, "Error reading fragment shader source file '%s'\n", vertex_shader_file );
		glDeleteShader( vertex_shader );
		return false;
	};
	// Source assumed to be null-terminated, see read function below
	GLuint fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	printf( "Compiling shader '%s'\n", fragment_shader_file );
	if( !shader_compile( fragment_shader, source_code ) ) {
		free( source_code );
		glDeleteShader( vertex_shader );
		glDeleteShader( fragment_shader );
		return false;
	}
	free( source_code );
	// attaching to program
	*out_program = glCreateProgram();
	if( !glIsProgram( *out_program ) ) {
		fputs( "Error creating shader program. Is not a program\n", stderr );
		glDeleteShader( vertex_shader );
		glDeleteShader( fragment_shader );
		return false;
	}
	glAttachShader( *out_program, vertex_shader );
	printf( "Attaching vertex shader '%s' #%d\n", vertex_shader_file, vertex_shader );
	glAttachShader( *out_program, fragment_shader );
	printf( "Attaching fragment shader '%s' #%d\n", fragment_shader_file, fragment_shader );
	// Linking
	glLinkProgram( *out_program );
	GLint linked;
	glGetProgramiv( *out_program, GL_LINK_STATUS, &linked );
	if( GL_TRUE != linked ) {
		GLint len;
		glGetProgramiv( *out_program, GL_INFO_LOG_LENGTH, &len );
		GLchar *log = malloc( sizeof(GLchar) * (size_t)(len + 1 ) );
		glGetProgramInfoLog( *out_program, len, &len, log );
		fprintf( stderr, "Shader linkage failed: '%s'\n", log );
		free( log );
		glDeleteProgram( *out_program );
		glDeleteShader( vertex_shader );
		glDeleteShader( fragment_shader );
		return false;
	}
	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );
	printf( "Shader program #%ud linked. Ready for use\n", *out_program );
	return true;
}

void shader_program_delete( GLuint program ) {
	if( glIsProgram( program ) )
		glDeleteProgram( program );
	printf( "Shader program #%d destroyed\n", program );
}

// Pass NULL pointer and don't forget to free returned pointer in calling routine !
static bool shader_read_source_file( GLchar** out_source, const char* filename ) {
	FILE* shader_file = fopen( filename, "r" );
	if( !shader_file ) {
		fprintf( stderr, "Error opening shader file '%s'\n", filename );
		return false;
	}
	fseek( shader_file, 0, SEEK_END );
	long int file_length = ftell( shader_file );
	size_t file_size = (size_t)file_length;
	if( file_size > 1 ) {
		fseek( shader_file, 0, SEEK_SET );
		// +1 for trailing \0
		*out_source = malloc( sizeof(GLchar) * ( file_size + 1 ) );
		if( 1 == fread( *out_source, file_size, 1, shader_file ) ) {
			// Just to be sure
			(*out_source)[file_size] = '\0';
			printf( "Shader file '%s' successfully read\n", filename );
			return true;
		}
	}
	fprintf( stderr, "Error reading contents of shader file '%s'\n", filename );
	fclose( shader_file );
	if( NULL != *out_source )
		free( *out_source );
	return false;
}

static bool shader_compile( const GLuint shader, const GLchar* shader_source ) {
	const char* vsp = shader_source;
	glShaderSource( shader, 1, &vsp, NULL );
	glCompileShader( shader );
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if( !compiled ) {
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );
		GLchar *log = malloc( (size_t)(len + 1) * sizeof(GLchar) );
		glGetShaderInfoLog( shader, len, &len, log );
		fprintf( stderr, "Shader '%u' compilation failed: %s\n", shader, log );
		free( log );
		glDeleteShader( shader );
		return false;
	}
	return true;
}
