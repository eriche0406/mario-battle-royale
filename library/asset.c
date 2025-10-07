#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "color.h"
#include "sdl_wrapper.h"

typedef struct asset {
  asset_type_t type;
  SDL_Rect bounding_box;
} asset_t;

typedef struct text_asset {
  asset_t base;
  TTF_Font *font;
  const char *text;
  rgb_color_t color;
} text_asset_t;

typedef struct image_asset {
  asset_t base;
  SDL_Texture *texture;
  body_t *body;
} image_asset_t;

typedef struct button_asset {
  asset_t base;
  image_asset_t *image_asset;
  text_asset_t *text_asset;
  button_handler_t handler;
  bool is_rendered;
} button_asset_t;

static asset_t *asset_init(asset_type_t ty, SDL_Rect bounding_box) {
  asset_t *new;
  switch (ty) {
  case ASSET_IMAGE: {
    new = malloc(sizeof(image_asset_t));
    break;
  }
  case ASSET_FONT: {
    new = malloc(sizeof(text_asset_t));
    break;
  }
  case ASSET_BUTTON: {
    new = malloc(sizeof(button_asset_t));
    break;
  }
  default: {
    assert(false && "Unknown asset type");
  }
  }
  assert(new);
  new->type = ty;
  new->bounding_box = bounding_box;
  return new;
}

asset_type_t asset_get_type(asset_t *asset) { return asset->type; }

asset_t *asset_make_image(const char *filepath, SDL_Rect bounding_box) {
  SDL_Texture *texture = asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  asset_t *asset = asset_init(ASSET_IMAGE, bounding_box);
  image_asset_t *image_asset = malloc(sizeof(image_asset_t));
  assert(image_asset != NULL);
  image_asset->base = *asset;
  image_asset->texture = texture;
  image_asset->body = NULL;
  return (asset_t *)image_asset;
}

asset_t *asset_make_image_with_body(const char *filepath, body_t *body) {
  SDL_Texture *texture = asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  asset_t *asset = asset_init(ASSET_IMAGE, *sdl_make_bounding_box(body));
  image_asset_t *image_asset = malloc(sizeof(image_asset_t));
  assert(image_asset != NULL);
  image_asset->base = *asset;
  image_asset->texture = texture;
  image_asset->body = body;
  return (asset_t *)image_asset;
}

asset_t *asset_make_text(const char *filepath, SDL_Rect bounding_box,
                         const char *text, rgb_color_t color) {
  TTF_Font *font = asset_cache_obj_get_or_create(ASSET_FONT, filepath);
  asset_t *asset = asset_init(ASSET_FONT, bounding_box);
  text_asset_t *text_asset = malloc(sizeof(text_asset_t));
  assert(text_asset != NULL);
  text_asset->base = *asset;
  text_asset->color = color;
  text_asset->text = text;
  text_asset->font = font;
  return (asset_t *)text_asset;
}

asset_t *asset_make_button(SDL_Rect bounding_box, asset_t *image_asset,
                           asset_t *text_asset, button_handler_t handler) {
  asset_t *asset = asset_init(ASSET_BUTTON, bounding_box);
  button_asset_t *button_asset = malloc(sizeof(button_asset_t));
  assert(button_asset != NULL);
  button_asset->base = *asset;
  button_asset->handler = handler;
  button_asset->image_asset = (image_asset_t *)image_asset;
  button_asset->text_asset = (text_asset_t *)text_asset;
  button_asset->is_rendered = false;

  return (asset_t *)button_asset;
}

void asset_on_button_click(asset_t *button, state_t *state, double x,
                           double y) {
  button_asset_t *button_asset = (button_asset_t *)button;

  if (!button_asset->is_rendered) {
    return;
  }
  if (in_bounding_box(button->bounding_box, x, y)) {
    button_asset->handler(state);
    button_asset->is_rendered = false;
  }
}

bool in_bounding_box(SDL_Rect bounding_box, double x, double y) {
  return (x >= bounding_box.x && x <= bounding_box.x + bounding_box.w &&
          y >= bounding_box.y && y <= bounding_box.y + bounding_box.h);
}

void asset_render(asset_t *asset) {
  switch (asset->type) {
  case ASSET_IMAGE: {
    image_asset_t *image_asset = (image_asset_t *)asset;
    body_t *body = image_asset->body;
    SDL_Rect bounding_box;
    if (body != NULL) {
      bounding_box = *sdl_make_bounding_box(body);
    } else {
      bounding_box = image_asset->base.bounding_box;
    }
    vector_t position = (vector_t){bounding_box.x, bounding_box.y};
    vector_t size = (vector_t){bounding_box.w, bounding_box.h};
    sdl_create_image(image_asset->texture, position, size);
    break;
  }
  case ASSET_FONT: {
    text_asset_t *text_asset = (text_asset_t *)asset;
    vector_t position = (vector_t){text_asset->base.bounding_box.x,
                                   text_asset->base.bounding_box.y};
    SDL_Color color = {text_asset->color.r, text_asset->color.g,
                       text_asset->color.b};
    sdl_make_text(text_asset->font, text_asset->text, &color, position);
    break;
  }
  case ASSET_BUTTON: {
    button_asset_t *button_asset = (button_asset_t *)asset;
    if (button_asset->image_asset != NULL) {
      asset_render((asset_t *)button_asset->image_asset);
    }
    if (button_asset->text_asset != NULL) {
      asset_render((asset_t *)button_asset->text_asset);
    }
    button_asset->is_rendered = true;
    break;
  }
  default: {
    break;
  }
  }
}

body_t *asset_get_body(asset_t *asset) {
    if (asset == NULL || asset->type != ASSET_IMAGE) {
        return NULL;
    }
    image_asset_t *image_asset = (image_asset_t *)asset;
    return image_asset->body;
}

SDL_Rect *asset_get_bounding_box(asset_t *asset) {
  return &asset->bounding_box;
}

void asset_change_text(asset_t *asset, char *text) {
  text_asset_t *text_asset = (text_asset_t *)asset;
  text_asset->text = text;
}

void asset_change_texture(asset_t *asset, const char *filepath) {
  image_asset_t *image_asset = (image_asset_t *)asset;
  image_asset->texture = asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
}

void asset_destroy(asset_t *asset) {
  free(asset);
}
