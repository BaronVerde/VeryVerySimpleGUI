
/*
 * Build a font atlas, all glyphs sequentially in a row
 * https://gitlab.com/wikibooks-opengl/modern-tutorials/-/blob/master/text02_atlas/text.cpp
 */

#pragma once

#include <stdbool.h>
#include "glad/glad.h"

typedef struct {
	char code;
	float ax;			// advance x
	float ay;			// advance y
	float size_x;		// bitmap width;
	float size_y;		// bitmap height;
	float bearing_x;	// bitmap bearing top left;
	float bearing_y;
	float offset_x;		// x offset of glyph in texture coordinates
	float offset_y;		// y offset of glyph in texture coordinates (multiline, not implemented)
} glyph_info_t;

typedef struct {
	GLuint texture_atlas;
	unsigned int height;
	unsigned int texture_width;
	unsigned int texture_height;
	glyph_info_t glyphs[96];	// starts at 32
} font_info_t;

font_info_t* font_create( const char* const filename, unsigned int height );

void font_render_texture_atlas( const font_info_t* font_info );

void font_delete( font_info_t* font_info );
