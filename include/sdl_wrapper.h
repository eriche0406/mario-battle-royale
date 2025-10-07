#ifndef __SDL_WRAPPER_H__
#define __SDL_WRAPPER_H__

#include "color.h"
#include "list.h"
#include "polygon.h"
#include "scene.h"
#include "state.h"
#include "vector.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>

// Values passed to a key handler when the given arrow key is pressed
typedef enum {
  LEFT_ARROW = 1,
  UP_ARROW = 2,
  RIGHT_ARROW = 3,
  DOWN_ARROW = 4,
  SPACE_BAR = 5,
  W_KEY = 6,
  A_KEY = 7,
  S_KEY = 8,
  D_KEY = 9,
  F_KEY = 10,
  RSHIFT_KEY = 11
} arrow_key_t;

/**
 * The possible types of key events.
 * Enum types in C are much more primitive than in Java; this is equivalent to:
 * typedef unsigned int KeyEventType;
 * #define KEY_PRESSED 0
 * #define KEY_RELEASED 1
 */
typedef enum { KEY_PRESSED, KEY_RELEASED } key_event_type_t;

/**
 * A keypress handler.
 * When a key is pressed or released, the handler is passed its char value.
 * Most keys are passed as their char value, e.g. 'a', '1', or '\r'.
 * Arrow keys have the special values listed above.
 *
 * @param key a character indicating which key was pressed
 * @param type the type of key event (KEY_PRESSED or KEY_RELEASED)
 * @param held_time if a press event, the time the key has been held in seconds
 */
typedef void (*key_handler_t)(char key, key_event_type_t type, double held_time,
                              void *state);
/**
 * A mousepress handler.
 * When a mouse is pressed or released, the handler is passed its value.
 *
 * @param state a character indicating which state was the game
 * @param x where the mouse is clicked - x location
 * @param y where the mouse is clicked - y location
 */
typedef void (*mouse_handler_t)(state_t *state, double x, double y);
/**
 * Initializes the SDL window and renderer.
 * Must be called once before any of the other SDL functions.
 *
 * @param min the x and y coordinates of the bottom left of the scene
 * @param max the x and y coordinates of the top right of the scene
 */
void sdl_init(vector_t min, vector_t max);

/**
 * Processes all SDL events and returns whether the window has been closed.
 * This function must be called in order to handle inputs.
 *
 * @return true if the window was closed, false otherwise
 */
bool sdl_is_done(void *state);

/**
 * Clears the screen. Should be called before drawing polygons in each frame.
 */
void sdl_clear(void);

/**
 * Draws a polygon from the given list of vertices and a color.
 *
 * @param poly a struct representing the polygon
 * @param color the color used to fill in the polygon
 */
void sdl_draw_polygon(polygon_t *poly, rgb_color_t color);

/**
 * Displays the rendered frame on the SDL window.
 * Must be called after drawing the polygons in order to show them.
 */
void sdl_show(void);

/**
 * Draws all bodies in a scene.
 * This internally calls sdl_clear(), sdl_draw_polygon(), and sdl_show(),
 * so those functions should not be called directly.
 *
 * @param scene the scene to draw
 * @param aux an additional body to draw (can be NULL if no additional bodies)
 */
void sdl_render_scene(scene_t *scene, void *aux);

/**
 * Registers a function to be called every time a key is pressed.
 * Overwrites any existing handler.
 *
 * Example:
 * ```
 * void on_key(char key, key_event_type_t type, double held_time) {
 *     if (type == KEY_PRESSED) {
 *         switch (key) {
 *             case 'a':
 *                 printf("A pressed\n");
 *                 break;
 *             case UP_ARROW:
 *                 printf("UP pressed\n");
 *                 break;
 *         }
 *     }
 * }
 * int main(void) {
 *     sdl_on_key(on_key);
 *     while (!sdl_is_done());
 * }
 * ```
 *
 * @param handler the function to call with each key press
 */
void sdl_on_key(key_handler_t handler);

void sdl_on_click(mouse_handler_t handler);

/**
 * Gets the amount of time that has passed since the last time
 * this function was called, in seconds.
 *
 * @return the number of seconds that have elapsed
 */
double time_since_last_tick(void);
/**
 * Creates the image given the position and size and texture.
 */

void sdl_create_image(SDL_Texture *img, vector_t position, vector_t size);

/**
 * Given the file name, it will load the texture
 *
 * @return the SDL_Texture of the file
 */
SDL_Texture *sdl_create_texture(const char *filename);

/**
 * Creating SDL Surface given file name and time to create the clock.
 *
 * @return SDL_Surface object
 */
SDL_Surface *sdl_create_message(const char *filename, double curr_time);

/**
 * Given the body, return the bounding box as a SDL_Rect object.
 */
SDL_Rect *sdl_make_bounding_box(body_t *body);

/**
 * Given font, text, color and position. It will draw the text at the
 * appropriate position with its color and text.
 */
void sdl_make_text(TTF_Font *font, const char *str, SDL_Color *color,
                   vector_t pos);

int sdl_sound_init();

Mix_Chunk* sdl_sound(const char *filename);

void sdl_play_sound(Mix_Chunk *sound, double volume_percentage);

void sdl_free_sound(Mix_Chunk *sound);

void sdl_stop_sound(Mix_Chunk *sound);      

#endif // #ifndef __SDL_WRAPPER_H__
