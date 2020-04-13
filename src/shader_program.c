
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

/*

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

*/

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
