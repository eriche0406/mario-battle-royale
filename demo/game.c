#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "asset.h"
#include "asset_cache.h"
#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "character.h"
#include <SDL2/SDL_mixer.h>

const char *MARIO_PATH_LEFT = "assets/Super Mario Sprite (Facing Left).png";
const char *MARIO_PATH_RIGHT = "assets/Super Mario Sprite.png";
const char *BOWSER_PATH_LEFT = "assets/Bowser Sprite.png";
const char *BOWSER_PATH_RIGHT = "assets/Bowser Sprite (Right Facing).png";
const char *POWERED_MARIO_PATH_LEFT = "assets/Tanooki_Mario_Left.png";
const char *PLUS_HEALTH = "assets/plus health.png";
const char *POWERED_MARIO_PATH_RIGHT = "assets/Tanooki_Mario_Right.png";
const char *POWERED_BOWSER_PATH_LEFT = "assets/Dark_Bowser_Left.png";
const char *POWERED_BOWSER_PATH_RIGHT = "assets/Dark_Bowser_Right.png";
const char *BACKGROUND_PATH = "assets/Mario Background.png";
const char *START_BUTTON_PATH = "assets/start button.png";
const char *BOMB_BUTTON_PATH = "assets/Bombs Only.png";
const char *MYSTERY_BOX_PATH = "assets/Mario Question Mark Brick.png";
const char *GOOMBA_PATH = "assets/Gumba Sprite.png";
const char *RIGHT_STANDARD_BULLET = "assets/Right Standard Bullet.png";
const char *LEFT_STANDARD_BULLET = "assets/Left Standard Bullet.png";
const char *RIGHT_BOMB_BULLET = "assets/Right Bomb.png";
const char *LEFT_BOMB_BULLET = "assets/Left Bomb.png";
const char *LOADING_PATH = "assets/Loading Screen.png";
const char *MARIO_ATTACKED = "assets/mario damage .png";
const char *BOWSER_ATTACKED_RIGHT = "assets/Bowser Hit Right.png";
const char *BOWSER_ATTACKED_LEFT = "assets/Bowser Hit Left.png";
const char *FIREBALL_SOUND = "assets/smb_fireball.wav";
const char *POWER_UP_SOUND = "assets/powerup.wav";
const char *HURT_CHARACTER_SOUND = "assets/Mario Hit.wav";
const char *DEAD_CHARACTER_SOUND = "assets/Mario Dies.wav";
const char *CHARACTER_JUMP_SOUND = "assets/Mario Jump.wav";
const char *TEXT_FONT = "assets/Impacted.ttf";
const char *RESTART_BUTTON = "assets/restart button.png";
const char *MARIO_VICTORY_SKIN = "assets/Mario_Winner.png";
const char *BOWSER_VICTORY_SKIN = "assets/Bowser_Winner.png";
const char *MARIO_LOSER_SKIN = "assets/Loser_Mario.png";
const char *BOWSER_LOSER_SKIN = "assets/Loser_Bowser.png";
const char *HEALTH_UP = "assets/plus health.png";
const char *PLAYER_TYPE = "PLAYER";
const char *STANDARD_BULLET_TYPE = "STANDARD";
const char *BOMB_BULLET_TYPE = "BOMB";
const char *GOOMBA_TYPE = "GOOMBA";
const char *MYSTERY_TYPE = "MYSTERY";
const char *MARIO_HEALTH = "100";
const char *BOWSER_HEALTH = "100";
const char *POWER_UPS[] = {"SPEED", "HEALTH", "INVINCIBILITY", "BOMB"};
const char *DEFAULT_POWER = "NONE";
const bool RIGHT = true;
const bool LEFT = false;
const vector_t MIN = {0, 0};
const vector_t MAX = {1000, 500};
const vector_t BOMB_VEL = {300, 900};
const vector_t WINNER_POSITION = {400, 100};
const vector_t LOSER_POSITION = {600, 100};
const vector_t START_POS1 = {100, 98};
const vector_t START_POS2 = {900, 102};
const size_t GOOMBA_MINIMUM_HEIGHT = 85;
const size_t POWER_UP_MIN_HEIGHT = 100;
const size_t POWER_UP_MAX_HEIGHT = 300;
const size_t GOOMBA_SPAWN_HEIGHT = 450;
const size_t NUM_POINTS = 20;
const rgb_color_t PLAYER_COLOR = (rgb_color_t){0.1, 0.9, 0.2};
const rgb_color_t MARIO_TEXT_COLOR = (rgb_color_t){255, 0, 0};
const rgb_color_t BOWSER_TEXT_COLOR = (rgb_color_t){0, 255, 0};
const rgb_color_t POWER_TEXT_COLOR = (rgb_color_t){255, 255, 0};
const SDL_Rect BUTTON_BOUNDING_BOX = (SDL_Rect) {300, 170, 400, 160};
const SDL_Rect RESTART_BOUNDING_BOX = (SDL_Rect) {425, 25, 150, 150};
const SDL_Rect BASE_BOUNDING_BOX = (SDL_Rect) {0, 30, 50, 40};
const SDL_Rect POWER_BOUNDING_BOX = (SDL_Rect) {0, 50, 50, 40};
const SDL_Rect BACKGROUND_BOX = (SDL_Rect) {MIN.x, MIN.y, MAX.x - MIN.x, MAX.y - MIN.y};
const SDL_Rect BOMB_BOUNDING_BOX = (SDL_Rect) {350, 350, 300, 75};
const double GOOMBA_INTERVAL = 10.0;
const double MYSTERY_INTERVAL = 7.5;
const double OUTER_RADIUS = 50;
const double INNER_RADIUS = 50;
const double GOOMBA_RADIUS = 30;
const double BULLET_RADIUS = 30;
const double HEART_RADIUS = 30;
const double MYSTERY_BOX_RADIUS = 30;
const double GOOMBA_VELOCITY = 25.0;
const double JUMP_VELOCITY = 800.0;
const double BULLET_SHIFT = 80.5;
const double HEART_SHIFT = 70;
const double STANDARD_BULLET = 800.0;
const double GRAVITY_CONSTANT = 2000;
const double GOOMBA_GRAVITY = 1000;
const double PLAYER_HEALTH = 100.0;
const double ELASTICITY = 0.5;
const double FIRE_RATE = 1;
const double VELOCITY_INTERVAL = 0.2;
const double GOOMBA_DAMAGE = -10.0;
const double STANDARD_DAMAGE = -10.0;
const double BOMB_DAMAGE = -34.0;
const double MIN_HEALTH_INCREASE = 5.0;
const double MAX_HEALTH_INCREASE = 30.0;
const double INVINCE_LIMIT = 10.0;
const double POWER_LIMIT = 10.0;
const double LOW_VOLUME = 0.1;
const double FREEZE_INTERVALS[] = {0.2, 0.4, 0.6, 0.8};
const size_t NUM_POWER_UPS = sizeof(POWER_UPS) / sizeof(POWER_UPS[0]);
const size_t HEALTH_INDEX = 1;
const size_t BOMB_INDEX = 3;
const size_t CHAR_SIZE = 100;
const size_t JUMP_RESTRICTION = 141;
const size_t MARIO_CHARACTER = 0;
const size_t BOWSER_CHARACTER = 1;
const size_t BACKGROUND_ASSET = 0;
const size_t MARIO_ASSET = 1;
const size_t BOWSER_ASSET = 2;
const int16_t H_STEP = 5;

struct state {
  list_t *body_assets;
  list_t *bullet_assets;
  list_t *button_assets;
  list_t *heart_assets;
  list_t *sounds;
  list_t *characters;
  scene_t *scene;
  double timer;
  double goomba_timer;
  double goomba_count;
  double mystery_box_count;
  double freeze_timer;
  double mystery_timer;
  double fire_timer;
  double velocity_timer;
  bool key_states[SDL_NUM_SCANCODES];
  bool fire_rate;
  bool frozen;
  bool loading;
  bool bombs_only;
  bool is_win;
  bool winner; // true for mario and false for bowser
  asset_t *mario_health;
  asset_t *bowser_health;
  asset_t *mario_power;
  asset_t *bowser_power;
  asset_t *restart_button;
};

double rand_neg1_or_1() {
    return (rand() % 2 == 0) ? 1 : -1;
}

double rand_double(double low, double high) {
  return (high - low) * rand() / RAND_MAX + low;
}

size_t rand_int(size_t low, size_t high) {
  return rand() % (high - low + 1) + low;
}

body_t *make_body(double outer_radius, double inner_radius, vector_t center) {
  center.y += inner_radius;
  list_t *c = list_init(NUM_POINTS, free);
  for (size_t i = 0; i < NUM_POINTS; i++) {
    double angle = 2 * M_PI * i / NUM_POINTS;
    vector_t *v = malloc(sizeof(*v));
    assert(v != NULL);
    *v = (vector_t){center.x + inner_radius * cos(angle),
                    center.y + outer_radius * sin(angle)};
    list_add(c, v);
  }
  body_t *player = body_init(c, 1, PLAYER_COLOR);
  return player;
}

void apply_gravity(body_t *body, state_t *state, double min_y) {
  vector_t body_vel = body_get_velocity(body);
  vector_t body_pos = body_get_centroid(body);
  bool is_jumping = (body_vel.y > 0);
  if (body_pos.y > min_y) {
    body_add_force(body, (vector_t){0, -GRAVITY_CONSTANT});
  }
  else {
    if (!is_jumping) {
    body_reset(body);
    body_set_velocity(body, (vector_t){body_vel.x, 0});
  }
  }
}

void jump(body_t *player, state_t *state) {
  vector_t player_curr_vel = body_get_velocity(player);
  vector_t player_curr_pos = body_get_centroid(player);
  if (player_curr_vel.y == 0 && player_curr_pos.y <= JUMP_RESTRICTION) {
    vector_t jump_vel = {player_curr_vel.x, JUMP_VELOCITY};
    body_set_velocity(player, jump_vel);
    if (!state->is_win && !state->loading) {
      Mix_Chunk *jump_sound = sdl_sound(CHARACTER_JUMP_SOUND);
      list_add(state->sounds, jump);
      sdl_play_sound(jump_sound, LOW_VOLUME);
    }
  }
}

//animation for when a player collects a mystery box
void powerup_oscillation(state_t *state, asset_t *asset, const char *filepath1, 
                          const char *filepath2){
  if (state->frozen) {
    if (state->freeze_timer < FREEZE_INTERVALS[0]) {
      asset_change_texture(asset, filepath1);
    }
    else if (state->freeze_timer < FREEZE_INTERVALS[1]) {
      asset_change_texture(asset, filepath2);
    }
    else if (state->freeze_timer < FREEZE_INTERVALS[2]) {
      asset_change_texture(asset, filepath1);
    }
    else if (state->freeze_timer < FREEZE_INTERVALS[3]) {
      asset_change_texture(asset, filepath2);
    }
    else {
      asset_change_texture(asset, filepath1);
    }
  }
}

//calls the animation for the powerup on a specific character
void powerup_animation(state_t *state, character_t *character) {
    asset_t *mario_asset = list_get(state->body_assets, MARIO_ASSET);
    asset_t *bowser_asset = list_get(state->body_assets, BOWSER_ASSET);
    character_t *mario = list_get(state->characters, MARIO_CHARACTER);
      if (character == mario) {
        if (character_get_direction(character)) {
          powerup_oscillation(state, mario_asset, POWERED_MARIO_PATH_RIGHT, 
                              MARIO_PATH_RIGHT);
        }
        else {
          powerup_oscillation(state, mario_asset, POWERED_MARIO_PATH_LEFT, 
                              MARIO_PATH_LEFT);
        }
      }
      else {
        if (character_get_direction(character)) {
          powerup_oscillation(state, bowser_asset, POWERED_BOWSER_PATH_RIGHT, 
                              BOWSER_PATH_RIGHT);
        }
        else {
          powerup_oscillation(state, bowser_asset, POWERED_BOWSER_PATH_LEFT, 
                              BOWSER_PATH_LEFT);
        }
      }
}

void fire_bullet(bool fire_left, character_t *character, state_t *state) {
  if (character_get_fire(character)) {
    double gravity = 0;
    char *bullet_type = malloc(sizeof(char) * CHAR_SIZE);
    assert(bullet_type != NULL);
    bool direction = true;
    bool is_bomb = false;
    char *bullet_path = malloc(sizeof(char) * CHAR_SIZE);
    assert(bullet_path != NULL);
    vector_t bullet_vel = {STANDARD_BULLET, 0};
    if (strcmp(character_get_bullet_type(character), STANDARD_BULLET_TYPE) 
        == 0) {
      bullet_type = strdup(STANDARD_BULLET_TYPE);
      bullet_path = strdup(RIGHT_STANDARD_BULLET);
    }
    if ((strcmp(character_get_bullet_type(character), BOMB_BULLET_TYPE) == 0 ||
          state->bombs_only)) {
      bullet_vel = BOMB_VEL;
      gravity = GRAVITY_CONSTANT;
      bullet_type = strdup(BOMB_BULLET_TYPE);
      bullet_path = strdup(RIGHT_BOMB_BULLET);
      is_bomb = true;
    }
    body_t *player = character_get_body(character);
    vector_t bullet_pos = body_get_centroid(player);
    bullet_pos.x += BULLET_SHIFT;
    // implements left-firing bullets (different assets/direction)
    if (fire_left) {
      bullet_vel.x = -bullet_vel.x;
      bullet_pos.x -= 2 * BULLET_SHIFT;
      direction = false;
      if (is_bomb) {
        bullet_path = strdup(LEFT_BOMB_BULLET);
      }
      else {
        bullet_path = strdup(LEFT_STANDARD_BULLET);
      }
    }
    body_t *bullet = make_body(BULLET_RADIUS, BULLET_RADIUS, VEC_ZERO);
    body_set_centroid(bullet, bullet_pos);
    body_set_velocity(bullet, bullet_vel);
    body_add_force(bullet, (vector_t){0, -gravity});
    scene_add_body(state->scene, bullet);
    asset_t *bullet_asset =
      asset_make_image_with_body(bullet_path, scene_get_body(state->scene, 
      scene_bodies(state->scene) - 1));
    list_add(state->bullet_assets, bullet_asset);
    character_t *bullet_char = character_init(bullet, bullet_type, 0, 0, 0, 0, 
    NULL, direction);
    list_add(state->characters, bullet_char);
    character_set_fire(character, false);
    character_set_fire_time(character, 0.0);
    Mix_Chunk *fire_sound = sdl_sound(FIREBALL_SOUND);
    list_add(state->sounds, fire_sound);
    sdl_play_sound(fire_sound, LOW_VOLUME);
  }
}

char *get_power(state_t *state) {
  if (state->bombs_only) {
    // remove bomb powerup if bombs only gamemode
    size_t index = rand_int(0, NUM_POWER_UPS - 2);
    return (char *)POWER_UPS[index];
  }
  else {
    size_t index = rand_int(0, NUM_POWER_UPS - 1);
    return (char *)POWER_UPS[index];
  }
}

//frezes screen for animation effects
void freeze_screen(state_t *state, double freeze_duration) {
  state->frozen = true;
  state->freeze_timer = freeze_duration;
}

void speed_power(character_t *character, state_t *state) {
  character_set_translation(character, H_STEP * 2);
  character_set_power_time(character, state->timer);
}

void health_power(character_t *character, state_t *state) {
  double health_incerase = rand_int(MIN_HEALTH_INCREASE, MAX_HEALTH_INCREASE);
  character_change_health(character, health_incerase);
}

void invince_power(character_t *character, state_t *state) {
  character_set_invince(character, true);
  character_set_power_time(character, state->timer);
}

void power_reset(character_t *character) {
  character_set_invince(character, false);
  character_set_translation(character, H_STEP);
  character_set_ability(character, (char *)DEFAULT_POWER);
  character_set_bullet_type(character, STANDARD_BULLET_TYPE);
}

void bomb_power(character_t *character, state_t* state) {
  character_set_bullet_type(character, BOMB_BULLET_TYPE);
  character_set_power_time(character, state->timer);
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
    character_t *character1 = list_get(state->characters, MARIO_CHARACTER);
    character_t *character2 = list_get(state->characters, BOWSER_CHARACTER);
    body_t *player1 = character_get_body(character1);
    body_t *player2 = character_get_body(character2);
    vector_t translation1 = (vector_t){0, 0};
    vector_t translation2 = (vector_t){0, 0};
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    if (!state->frozen && !state->is_win && !state->loading) {
      switch (key) {
          case LEFT_ARROW:
              translation2.x -= character_get_translation(character2);
              character_set_direction(character2, LEFT);
              break;
          case RIGHT_ARROW:
              translation2.x += character_get_translation(character2);
              character_set_direction(character2, RIGHT);
              break;
          case UP_ARROW:
              if (type == KEY_PRESSED && keyboard_state[SDL_SCANCODE_UP]) {
                jump(player2, state);
              }
              break;
          case RSHIFT_KEY:
              if (type == KEY_PRESSED && keyboard_state[SDL_SCANCODE_RSHIFT]) {
                if (character_get_direction(character2)) {
                  fire_bullet(false, character2, state);
                }
                else {
                  fire_bullet(true, character2, state);
                }
              }
              break;
          case A_KEY:
              translation1.x -= character_get_translation(character1);
              character_set_direction(character1, LEFT);
              break;
          case D_KEY:
              translation1.x += character_get_translation(character1);
              character_set_direction(character1, RIGHT);
              break;
          case W_KEY:
              if (type == KEY_PRESSED && keyboard_state[SDL_SCANCODE_W]) {
                jump(player1, state);
              }
              break;
          case F_KEY:
              if (type == KEY_PRESSED && keyboard_state[SDL_SCANCODE_F]) {
                if (character_get_direction(character1)) {
                  fire_bullet(false, character1, state);
                }
                else {
                  fire_bullet(true, character1, state);
                }
              }
              break;
          default:
              return;
      }
    }
    vector_t new_centroid1 = vec_add(body_get_centroid(player1), translation1);
    body_set_centroid(player1, new_centroid1);
    
    vector_t new_centroid2 = vec_add(body_get_centroid(player2), translation2);
    body_set_centroid(player2, new_centroid2);
}

void remove_character(state_t *state, body_t *body) {
    for (size_t i = 0; i < list_size(state->characters); i++) {
      character_t *character = list_get(state->characters, i);
        if (character_get_body(character) == body) {
            list_remove(state->characters, i);
            character_free(character);
            break;
        }
    }
}

void remove_asset(list_t *list, body_t *body) {
    for (size_t i = 0; i < list_size(list); i++) {
        asset_t *asset = list_get(list, i);
        if (asset_get_body(asset) == body) {
            asset_destroy(asset);
            list_remove(list, i);
            break;
        }
    }
}

void generate_goomba(state_t *state) {
    state->goomba_count++;
    body_t *goomba = make_body(GOOMBA_RADIUS, GOOMBA_RADIUS, VEC_ZERO);
    vector_t goomba_pos = {rand_double(0, MAX.x), GOOMBA_SPAWN_HEIGHT};
    vector_t goomba_vel = vec_multiply(rand_neg1_or_1(), 
                                        (vector_t){GOOMBA_VELOCITY, 0});
    body_set_centroid(goomba, goomba_pos);
    body_set_velocity(goomba, goomba_vel);
    scene_add_body(state->scene, goomba);
    asset_t *goomba_asset =
      asset_make_image_with_body(GOOMBA_PATH, scene_get_body(state->scene, 
                                              scene_bodies(state->scene) - 1));
    list_add(state->body_assets, goomba_asset);
    character_t *goomba_char = character_init(goomba, (char *) GOOMBA_TYPE, 0, 
                                              0, 0, 0, NULL, NULL);
    list_add(state->characters, goomba_char);
}

void generate_mystery_box(state_t *state) {
  body_t *mystery = make_body(MYSTERY_BOX_RADIUS, MYSTERY_BOX_RADIUS, 
                                VEC_ZERO);
  vector_t mystery_pos = {rand_double(0, MAX.x), 
                         rand_double(POWER_UP_MIN_HEIGHT, POWER_UP_MAX_HEIGHT)};
  body_set_centroid(mystery, mystery_pos);
  scene_add_body(state->scene, mystery);
  asset_t *mystery_asset =
    asset_make_image_with_body(MYSTERY_BOX_PATH, scene_get_body(state->scene, 
    scene_bodies(state->scene) - 1));
  list_add(state->body_assets, mystery_asset);
  character_t *mystery_char = character_init(mystery, (char *) MYSTERY_TYPE, 
                                              0, 0, 0, 0, NULL, NULL);
  list_add(state->characters, mystery_char);
}

//updates the floating powerup texts above the players
void update_power_texts(asset_t *power_text, character_t *character) {
  char *ability = character_get_ability(character);
  asset_change_text(power_text, ability);
  SDL_Rect *bounding_box = asset_get_bounding_box(power_text);
  vector_t center = body_get_centroid(character_get_body(character));
  bounding_box->x = center.x - bounding_box->w/2;
  asset_render(power_text);
}

//flashes a heart above the player if they collect a health boos powerup
void health_animation(state_t *state, character_t *character) {
   body_t *player = character_get_body(character);
   vector_t heart_pos = body_get_centroid(player);
   heart_pos.y += HEART_SHIFT;
   body_t *heart = make_body(HEART_RADIUS, HEART_RADIUS, VEC_ZERO);
   body_set_centroid(heart, heart_pos);
   asset_t *heart_asset = asset_make_image_with_body(HEALTH_UP, heart);
   list_add(state->heart_assets, heart_asset);
}

//updates which character won the game and creates a win screen
void update_win(state_t *state, character_t *loser) {
  character_t *mario = list_get(state->characters, MARIO_CHARACTER);
  character_t *bowser = list_get(state->characters, BOWSER_CHARACTER);
  if (loser == mario) {
    state->winner = false;
    body_t *winner_body = character_get_body(bowser);
    body_t *loser_body = character_get_body(mario);
    body_set_centroid(winner_body, WINNER_POSITION);
    body_set_velocity(winner_body, MIN);
    body_set_centroid(loser_body, LOSER_POSITION);
    body_set_velocity(loser_body, MIN);
  }
  else {
    state->winner = true;
    body_t *winner_body = character_get_body(mario);
    body_t *loser_body = character_get_body(bowser);
    body_set_centroid(winner_body, WINNER_POSITION);
    body_set_velocity(winner_body, MIN);
    body_set_centroid(loser_body, LOSER_POSITION);
    body_set_velocity(loser_body, MIN);
  }
  state->is_win = true;
}

//applies an animation to the winner of the game
void update_win_screen(state_t *state) {
  character_t *mario = list_get(state->characters, MARIO_CHARACTER);
  character_t *bowser = list_get(state->characters, BOWSER_CHARACTER);
  if (state->winner) {
    body_t *winner = character_get_body(mario);
    body_t *loser = character_get_body(bowser);
    jump(winner, state);
    apply_gravity(winner, state, WINNER_POSITION.y);
    apply_gravity(loser, state, START_POS2.y);
  }
  else {
    body_t *winner = character_get_body(bowser);
    body_t *loser = character_get_body(mario);
    jump(winner, state);
    apply_gravity(winner, state, WINNER_POSITION.y);
    apply_gravity(loser, state, START_POS1.y);
  }
}

bool compare_character_type(character_t *character, const char* type) {
  return (strcmp(character_get_type(character), type) == 0);
}

void handle_collisions(state_t *state, character_t *character1, 
                        character_t *character2, collision_info_t collision) {
     body_t *body1 = character_get_body(character1);
    //With the way collisions is called, character 2 can never player type
     body_t *body2 = character_get_body(character2);
    //Bullet - Goomba Collisions
    if ((compare_character_type(character1, STANDARD_BULLET_TYPE) ||
          compare_character_type(character1, BOMB_BULLET_TYPE)) && 
          compare_character_type(character2, GOOMBA_TYPE)) {
        remove_asset(state->bullet_assets, body1);
        remove_asset(state->body_assets, body2);
        remove_character(state, body1);
        remove_character(state, body2);
        create_destructive_collision(state->scene, body1, body2);
    } 
    else if ((compare_character_type(character2, STANDARD_BULLET_TYPE) ||
              compare_character_type(character2, BOMB_BULLET_TYPE)) && 
              compare_character_type(character1, GOOMBA_TYPE)) {
        remove_asset(state->bullet_assets, body2);
        remove_asset(state->body_assets, body1);
        remove_character(state, body2);
        remove_character(state, body1);
        create_destructive_collision(state->scene, body2, body1);
    }
    //Player - Mystery Box collisions
    else if (compare_character_type(character1, PLAYER_TYPE)
              && compare_character_type(character2, MYSTERY_TYPE)) {
        freeze_screen(state, 1.0);
        remove_asset(state->body_assets, body2);
        remove_character(state, body2);
        create_physics_collision(state->scene, body1, body2, 0.0);
        body_remove(body2);
        Mix_Chunk *powerup_sound = sdl_sound(POWER_UP_SOUND);
        list_add(state->sounds, powerup_sound);
        sdl_play_sound(powerup_sound, LOW_VOLUME);
        power_reset(character1);
        char *power = get_power(state);
        //speed powerup
        if (strcmp(power, POWER_UPS[0]) == 0) {
          speed_power(character1, state);
          character_set_ability(character1, (char *)POWER_UPS[0]);
        }
        //health boost powerup
        if (strcmp(power, POWER_UPS[1]) == 0) {
          health_power(character1, state);
          character_set_health_boost(character1, true);
        }
        //invincible powerup
        if (strcmp(power, POWER_UPS[2]) == 0) {
          invince_power(character1, state);
          character_set_ability(character1, (char *)POWER_UPS[2]);
        }
        //bomb powerup
        if (strcmp(power, POWER_UPS[3]) == 0) {
          bomb_power(character1, state);
          character_set_ability(character1, (char *)POWER_UPS[3]);
        }
        state->mystery_box_count--;
    }
    
    //Bullet - Player Collisions
    else if (compare_character_type(character1, PLAYER_TYPE) && 
    (compare_character_type(character2, STANDARD_BULLET_TYPE) || 
    compare_character_type(character2, BOMB_BULLET_TYPE))) {
      remove_asset(state->bullet_assets, body2);
      remove_character(state, body2);
      if (compare_character_type(character2, BOMB_BULLET_TYPE)) {
        character_change_health(character1, BOMB_DAMAGE);
        Mix_Chunk *bomb_hit_sound = sdl_sound(HURT_CHARACTER_SOUND);
        list_add(state->sounds, bomb_hit_sound);
        if (!character_get_invince(character1)) {
          sdl_play_sound(bomb_hit_sound, LOW_VOLUME);
          character_set_hit_time(character1, state->timer);
        }
      }
      else {
        character_change_health(character1, STANDARD_DAMAGE);
        Mix_Chunk *hit_sound = sdl_sound(HURT_CHARACTER_SOUND);
        list_add(state->sounds, hit_sound);
        if (!character_get_invince(character1)) {
          character_set_hit_time(character1, state->timer);
          sdl_play_sound(hit_sound, LOW_VOLUME);
        }
      }
      //check if the character is dead
      if (character_get_health(character1) <= 0) {
      Mix_Chunk *powerup_sound = sdl_sound(DEAD_CHARACTER_SOUND);
      list_add(state->sounds, powerup_sound);
      sdl_play_sound(powerup_sound, LOW_VOLUME);
      body_remove(body2);
      update_win(state, character1);
      }
      else{
        if(!character_get_invince(character1)) {
            create_physics_collision(state->scene, body1, body2, ELASTICITY);
        }
        body_remove(body2);
      }   
    }
    // Player - Goomba Collisions
    else if (compare_character_type(character1, PLAYER_TYPE)
    && compare_character_type(character2, GOOMBA_TYPE)) {
      remove_asset(state->body_assets, body2);
      remove_character(state, body2);
      create_physics_collision(state->scene, body1, body2, 0.0);
      body_remove(body2);
      if (body_get_velocity(body1).y >= 0 || body_get_velocity(body2).y != 0) {
        character_change_health(character1, GOOMBA_DAMAGE);
        Mix_Chunk *goomba_hit_sound = sdl_sound(HURT_CHARACTER_SOUND);
        list_add(state->sounds, goomba_hit_sound);
        if (!character_get_invince(character1)) {
          character_set_hit_time(character1, state->timer);
          sdl_play_sound(goomba_hit_sound, LOW_VOLUME);
        }
      }
      //check if the player is dead
      if (character_get_health(character1) <= 0) {
        Mix_Chunk *mario_dies = sdl_sound(DEAD_CHARACTER_SOUND);
        list_add(state->sounds, mario_dies);
        sdl_play_sound(mario_dies, LOW_VOLUME);
        update_win(state, character1);
      }
    }
  }

void collisions(state_t *state) {
  for (size_t i = 0; i < list_size(state->characters); i++) {
    character_t *character1 = list_get(state->characters, i);
      for (size_t j = i + 1; j < list_size(state->characters); j++) {
        character_t *character2 = list_get(state->characters, j);
        collision_info_t collision = 
          find_collision(character_get_body(character1), 
                          character_get_body(character2)); 
        if (collision.collided) {
          handle_collisions(state, character1, character2, collision);
        }
      }
  }
}

//frees the bullets if they leave the screen
void free_bullets(state_t *state) {
  for (size_t i = 0; i < list_size(state->bullet_assets); i++) {
    asset_t *bullet = list_get(state->bullet_assets, i);
    body_t *body = asset_get_body(bullet);
    if (body_get_centroid(body).x > MAX.x || body_get_centroid(body).x < MIN.x
    || body_get_centroid(body).y < MIN.y) {
      body_remove(body);
      asset_destroy(bullet);
      remove_character(state, body);
      list_remove(state->bullet_assets, i);
      i--;
    }
  }
}

void wrap_edges(body_t *body) {
  vector_t centroid = body_get_centroid(body);
  if (centroid.x > MAX.x) {
    body_set_centroid(body, (vector_t){MIN.x, centroid.y});
  } else if (centroid.x < MIN.x) {
    body_set_centroid(body, (vector_t){MAX.x, centroid.y});
  } else if (centroid.y > MAX.y) {
    body_set_centroid(body, (vector_t){centroid.x, MIN.y});
  } else if (centroid.y < MIN.y) {
    body_set_centroid(body, (vector_t){centroid.x, MAX.y});
  }
}

void update_health_texts(asset_t *health_text, character_t *character) {
  double health = character_get_health(character);
  char *health_text_content = malloc(sizeof(char) * CHAR_SIZE);
  assert (health_text_content != NULL);
  sprintf(health_text_content, "%.0f", health);
  asset_change_text(health_text, health_text_content);
  SDL_Rect *bounding_box = asset_get_bounding_box(health_text);
  vector_t center = body_get_centroid(character_get_body(character));
  bounding_box->x = center.x - bounding_box->w/2;
  asset_render(health_text);
}

void is_loading(state_t *state) {
  state->loading = false;
}

void clear_hearts(state_t *state) {
  asset_t *heart = list_remove(state->heart_assets, 0);
  body_free(asset_get_body(heart));
  asset_destroy(heart);
}

void bombs_only(state_t *state) {
  state->bombs_only = true;
  state->loading = false;
}

void init_game(state_t *state) {
  state->scene = scene_init();
  state->body_assets = list_init(2, (free_func_t)asset_destroy);
  state->bullet_assets = list_init(2, (free_func_t)asset_destroy);
  state->button_assets = list_init(2, (free_func_t)asset_destroy);
  state->heart_assets = list_init(2, (free_func_t)asset_destroy);
  state->characters = list_init(2, (free_func_t)character_free);
  state->sounds = list_init(2, (free_func_t)sdl_free_sound);

  body_t *player1 = make_body(OUTER_RADIUS, INNER_RADIUS, VEC_ZERO);
  body_set_centroid(player1, START_POS1);
  body_t *player2 = make_body(OUTER_RADIUS, INNER_RADIUS, VEC_ZERO);
  body_set_centroid(player2, START_POS2);
  scene_add_body(state->scene, player1);
  scene_add_body(state->scene, player2);

  character_t *mario = character_init(player1, (char *)PLAYER_TYPE, 
  PLAYER_HEALTH, true, 0, H_STEP, (char *)STANDARD_BULLET_TYPE, RIGHT);
  character_t *bowser = character_init(player2, (char *)PLAYER_TYPE, 
  PLAYER_HEALTH, true, 0, H_STEP, (char *)STANDARD_BULLET_TYPE, LEFT);
  list_add(state->characters, mario);
  list_add(state->characters, bowser);

  state->mario_health = asset_make_text(TEXT_FONT, 
  BASE_BOUNDING_BOX, MARIO_HEALTH, MARIO_TEXT_COLOR);
  state->bowser_health = asset_make_text(TEXT_FONT, 
  BASE_BOUNDING_BOX, BOWSER_HEALTH, BOWSER_TEXT_COLOR);

  state->mario_power = asset_make_text(TEXT_FONT, 
  POWER_BOUNDING_BOX, DEFAULT_POWER, POWER_TEXT_COLOR);
  state->bowser_power = asset_make_text(TEXT_FONT, 
  POWER_BOUNDING_BOX, DEFAULT_POWER, POWER_TEXT_COLOR);

  asset_t *loading_screen = asset_make_image(LOADING_PATH, BACKGROUND_BOX);
  list_add(state->body_assets, loading_screen);

  asset_t *player_asset1 =
      asset_make_image_with_body(MARIO_PATH_RIGHT, 
                                  scene_get_body(state->scene, 0));
  asset_t *player_asset2 = 
      asset_make_image_with_body(BOWSER_PATH_LEFT, 
                                  scene_get_body(state->scene, 1));
  list_add(state->body_assets, player_asset1);
  list_add(state->body_assets, player_asset2);


  asset_t *bomb_button_im = asset_make_image(BOMB_BUTTON_PATH, 
                                              BOMB_BOUNDING_BOX);
  asset_t *bomb_button = asset_make_button(BOMB_BOUNDING_BOX, bomb_button_im, 
                                            NULL, (button_handler_t)bombs_only);
  asset_cache_register_button(bomb_button);
  list_add(state->button_assets, bomb_button);

  asset_t *start_button_image = asset_make_image(START_BUTTON_PATH, 
                                                  BUTTON_BOUNDING_BOX);
  asset_t *start_button = asset_make_button(BUTTON_BOUNDING_BOX, 
                                            start_button_image, NULL, 
                                            (button_handler_t)is_loading);
  asset_cache_register_button(start_button);
  list_add(state->button_assets, start_button);
  state->timer = 0.0;
  state->goomba_timer = 0.0;
  state->goomba_count = 0.0;
  state->mystery_timer = 0.0;
  state->mystery_box_count = 0.0;
  state->fire_rate = true;
  state->fire_timer = 0.0;
  state->velocity_timer = 0.0;
  state->frozen = false;
  state->freeze_timer = 0.0;
  state->loading = true;
  state->is_win = false;
  state->bombs_only = false;
  state->winner = true;
  //Mario and Bowser "jump" into the game
  jump(player1, state);
  jump(player2, state);
}

void reset(state_t *state) {
  remove_entire_list(state->body_assets);
  remove_entire_list(state->bullet_assets);
  remove_entire_list(state->button_assets);
  remove_entire_list(state->heart_assets);
  remove_entire_list(state->characters);
  remove_entire_list(state->sounds);
  scene_free(state->scene);
  init_game(state);
  asset_t *restart_button_image = asset_make_image(RESTART_BUTTON, 
                                                  RESTART_BOUNDING_BOX);
  asset_t *restart_button = asset_make_button(RESTART_BOUNDING_BOX, 
                                            restart_button_image, NULL, 
                                            (button_handler_t)reset);
  asset_cache_register_button(restart_button);
  state->restart_button = restart_button;
}


state_t *emscripten_init() {
  asset_cache_init();
  sdl_init(MIN, MAX);
  sdl_sound_init();
  state_t *state = malloc(sizeof(state_t));
  assert(state != NULL);
  srand(time(NULL));
  init_game(state);
  asset_t *restart_button_image = asset_make_image(RESTART_BUTTON, 
                                                  RESTART_BOUNDING_BOX);
  asset_t *restart_button = asset_make_button(RESTART_BOUNDING_BOX, 
                                            restart_button_image, NULL, 
                                            (button_handler_t)reset);
  asset_cache_register_button(restart_button);
  state->restart_button = restart_button;
  sdl_on_key((key_handler_t)on_key);
  return state;
}


bool emscripten_main(state_t *state) {
  character_t *mario = list_get(state->characters, MARIO_CHARACTER);
  character_t *bowser = list_get(state->characters, BOWSER_CHARACTER);
  double dt = time_since_last_tick();
  state->timer += dt;
  //win animation
  if (state->is_win) {
    update_win_screen(state);
  }
  //powerup animations
  else if (state->frozen) {
    state->freeze_timer -= dt;
    if (state->freeze_timer <= 0) {
      state->frozen = false;
      state->freeze_timer = 0.0;
    }
    if (strcmp(character_get_ability(mario), DEFAULT_POWER) != 0 &&
        character_get_power_time(mario) + 1 >= state->timer) {
      powerup_animation(state, mario);
    }
    if (strcmp(character_get_ability(bowser), DEFAULT_POWER) != 0 &&
        character_get_power_time(bowser) + 1 >= state->timer) {
      powerup_animation(state, bowser);
    }
    if (character_get_health_boost(bowser)) {
      health_animation(state, bowser);
    }
    if (character_get_health_boost(mario)) {
      health_animation(state, mario);
    }
  }

  //main game functionality
  else if (!state->loading) {
    body_t *player1 = character_get_body(mario);
    body_t *player2 = character_get_body(bowser);
    state->goomba_timer += dt;
    state->mystery_timer += dt;
    state->velocity_timer += dt;
    character_set_fire_time(mario, character_get_fire_time(mario) + dt);
    character_set_fire_time(bowser, character_get_fire_time(bowser) + dt);
    if (state->goomba_timer >= GOOMBA_INTERVAL) {
      generate_goomba(state);
      state->goomba_timer = 0.0;
    }
    if (character_get_fire_time(mario) >= FIRE_RATE) {
      character_set_fire(mario, true);
    }
    if (character_get_fire_time(bowser) >= FIRE_RATE) {
      character_set_fire(bowser, true);
    }
    if (character_get_power_time(mario) <= state->timer - POWER_LIMIT) {
      power_reset(mario);
      character_set_power_time(mario, 0.0);
    }
    if (character_get_power_time(bowser) <= state->timer - POWER_LIMIT) {
      power_reset(bowser);
      character_set_power_time(bowser, 0.0);
    }
    if (state->velocity_timer >= VELOCITY_INTERVAL) {
      vector_t player1_vel = body_get_velocity(player1);
      player1_vel.x = 0;
      vector_t player2_vel = body_get_velocity(player2);
      player2_vel.x = 0;
      body_set_velocity(player1, player1_vel);
      body_set_velocity(player2, player2_vel);
      state->velocity_timer = 0.0;
    }
    collisions(state);
    apply_gravity(player1, state, START_POS1.y);
    apply_gravity(player2, state, START_POS2.y);
    if (list_size(state->heart_assets) > 0) {
      clear_hearts(state);
      character_set_health_boost(mario, false);
      character_set_health_boost(bowser, false);
    }
    for (size_t i = 0; i < list_size(state->characters); i++) {
      character_t *character = list_get(state->characters, i);
      if (compare_character_type(character, BOMB_BULLET_TYPE)) {
        apply_gravity(character_get_body(character), state, 0);
      }
      if (compare_character_type(character, GOOMBA_TYPE)) {
        apply_gravity(character_get_body(character), state, 
                                          GOOMBA_MINIMUM_HEIGHT);
        wrap_edges(character_get_body(character));
      }
    }
    free_bullets(state);
    if (state->mystery_timer >= MYSTERY_INTERVAL) {
      if (state->mystery_box_count < 2) {
        generate_mystery_box(state);
        state->mystery_box_count++;
      }
      state->mystery_timer = 0.0;
    }
    for (size_t i = 0; i < 2; i++) {
      wrap_edges(scene_get_body(state->scene, i));
    }

    //asset changing for sprite switching
    asset_t *mario_asset = list_get(state->body_assets, MARIO_ASSET);
    asset_t *bowser_asset = list_get(state->body_assets, BOWSER_ASSET);
    if (character_get_direction(mario)) {
      if (character_get_hit_time(mario) + 1 <= state->timer) {
        if (strcmp(character_get_ability(mario), DEFAULT_POWER) == 0) {
          asset_change_texture(mario_asset, MARIO_PATH_RIGHT);
        }
        else {
          asset_change_texture(mario_asset, POWERED_MARIO_PATH_RIGHT);
        }
      }
      else {
        asset_change_texture(mario_asset, (char *)MARIO_ATTACKED);
      }
      }
    else {
      if (character_get_hit_time(mario) + 1 <= state->timer) {
        if (strcmp(character_get_ability(mario), DEFAULT_POWER) == 0) {
          asset_change_texture(mario_asset, MARIO_PATH_LEFT);
        }
        else {
          asset_change_texture(mario_asset, POWERED_MARIO_PATH_LEFT);
        }
    }
    else {
      asset_change_texture(mario_asset, (char *)MARIO_ATTACKED);
    }
    }
    if (character_get_direction(bowser)) {
      if (character_get_hit_time(bowser) + 1 <= state->timer) {
        if (strcmp(character_get_ability(bowser), DEFAULT_POWER) == 0) {
          asset_change_texture(bowser_asset, BOWSER_PATH_RIGHT);
        }
        else {
          asset_change_texture(bowser_asset, POWERED_BOWSER_PATH_RIGHT);
        }
      }
      else {
        asset_change_texture(bowser_asset, BOWSER_ATTACKED_RIGHT);
      }
      }
    else {
      if (character_get_hit_time(bowser) + 1 <= state->timer) {
        if (strcmp(character_get_ability(bowser), DEFAULT_POWER) == 0) {
          asset_change_texture(bowser_asset, BOWSER_PATH_LEFT);
        }
        else {
          asset_change_texture(bowser_asset, POWERED_BOWSER_PATH_LEFT);
        }
      }
      else {
        asset_change_texture(bowser_asset, BOWSER_ATTACKED_LEFT);
      }
    }
    sdl_clear();
  }
  //win state rendering
  if (state->is_win) {
    if (state->winner) {
      asset_change_texture(list_get(state->body_assets, MARIO_ASSET), 
                                    MARIO_VICTORY_SKIN);
      asset_change_texture(list_get(state->body_assets, BOWSER_ASSET), 
                                    BOWSER_LOSER_SKIN);
    }
    else {
      asset_change_texture(list_get(state->body_assets, MARIO_ASSET), 
                                    MARIO_LOSER_SKIN);
      asset_change_texture(list_get(state->body_assets, BOWSER_ASSET), 
                                    BOWSER_VICTORY_SKIN);
    }
    asset_render(list_get(state->body_assets, BACKGROUND_ASSET));
    asset_render(list_get(state->body_assets, MARIO_ASSET));
    asset_render(list_get(state->body_assets, BOWSER_ASSET));
    asset_render(state->restart_button);
  }
  //main game rendering
  else if (!state->loading) {
    asset_change_texture(list_get(state->body_assets, BACKGROUND_ASSET), 
                                  BACKGROUND_PATH);
    for (size_t i = 0; i < list_size(state->body_assets); i++) {
      asset_render(list_get(state->body_assets, i));
    }
    for (size_t i = 0; i < list_size(state->bullet_assets); i++) {
      asset_render(list_get(state->bullet_assets, i));
    }
    for (size_t i = 0; i < list_size(state->heart_assets); i++) {
      if (state->frozen){
        asset_render(list_get(state->heart_assets, i));
      }
    }
    update_health_texts(state->mario_health, mario);
    update_health_texts(state->bowser_health, bowser);
    update_power_texts(state->mario_power, mario);
    update_power_texts(state->mario_power, bowser);
  }
  //loading screen rendering
  else {
    asset_render(list_get(state->body_assets, BACKGROUND_ASSET));
    for (size_t i = 0; i < list_size(state->button_assets); i++) {
      asset_render(list_get(state->button_assets, i));
    }
  }
  sdl_show();
  //only moves the objects if the game is not frozen or loading
  if (!state->frozen && !state->loading) {
  scene_tick(state->scene, dt);
  }
  return false;
}

void emscripten_free(state_t *state) {
   list_free(state->body_assets);
   list_free(state->heart_assets);
   list_free(state->button_assets);
   list_free(state->sounds);
   list_free(state->characters);
   asset_cache_destroy();
   scene_free(state->scene);
   free(state);
}

