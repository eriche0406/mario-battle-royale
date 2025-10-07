#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "list.h"
#include "sdl_wrapper.h"

static list_t *ASSET_CACHE;

const size_t FONT_SIZE = 18;
const size_t INITIAL_CAPACITY = 5;

typedef struct {
  asset_type_t type;
  const char *filepath;
  void *obj;
} entry_t;

static void asset_cache_free_entry(entry_t *entry) {
  switch (entry->type) {
  case ASSET_IMAGE: {
    asset_destroy((asset_t *)entry->obj);
    break;
  }
  case ASSET_FONT: {
    asset_destroy((asset_t *)entry->obj);
    break;
  }
  case ASSET_BUTTON: {
    asset_destroy((asset_t *)entry->obj);
  }
  default: {
    break;
  }
  }
  free(entry);
}

void asset_cache_init() {
  ASSET_CACHE =
      list_init(INITIAL_CAPACITY, (free_func_t)asset_cache_free_entry);
}

void asset_cache_destroy() { list_free(ASSET_CACHE); }

void *asset_cache_obj_get_or_create(asset_type_t ty, const char *filepath) {
  if (filepath == NULL) {
    return NULL;
  }
  entry_t *entry = (entry_t *)obj_exists(filepath);
  if (entry != NULL) {
    assert(entry->type == ty);
    return entry->obj;
  }
  entry_t *new_entry = malloc(sizeof(entry_t));
  assert(new_entry != NULL);
  new_entry->filepath = filepath;
  new_entry->type = ty;

  switch (ty) {
  case ASSET_IMAGE: {
    new_entry->obj = sdl_create_texture(filepath);
    break;
  }
  case ASSET_FONT: {
    new_entry->obj = TTF_OpenFont(filepath, FONT_SIZE);
    break;
  }
  default: {
    break;
  }
  }
  list_add(ASSET_CACHE, new_entry);
  return new_entry->obj;
}

void *obj_exists(const char *filepath) {
  if (filepath == NULL) {
    return NULL;
  }
  for (int i = 0; i < list_size(ASSET_CACHE); i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (strcmp(entry->filepath, filepath) == 0) {
      return entry;
    }
  }
  return NULL;
}

void asset_cache_register_button(asset_t *button) {
  entry_t *new_button = malloc(sizeof(entry_t));
  assert(new_button);
  new_button->type = ASSET_BUTTON;
  new_button->filepath = NULL;
  new_button->obj = button;
  list_add(ASSET_CACHE, new_button);
}

void asset_cache_handle_buttons(state_t *state, double x, double y) {
  for (size_t i = 0; i < list_size(ASSET_CACHE); i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (entry->type == ASSET_BUTTON) {
      asset_on_button_click(entry->obj, state, x, y);
    }
  }
}
