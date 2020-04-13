#include "gui_window.h"
#include "shader_program.h"
#include "omath/vec4f.h"
#include "omath/mat4f.h"
#include <stdio.h>
#include <string.h>	// memset()

static GLuint shader_program;

static bool glyph_screen_coords( vec4f* buffer, GLsizei* index, const char* restrict text,
		const font_info_t* restrict font, float position_x, float position_y );

gui_window_t* gui_window_create( const char* title, const font_info_t* font,
		int upper_left_x, int upper_left_y, float app_window_size_x, float app_window_size_y ) {
	// @todo: validity checks
	gui_window_t* w = malloc( sizeof( gui_window_t ) );
	// Create shader only once
	if( NULL != w ) {
		w->internals = malloc( sizeof( gui_window_internals_t ) );
		if( NULL == w->internals ) {
			free( w );
			w = NULL;
		} else {
			if( !glIsProgram( shader_program ) )
				shader_program_create( &shader_program, "src/glyph_shader.vs", "src/glyph_shader.fs" );
			strncpy( w->title, title, MAX_GUI_ELEMENT_LENGTH );
			w->font = font;
			w->upper_left_x = upper_left_x;
			w->upper_left_y = upper_left_y;
			w->app_window_size_x = app_window_size_x;
			w->app_window_size_y = app_window_size_y;
		}
	}
	return w;
}

bool gui_window_begin( const gui_window_t* w ) {
	// @todo: projection matrix must be renewed when app. window size changes
	mat4f projection;
	mat4f_ortho( &projection, 0.0f, w->app_window_size_x, 0.0f, w->app_window_size_y, 0.0f, 1.0f );
	glUseProgram( shader_program );
	glUniformMatrix4fv( glGetUniformLocation( shader_program, "projection"), 1, GL_FALSE, &projection.data[0] );

	gui_window_internals_t* i = w->internals;
	// Configure vertex array and buffers
	glCreateVertexArrays( 1, &(i->vertex_array) );
	// Only one vec4f attrib: .xy = coords, .zw = texture coords from font atlas
	glVertexArrayAttribFormat( i->vertex_array, 0, 4, GL_FLOAT, GL_FALSE, 0 );
	glEnableVertexArrayAttrib( i->vertex_array, 0 );
	glCreateBuffers( 1, &(i->static_vertex_buffer) );
	glCreateBuffers( 1, &(i->dynamic_vertex_buffer) );

	// Set counters and init element arrays
	i->num_static_elements = 0;
	i->num_static_vertices = 0;
	i->num_dynamic_elements = 0;
	i->num_dynamic_vertices = 0;
	memset( &(i->static_elements[0]), 0, sizeof( i->static_elements ) );
	memset( &(i->dynamic_elements[0]), 0, sizeof( i->dynamic_elements ) );

	return true;
}

/* A static text element. Element is copied into the window struct.
 * Renders the 0-terminated string with the gui window's font inside of it
 * at the given position. Gui window position in pixels from upper left */
bool gui_window_add_static_text( gui_window_t* w, const char* text, const float pos_x, const float pos_y ) {
	const size_t len = strlen( text );
	if( 0 == len || MAX_GUI_ELEMENT_LENGTH-1 <= len || NULL == text ) {
		fputs( "Gui window element text empty or too long\n", stderr );
		return false;
	}
	gui_window_internals_t* i = w->internals;
	if( MAX_GUI_ELEMENTS_PER_WINDOW <= i->num_static_elements ) {
		fputs( "Maximum number of gui elements per window reached\n", stderr );
		return false;
	}
	i->static_elements[i->num_static_elements].pos_x = pos_x;
	i->static_elements[i->num_static_elements].pos_y = pos_y;
	strncpy( &(i->static_elements[i->num_static_elements].text[0]), text, len );
	++i->num_static_elements;
	i->num_static_vertices += (GLsizei)( 6 * len );
	return true;
}

bool gui_window_add_variable( gui_window_t* w, const gui_variable_datatype_t data_type,
		void* variable_name, const float pos_x, const float pos_y ) {
	gui_window_internals_t* i = w->internals;
	if( MAX_GUI_ELEMENTS_PER_WINDOW <= i->num_dynamic_elements ) {
		fputs( "Maximum number of gui elements per window reached\n", stderr );
		return false;
	}
	// nothing to memcpy in this struct
	i->dynamic_elements[i->num_dynamic_elements].datatype = data_type;
	i->dynamic_elements[i->num_dynamic_elements].variable = variable_name;
	i->dynamic_elements[i->num_dynamic_elements].pos_x = pos_x;
	i->dynamic_elements[i->num_dynamic_elements].pos_y = pos_y;
	++i->num_dynamic_elements;
	return true;
}

// update the buffer data of variable elements;
bool gui_window_update( gui_window_t* w ) {
	gui_window_internals_t* in = w->internals;
	in->num_dynamic_vertices = 0;
	vec4f* buf = glMapNamedBuffer( in->dynamic_vertex_buffer, GL_WRITE_ONLY );
	if( NULL == buf ) {
		fputs( "Error mapping buffer for dynamic gui data\n", stderr );
		return false;
	}
	// temporary buffer for an element string
	GLsizei idx = 0;
	char to_display[MAX_GUI_ELEMENT_LENGTH];
	for( int i = 0; i < in->num_dynamic_elements; ++i ) {
		const gui_element_variable_t* e = &(in->dynamic_elements[i]);
		switch( e->datatype ) {
			case gui_float:
				snprintf( &to_display[0], MAX_GUI_ELEMENT_LENGTH, "%7.2f", *(float*)e->variable );
				break;
			case gui_int:
				snprintf( &to_display[0], MAX_GUI_ELEMENT_LENGTH, "%9d", *(int*)e->variable );
				break;
			default:
				fputs( "Unknown datatype in gui variable\n", stderr );
		}
		glyph_screen_coords( buf, &idx, to_display, w->font,
				(float)w->upper_left_x + e->pos_x, (float)w->upper_left_y - e->pos_y );
		in->num_dynamic_vertices += idx;
	}
	if( GL_TRUE != glUnmapNamedBuffer( in->dynamic_vertex_buffer ) )
		fputs( "Error unmapping gui dynamic buffer. Data corruption ?\n", stderr );
	return true;
}

bool gui_window_end( gui_window_t* w ) {
	gui_window_internals_t* in = w->internals;
	// calculate screen positions and texture coordinates for all window elements
	// x, y, s, t - screen position x/y, texture atlas position s/t
	const size_t buffer_size = (size_t)in->num_static_vertices * sizeof( vec4f );
	// temporary buffer
	vec4f* buf = malloc( buffer_size );
	GLsizei idx = 0;
	for( int i = 0; i < in->num_static_elements; ++i ) {
		const gui_element_static_text_t* e = &(in->static_elements[i]);
		// OpenGL has 0/0 in the lower left corner
		glyph_screen_coords( buf, &idx, e->text, w->font,
				(float)w->upper_left_x + e->pos_x, (float)w->upper_left_y - e->pos_y );
	}
	// Update content of static buffer. Dynamic buffer is updated in gui_window_update()
	glNamedBufferData( in->static_vertex_buffer, (GLsizeiptr)buffer_size, buf, GL_STATIC_DRAW );
	free( buf );
	// Generously grant a maximum of MAX_GUI_ELEMENT_LENGTH per dynamic element
	const GLsizeiptr s = in->num_dynamic_elements * MAX_GUI_ELEMENT_LENGTH * 6 * (int)sizeof( vec4f );
	glNamedBufferStorage( in->dynamic_vertex_buffer, s, NULL, GL_MAP_WRITE_BIT );
	return true;
}

// set scissors and draw call;
void gui_window_render( gui_window_t* w, const vec3f* color ) {
	gui_window_internals_t* i = w->internals;
	// @todo glViewport(); glScissor()
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glUseProgram( shader_program );
	glBindTextureUnit( 0, w->font->texture_atlas );
	glUniform3f( glGetUniformLocation( shader_program, "pen_color" ), color->x, color->y, color->z );
	// draw static and dynamic buffer
	glBindVertexArray( i->vertex_array );
	glVertexArrayVertexBuffer( i->vertex_array, 0, i->static_vertex_buffer, 0, sizeof( vec4f ) );
	glDrawArrays( GL_TRIANGLES, 0, i->num_static_vertices );
	glVertexArrayVertexBuffer( i->vertex_array, 0, i->dynamic_vertex_buffer, 0, sizeof( vec4f ) );
	glDrawArrays( GL_TRIANGLES, 0, i->num_dynamic_vertices );
}

void gui_window_delete( gui_window_t* w ) {
	const gui_window_internals_t* i = w->internals;
	if( glIsBuffer( i->dynamic_vertex_buffer ) )
		glDeleteBuffers( 1, &(i->dynamic_vertex_buffer) );
	if( glIsBuffer( i->static_vertex_buffer ) )
		glDeleteBuffers( 1, &(i->static_vertex_buffer) );
	if( glIsVertexArray( i->vertex_array ) )
		glDeleteVertexArrays( 1, &(i->vertex_array) );
	// @todo: last window deletes shader porgram !
	if( glIsProgram( shader_program ) )
		shader_program_delete( shader_program );
	if( NULL != w->internals )
		free( w->internals );
	if( NULL != w )
		free( w );
}

/* Iterates over the chars in text and fills buffer with vec4f.
 * The buffer contents are changed, and the current index into the buffer is returned.
 * The screen positions of the first character in pixels, lower left of the char must
 * be given in the position parameters */
static bool glyph_screen_coords(
		vec4f* buffer, GLsizei* index, const char* restrict text, const font_info_t* restrict font,
		float position_x, float position_y ) {
	for( const char* p = text; *p; ++p ) {
		// Screen position of this glyph
		const glyph_info_t* g = &(font->glyphs[((int)(*p))-32]);	// index cast to int to avoid warning
		const float x2 = position_x + g->bearing_x;
		const float y2 = position_y - ( g->size_y - g->bearing_y );
		// Skip glyphs that have no bitmap, but advance the cursor
		position_x += g->ax;
		position_y -= g->ay;
		if( 0 == g->size_x || 0 == g->size_y )
			continue;
		const float x_min = g->offset_x;
		const float y_min = g->offset_y;
		const float x_max = g->offset_x + g->size_x / (float)font->texture_width;
		const float y_max = g->offset_y + g->size_y / (float)font->texture_height;
		vec4f_set( &buffer[(*index)++], x2,				y2 + g->size_y,	x_min, y_min );
		vec4f_set( &buffer[(*index)++], x2,				y2,				x_min, y_max );
		vec4f_set( &buffer[(*index)++], x2 + g->size_x,	y2,				x_max, y_max );
		vec4f_set( &buffer[(*index)++], x2,				y2 + g->size_y,	x_min, y_min );
		vec4f_set( &buffer[(*index)++], x2 + g->size_x,	y2,				x_max, y_max );
		vec4f_set( &buffer[(*index)++], x2 + g->size_x,	y2 + g->size_y,	x_max, y_min );
	}
	return true;
}
