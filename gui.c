
#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string.h>
#include "src/gui_window.h"
#include "omath/vec3f.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000
#define MAX_MSG_LEN 256
#define FONT_HEIGHT 14

GLFWwindow* win;

static bool init_graphics();

/* Other fonts:
 * sans variable
 * create_font( "DroidSans.ttf", FONT_HEIGHT );
 * create_font( "KarlaRegular.ttf", FONT_HEIGHT );
 * create_font( "RobotoMedium.ttf", FONT_HEIGHT );
 * sans mono
 * create_font( "CousineRegular.ttf", &cousine_regular );
 * create_font( "ProggyClean.ttf", FONT_HEIGHT );
 * create_font( "ProggyTiny.ttf", FONT_HEIGHT ); */
int main( void ) {
	puts( "Startup ..." );
    if( init_graphics() ) {
    	font_info_t* draw_font = font_create( "fonts/mplus-1c-regular.ttf", FONT_HEIGHT );
    	gui_window_t* gui_window = gui_window_create(
    			"Window data", draw_font, 1.0f, (float)WINDOW_HEIGHT - 1.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT
    	);
    	gui_window_begin( gui_window );
    		gui_window_add_static_text( gui_window, "Framerate:", 1.0f, (float)FONT_HEIGHT + 1.0f );
    		gui_window_add_static_text( gui_window, "Frame #:", 1.0f, 2.0f * ((float)FONT_HEIGHT + 1.0f) );
    		float framerate = 0.0f;
    		gui_window_add_variable( gui_window, gui_float, &framerate, 80.0f, (float)FONT_HEIGHT + 1.0f );
    		unsigned int frame_counter = 0;
    		gui_window_add_variable( gui_window, gui_int, &frame_counter, 80.0f, 2.0f * ((float)FONT_HEIGHT + 1.0f) );
    	gui_window_end( gui_window );

    	glEnable( GL_CULL_FACE );
    	double last_frame = 0.01;
    	puts( "Entering main loop ..." );
    	while( !glfwWindowShouldClose( win ) ) {
    		const double this_frame = glfwGetTime();
    		framerate = 1.0f / (float)( this_frame - last_frame );
    		last_frame = this_frame;
    		glUseProgram( 0 );
    		// Clear the colorbuffer
    		glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
    		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    		//float clear_color[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    		//glClearBufferfv( GL_COLOR, 0, clear_color );
    		vec3f col1 = { 1.0f, 1.0f, 1.0f };
    		gui_window_update( gui_window );
    		gui_window_render( gui_window, &col1 );
    		//render_texture_atlas();
    		frame_counter++;
    		glfwPollEvents();
    		glfwSwapBuffers( win );
    	}
    	puts( "... leaving main loop" );
    	gui_window_delete( gui_window );
    	font_delete( draw_font );
    	glfwDestroyWindow( win );
    	glfwTerminate();
    }
    puts( "... ending" );
    return EXIT_SUCCESS;
}

void error_callback( int error, const char *msg ) {
	char m[MAX_MSG_LEN];
	snprintf( m, MAX_MSG_LEN-1, "[%d] %s", error, msg );
	puts( m );
}

void APIENTRY glDebugOutput( GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, const void *userParam ) {
	// Ignore non-significant error/warning codes
	if( /*id == 131169 ||	// framebuffer storage allocation */
		id == 131185 /*||	// buffer memory usage
		id == 131218 ||	// shader being recompiled
		id == 131204 ||
		id == 6 || id == 7 || id == 8 || id == 9 || id == 10 ||
		id == 11 || id == 12 || id == 13 || id == 14 shader compiler debug messages */ )
		return;
	char m[MAX_MSG_LEN] = "Source: ";
    switch( source ) {
        case GL_DEBUG_SOURCE_API: strcat( m, "API" ); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:	strcat( m, "window system" ); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: strcat( m, "shader compiler" ); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: strcat( m, "third party" ); break;
        case GL_DEBUG_SOURCE_APPLICATION: strcat( m, "application" ); break;
        case GL_DEBUG_SOURCE_OTHER: strcat( m, "other" ); break;
        default: strcat( m, "unknown" );
    }
    strcat( m, "; Type: " );

    switch( type ) {
        case GL_DEBUG_TYPE_ERROR: strcat( m, "error" ); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: strcat( m, "deprecated behaviour" ); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: strcat( m, "undefined behaviour" ); break;
        case GL_DEBUG_TYPE_PORTABILITY: strcat( m, "portability" ); break;
        case GL_DEBUG_TYPE_PERFORMANCE: strcat( m, "performance" ); break;
        case GL_DEBUG_TYPE_MARKER: strcat( m, "marker" ); break;
        case GL_DEBUG_TYPE_PUSH_GROUP: strcat( m, "push group" ); break;
        case GL_DEBUG_TYPE_POP_GROUP: strcat( m, "pop group" ); break;
        case GL_DEBUG_TYPE_OTHER: strcat( m, "other" ); break;
        default: strcat( m, "unknown" );
    }
    strcat( m, "; Severity: " );

    switch( severity ) {
        case GL_DEBUG_SEVERITY_HIGH: strcat( m, "high" ); break;
        case GL_DEBUG_SEVERITY_MEDIUM: strcat( m, "medium" ); break;
        case GL_DEBUG_SEVERITY_LOW: strcat( m, "low" ); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: strcat( m, "notification" ); break;
        default: strcat( m, "unknown" );
    }
    strcat( m, "; " );
    if( strlen( m ) + strlen( message ) >= MAX_MSG_LEN )
    	fputs( "Gl debug output: message too long. Message truncated. Pls. split.", stderr );
    strncat( m, message, MAX_MSG_LEN-1 );
    puts( m );
    // @todo this should be appended to the log
    if( NULL != userParam )
    	puts( "Additional userdata not displayed" );
}

void key_callback( GLFWwindow* win, int key, int scancode, int action, int mods ) {
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( win, GLFW_TRUE );
}

static bool init_graphics() {
	glfwSetErrorCallback( error_callback );
	if( !glfwInit() ) {
		fputs( "glfwInit() faild\n", stderr );
		return false;
	}
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
	win = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Text render", NULL, NULL );
	glfwMakeContextCurrent( win );
	glfwSetKeyCallback( win, key_callback );
	glfwSwapInterval( 1 );
	if( !gladLoadGL() ) {
		fputs( "gladLoadGL() failed\n", stderr );
		glfwDestroyWindow( win );
		glfwTerminate();
		return false;
	}
	GLint flags;
	glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
	if( flags & GL_CONTEXT_FLAG_DEBUG_BIT ) {
		glEnable( GL_DEBUG_OUTPUT );
		// Envoke callback directly in case of error
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( glDebugOutput, NULL );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE );
		puts( "Debug context created." );
	} else
		puts( "Debug context not created. Continuing without debug messages." );
	return true;
}
