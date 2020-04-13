
#pragma once

#include "font.h"
#include "omath/vec3f.h"

// Length of an elemtn in chars
#define MAX_GUI_ELEMENT_LENGTH 64
// Maximum number of static or dynamic elements (total 2*)
#define MAX_GUI_ELEMENTS_PER_WINDOW 8

// Datatypes correspond to float and int.
typedef enum {
	gui_float, gui_int, gui_bool
} gui_variable_datatype_t;

/*typedef struct {
	float pos_x;
	float pos_y;
	float size_x;
	float size_y;
	bool check;
} gui_element_checkbox_t;*/

typedef struct {
	float pos_x;
	float pos_y;
	char text[MAX_GUI_ELEMENT_LENGTH];
} gui_element_static_text_t;

typedef struct {
	float pos_x;
	float pos_y;
	// Do use the right datatypes for variable because the pointer will be cast
	gui_variable_datatype_t datatype;
	void* variable;
} gui_element_variable_t;

typedef struct {
	// Set internally - vertex arrays and buffers for the window
	GLuint vertex_array;
	// Buffer for static elements
	GLsizei num_static_elements;
	gui_element_static_text_t static_elements[MAX_GUI_ELEMENTS_PER_WINDOW];
	GLsizei num_static_vertices;
	GLuint static_vertex_buffer;
	// Buffer for dynamic elements (variables)
	GLsizei num_dynamic_elements;
	gui_element_variable_t dynamic_elements[MAX_GUI_ELEMENTS_PER_WINDOW];
	GLsizei num_dynamic_vertices;
	GLuint dynamic_vertex_buffer;
} gui_window_internals_t;

typedef struct {
	// Set by the application
	// Screen position of gui window and size in pixels
	char title[MAX_GUI_ELEMENT_LENGTH];
	int upper_left_x;
	int upper_left_y;
	// Size of application window for relative placement
	float app_window_size_x;
	float app_window_size_y;
	// bool has_border;		no border for now
	// bool is_transparent;	is transparent for now
	// font for drawing
	const font_info_t* font;
	// Internals. Do not use by application.
	gui_window_internals_t* internals;
} gui_window_t;

/* Begins a new gui window. Expects title, font used for rendering,
 * position upper left in screnn pixels, width and height of the application window in pixels.
 * @todo: projection matrix must be renewed when app. window size changes */
gui_window_t* gui_window_create( const char* title, const font_info_t* font,
		int upper_left, int upper_right, float app_window_size_x, float app_window_size_y );

/* Begin defining elements for the window.
 * w must have been created before */
bool gui_window_begin( const gui_window_t* w );

/* A static text element. It's buffer is only allocated once; it will not change.
 * Element is copied into the window struct.
 * Renders the 0-terminated string with the gui window's font inside of it
 * at the given position. Gui window position in pixels from upper left of window, not screen.
 * Gui window must have been created and begun */
bool gui_window_add_static_text( gui_window_t* w, const char* text, const float pos_x, const float pos_y );

/* A dynamic element for a variable. It's buffer is allocated and filled every frame.
 * Converts variabel name pointer to a string and renders it at given position,
 * data_type determines conversion method (snprintf()).
 * Gui window position in pixels from upper left
 * Gui window must have been created and begun
 * @todo: pass in conversion string */
bool gui_window_add_variable( gui_window_t* w, const gui_variable_datatype_t data_type,
		void* variable_name, const float pos_x, const float pos_y );

/* Ends a begun gui window and calculates buffers and positions of its elements
 * Gui window must have been created and begun */
bool gui_window_end( gui_window_t* w );

/* update the buffer data of variable elements in a gui window
 * Gui window must have been created and begun */
bool gui_window_update( gui_window_t* w );

/* set scissors and draw call;
  Gui window must have been ended */
void gui_window_render( gui_window_t* w, const vec3f* color );

/* Deletes a creates gui window and cleans up
 * Gui window must have been created */
void gui_window_delete( gui_window_t* w );

/*
 * Window:
 * static buffer, calculate storage
 * dynamic buffer
 * static elements (number, number of vertices, buffer offsets, window positions)
 * dynamic elements (number, number of vertices, buffer offsets, window positions)
 * formats as in C -> to string ?
 */
