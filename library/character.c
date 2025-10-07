#include "character.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const char *DEFAULT = "NONE";

struct character {
  body_t *body;
  char *type;
  double health;
  double damage;
  double fire_timer;
  double translation;
  double power_time;
  double hit_time;
  bool fire;
  bool invincible;
  char *ability;
  char *bullet_type;
  bool direction;
  bool health_boost;
};

character_t *character_init(body_t *body, char* type, double health, bool fire, 
                            double fire_timer, double translation, char* bullet
                            , bool direction){
    character_t *character = malloc(sizeof(character_t));
    assert(character);
    character->body = body;
    character->health = health;
    character->type = type;
    character->fire = fire;
    character->fire_timer = fire_timer;
    character->translation = translation;
    character->invincible = false;
    character->power_time = 0.0;
    character->ability = (char *)DEFAULT;
    character->bullet_type = bullet;
    character->direction = direction; //true means right and false means left
    character->hit_time = 0.0;
    character->health_boost = false;
    return character;
}

double character_get_health(character_t *character){
    return character->health;
}

void character_set_health(character_t *character, double health){
    character->health = health;
}

void character_change_health(character_t *character, double damage) {
    if (!character->invincible || damage >= 0){
    character->health += damage;
    }
}

char *character_get_type(character_t *character){
    return character->type;
}

void character_set_type(character_t *character, char *type){
    character->type = type;
}

body_t *character_get_body(character_t *character){
    return character->body;
}

void character_set_body(character_t *character, body_t *body){
    character->body = body;
}

bool character_get_fire(character_t *character){
    return character->fire;
}

void character_set_fire(character_t *character, bool fire){
    character->fire = fire;
}

double character_get_fire_time(character_t *character){
    return character->fire_timer;
}

void character_set_fire_time(character_t *character, double time){
    character->fire_timer = time;
}

double character_get_translation(character_t *character){
    return character->translation;
}

void character_set_translation(character_t *character, double translation){
    character->translation = translation;
}

bool character_get_invince(character_t *character){
    return character->invincible;
}

void character_set_invince(character_t *character, bool invince){
    character->invincible = invince;
}

double character_get_power_time(character_t *character){
    return character->power_time;
}

void character_set_power_time(character_t *character, double power_time){
    character->power_time = power_time;
}

void character_set_ability(character_t *character, char *ability){
    character->ability = ability;
}

char *character_get_ability(character_t *character){
    return character->ability;
}

void character_free(character_t *character){
    free(character);
}

char *character_get_bullet_type(character_t *character) {
    return character->bullet_type;
}

void character_set_bullet_type(character_t *character, const char *bullet) {
    character->bullet_type = (char *)bullet;
}

bool character_get_direction(character_t *character) {
    return character->direction;
}

void character_set_direction(character_t *character, bool direction) {
    character->direction = direction;
}

double character_get_hit_time(character_t *character){
    return character->hit_time;
}

void character_set_hit_time(character_t *character, double hit_time){
    character->hit_time = hit_time;
}

bool character_get_health_boost(character_t *character){
    return character->health_boost;
}

void character_set_health_boost(character_t *character, bool health_boost){
    character->health_boost = health_boost;
}