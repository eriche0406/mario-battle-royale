#include "sdl_wrapper.h"
#include "state.h"
#include "asset_cache.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>


const char WINDOW_TITLE[] = "CS 3";
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
const double MS_PER_S = 1e3;
const double MINIMUM = -DBL_MAX;
const double MAXIMUM = DBL_MAX;
const size_t NUM_KEYS = 512;
const double MAX_VOLUME = 128;

/**
 * The coordinate at the center of the screen.
 */
vector_t center;
/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;
/**
 * The mousepress handler, or NULL if none has been configured.
 */
mouse_handler_t mouse_handler = NULL;
/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
uint32_t key_start_timestamp;
/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

/** Computes the center of the window in pixel coordinates */
vector_t get_window_center(void) {
  int *width = malloc(sizeof(*width)), *height = malloc(sizeof(*height));
  assert(width != NULL);
  assert(height != NULL);
  SDL_GetWindowSize(window, width, height);
  vector_t dimensions = {.x = *width, .y = *height};
  free(width);
  free(height);
  return vec_multiply(0.5, dimensions);
}

/**
 * Computes the scaling factor between scene coordinates and pixel coordinates.
 * The scene is scaled by the same factor in the x and y dimensions,
 * chosen to maximize the size of the scene while keeping it in the window.
 */
double get_scene_scale(vector_t window_center) {
  // Scale scene so it fits entirely in the window
  double x_scale = window_center.x / max_diff.x,
         y_scale = window_center.y / max_diff.y;
  return x_scale < y_scale ? x_scale : y_scale;
}

/** Maps a scene coordinate to a window coordinate */
vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
  // Scale scene coordinates by the scaling factor
  // and map the center of the scene to the center of the window
  vector_t scene_center_offset = vec_subtract(scene_pos, center);
  double scale = get_scene_scale(window_center);
  vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
  vector_t pixel = {.x = round(window_center.x + pixel_center_offset.x),
                    // Flip y axis since positive y is down on the screen
                    .y = round(window_center.y - pixel_center_offset.y)};
  return pixel;
}

/**
 * Converts an SDL scan code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_key_from_scancode(SDL_Scancode scancode) {
    switch (scancode) {
        case SDL_SCANCODE_LEFT:
            return LEFT_ARROW;
        case SDL_SCANCODE_RIGHT:
            return RIGHT_ARROW;
        case SDL_SCANCODE_UP:
            return UP_ARROW;
        case SDL_SCANCODE_DOWN:
            return DOWN_ARROW;
        case SDL_SCANCODE_SPACE:
            return SPACE_BAR;
        case SDL_SCANCODE_W:
            return W_KEY;
        case SDL_SCANCODE_A:
            return A_KEY;
        case SDL_SCANCODE_S:
            return S_KEY;
        case SDL_SCANCODE_D:
            return D_KEY;
        case SDL_SCANCODE_F:
            return F_KEY;
        case SDL_SCANCODE_RSHIFT:
            return RSHIFT_KEY;
        default:
            return '\0';
    }
}

void sdl_init(vector_t min, vector_t max) {
  // Check parameters
  assert(min.x < max.x);
  assert(min.y < max.y);

  center = vec_multiply(0.5, vec_add(min, max));
  max_diff = vec_subtract(max, center);
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  TTF_Init();
}

bool sdl_is_done(void *state) {
  const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
  SDL_Event *event = malloc(sizeof(*event));
  assert(event != NULL);
  while (SDL_PollEvent(event)) {
    switch (event->type) {
    case SDL_QUIT:
      free(event);
      return true;
    case SDL_MOUSEBUTTONDOWN: {
      asset_cache_handle_buttons(state, event->motion.x, event->motion.y);
      if (mouse_handler == NULL) {
        return false;
      }
      mouse_handler(state, event->motion.x, event->motion.y);
      break;
    }
    }
  }

  for (int scancode = 0; scancode < SDL_NUM_SCANCODES; scancode++) {
        if (keyboard_state[scancode]) {
            char key = get_key_from_scancode(scancode);
            key_event_type_t type = KEY_PRESSED;
            double held_time = time_since_last_tick(); 
            key_handler(key, type, held_time, state);
        }
    }
  free(event);
  return false;
}

void sdl_clear(void) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
}

void sdl_draw_polygon(polygon_t *poly, rgb_color_t color) {
  list_t *points = polygon_get_points(poly);
  // Check parameters
  size_t n = list_size(points);
  assert(n >= 3);

  vector_t window_center = get_window_center();

  // Convert each vertex to a point on screen
  int16_t *x_points = malloc(sizeof(*x_points) * n),
          *y_points = malloc(sizeof(*y_points) * n);
  assert(x_points != NULL);
  assert(y_points != NULL);
  for (size_t i = 0; i < n; i++) {
    vector_t *vertex = list_get(points, i);
    vector_t pixel = get_window_position(*vertex, window_center);
    x_points[i] = pixel.x;
    y_points[i] = pixel.y;
  }

  // Draw polygon with the given color
  filledPolygonRGBA(renderer, x_points, y_points, n, color.r * 255,
                    color.g * 255, color.b * 255, 255);
  free(x_points);
  free(y_points);
}

void sdl_show(void) {
  // Draw boundary lines
  vector_t window_center = get_window_center();
  vector_t max = vec_add(center, max_diff),
           min = vec_subtract(center, max_diff);
  vector_t max_pixel = get_window_position(max, window_center),
           min_pixel = get_window_position(min, window_center);
  SDL_Rect *boundary = malloc(sizeof(*boundary));
  assert(boundary != NULL);
  boundary->x = min_pixel.x;
  boundary->y = max_pixel.y;
  boundary->w = max_pixel.x - min_pixel.x;
  boundary->h = min_pixel.y - max_pixel.y;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, boundary);
  free(boundary);

  SDL_RenderPresent(renderer);
}

void sdl_render_scene(scene_t *scene, void *aux) {
  sdl_clear();
  size_t body_count = scene_bodies(scene);
  for (size_t i = 0; i < body_count; i++) {
    body_t *body = scene_get_body(scene, i);
    list_t *shape = body_get_shape(body);
    polygon_t *poly = polygon_init(shape, (vector_t){0, 0}, 0, 0, 0, 0);
    sdl_draw_polygon(poly, *body_get_color(body));
    list_free(shape);
  }
  if (aux != NULL) {
    body_t *body = aux;
    sdl_draw_polygon(body_get_polygon(body), *body_get_color(body));
  }
  sdl_show();
}

void sdl_on_key(key_handler_t handler) { key_handler = handler; }

void sdl_on_click(mouse_handler_t handler) { mouse_handler = handler; }

double time_since_last_tick(void) {
  clock_t now = clock();
  double difference = last_clock
                          ? (double)(now - last_clock) / CLOCKS_PER_SEC
                          : 0.0; // return 0 the first time this is called
  last_clock = now;
  return difference;
}

void sdl_create_image(SDL_Texture *img, vector_t position, vector_t size) {
  SDL_Rect texr;
  texr.x = position.x;
  texr.y = position.y;
  texr.w = size.x;
  texr.h = size.y;
  SDL_RenderCopy(renderer, img, NULL, &texr);
}

SDL_Texture *sdl_create_texture(const char *filename) {
  return IMG_LoadTexture(renderer, filename); //"/assets/cs3_logo.png"
}

SDL_Surface *sdl_create_message(const char *filename, double curr_time) {
  TTF_Font *Sans = TTF_OpenFont(filename, 24);
  SDL_Color Black = {0, 0, 0};
  char *temp = malloc(100 * sizeof(char));
  assert(temp != NULL);
  sprintf(temp, "Clock: %d", (int)floor(curr_time));
  SDL_Surface *ret = TTF_RenderText_Solid(Sans, temp, Black);
  free(temp);
  return ret;
}

void sdl_make_text(TTF_Font *font, const char *str, SDL_Color *color,
                   vector_t pos) {
  size_t width, height;
  TTF_SizeText(font, str, (int *)&width, (int *)&height);
  SDL_Rect rect;
  rect.x = pos.x;
  rect.y = pos.y;
  rect.w = (double)width;
  rect.h = (double)height;
  SDL_Surface *text = TTF_RenderText_Solid(font, str, *color);
  SDL_Texture *cap = SDL_CreateTextureFromSurface(renderer, text);
  SDL_RenderCopy(renderer, cap, NULL, &rect);

  SDL_FreeSurface(text);
  SDL_DestroyTexture(cap);
}

SDL_Rect *sdl_make_bounding_box(body_t *body) {
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  assert(rect != NULL);
  vector_t min, max;
  min.x = MAXIMUM;
  min.y = MAXIMUM;
  max.x = MINIMUM;
  max.y = MINIMUM;
  list_t *vertices = body_get_shape(body);
  vector_t window_center = get_window_center();
  for (size_t i = 0; i < list_size(vertices); i++) {
    vector_t *vertice = list_get(vertices, i);
    if (vertice->x < min.x) {
      min.x = vertice->x;
    }
    if (vertice->y < min.y) {
      min.y = vertice->y;
    }
    if (vertice->x > max.x) {
      max.x = vertice->x;
    }
    if (vertice->y > max.y) {
      max.y = vertice->y;
    }
  }
  vector_t pixel_min = get_window_position(min, window_center);
  vector_t pixel_max = get_window_position(max, window_center);
  rect->x = (int)pixel_min.x;
  rect->y = (int)pixel_max.y;
  rect->w = (int)(pixel_max.x - pixel_min.x);
  rect->h = (int)(pixel_min.y - pixel_max.y);
  return rect;
}

int sdl_sound_init() {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    return -1;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
    return -1;
  }

    return 0;
}


Mix_Chunk* sdl_sound(const char *filename) {
  Mix_Chunk *sound = Mix_LoadWAV(filename);
  assert(sound != NULL);
  return sound;
}

void sdl_play_sound(Mix_Chunk *sound, double volume_percentage) {
  Mix_VolumeChunk(sound , (int)(volume_percentage * MAX_VOLUME));
  Mix_PlayChannel(-1, sound, 0);
}

void sdl_free_sound(Mix_Chunk *sound) {
  Mix_FreeChunk(sound);
}

void sdl_stop_sound(Mix_Chunk *sound) {
    int channel = Mix_GroupAvailable(-1);
    if (channel != -1) {
        Mix_HaltChannel(channel);
    }
}