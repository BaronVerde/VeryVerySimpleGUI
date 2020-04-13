
#include "font.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include "omath/vec4f.h"
#include "shader_program.h"

static FT_Library ft = NULL;
static FT_Face face = NULL;

static bool font_init_and_check( const char* filename );
static void font_cleanup();

/* https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02
 * and https://learnopengl.com/code_viewer.php?code=in-practice/text_rendering */
font_info_t* font_create( const char* const filename, unsigned int height ) {
	if( font_init_and_check( filename ) ) {
		font_info_t* font_info = malloc( sizeof( font_info_t ) );
		if( NULL != font_info ) {
			font_info->height = height;
			if( height < 6 || height > 36 ) {
				fputs( "Font height must be [6-36], for now\n", stderr );
				return false;
			}
			// @todo check return code 0 == success
			FT_Set_Pixel_Sizes( face, 0, font_info->height );
			FT_GlyphSlot g = face->glyph;
			memset( font_info->glyphs, 0, sizeof( font_info->glyphs ) );
			font_info->texture_width = font_info->texture_height = 0;
			// Pass 1: determine total width and height of texture
			for( unsigned int i = 32; i < 128; ++i ) {
				if( FT_Load_Char( face, i, FT_LOAD_RENDER ) )
					continue;
				font_info->texture_width += g->bitmap.width;
				// Multiline not implemented, all glyphs are in one row. That limits the size of the glyphs
				font_info->texture_height =
						font_info->texture_height > g->bitmap.rows ? font_info->texture_height : g->bitmap.rows;
			}
			// Disable byte-alignment restriction to store the texture
			GLint upa;
			glGetIntegerv( GL_UNPACK_ALIGNMENT, &upa );
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			printf( "Loading font '%s'\n", filename );
			// Create font atlas and set default texture parameters
			glCreateTextures( GL_TEXTURE_2D, 1, &font_info->texture_atlas );
			glTextureParameteri( font_info->texture_atlas, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTextureParameteri( font_info->texture_atlas, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTextureParameteri( font_info->texture_atlas, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTextureParameteri( font_info->texture_atlas, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTextureStorage2D(
					font_info->texture_atlas, 1, GL_R8, (GLsizei)font_info->texture_width, (GLsizei)font_info->texture_height
			);
			// Pass 2 : load glyphs at current texture offset position, y for multiline (not implemented)
			int offset_x = 0;
			int offset_y = 0;
			for( GLubyte i = 32; i < 128; ++i ) {
				if( FT_Load_Char( face, i, FT_LOAD_RENDER ) ) {
					fprintf( stderr, "Failed to load glyph #%d, char '%c'\n", i, i );
					continue;
				}
				int idx = i - 32;
				//printf( "Loading glyph %d\t'%c'\n", i, i );
				glTextureSubImage2D( font_info->texture_atlas, 0, offset_x, 0/*offset_y*/,
						(GLsizei)g->bitmap.width, (GLsizei)g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer );
				font_info->glyphs[idx].code = (char)i;
				font_info->glyphs[idx].ax = (float)(g->advance.x >> 6);
				font_info->glyphs[idx].ay = (float)(g->advance.y >> 6);
				font_info->glyphs[idx].size_x = (float)g->bitmap.width;
				font_info->glyphs[idx].size_y = (float)g->bitmap.rows;
				font_info->glyphs[idx].bearing_x = (float)g->bitmap_left;
				font_info->glyphs[idx].bearing_y = (float)g->bitmap_top;
				font_info->glyphs[idx].offset_x = (float)offset_x / (float)font_info->texture_width;
				font_info->glyphs[idx].offset_y = (float)offset_y / (float)font_info->texture_height;
				// Offset for next glyph
				offset_x += (int)g->bitmap.width;
			}
			// cleanup
			glPixelStorei( GL_UNPACK_ALIGNMENT, upa );
			font_cleanup();
			// positive outcome
			return font_info;
		}
	}
	font_cleanup();
	return NULL;
}

// If font is ok cleanup() must be called by caller, else cleans up and returns false
static bool font_init_and_check( const char* filename ) {
	bool ok = true;
	if( FT_Init_FreeType( &ft ) ) {
		fputs(" Could not init FreeType Library\n", stderr );
		ok = false;
	}
	if( ok && FT_New_Face( ft, filename, 0, &face ) ) {
		fprintf( stderr, "Failed to load font face from '%s'\n", filename );
		ok = false;
	}
	if( ok && !face->face_flags & FT_FACE_FLAG_SCALABLE ) {
		fprintf( stderr, "Font '%s' is not scalable\n", filename );
		ok = false;
	}
	if( ok && NULL == face->charmap ) {
		fprintf( stderr, "Font '%s' seems to have no unicode charmap\n", filename );
		ok = false;
	}
	if( !ok )
		font_cleanup();
	return ok;
}

static void font_cleanup() {
	if( face )
		FT_Done_Face( face );
	if( ft )
		FT_Done_FreeType( ft );
	face = NULL;
	ft = NULL;
}

void font_delete( font_info_t* font_info ) {
	if( glIsTexture( font_info->texture_atlas ) )
		glDeleteTextures( 1, &font_info->texture_atlas );
	if( NULL != font_info )
		free( font_info );
}
