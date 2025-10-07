#include "body.h"

/**
 *Represents the different types of characters that
 can appear on the screen during the game (anything with a body or that needs
 to be rendered in that is not the background)
 */
typedef struct character character_t;

/**
 *Initializes the character objects
 @param body body for the character
 @param type type of character (ex. player, bullet, etc.)
 @param health current health of character
 @param fire whether the character can currently fire a bullet
 @param fire_timer interval between when it can fire
 @param translation current velocity
 @param bullet type of bullet (if it is a bullet)
 @param direction the direction the object is facing
 */
character_t *character_init(body_t *body, char* type, double health, bool fire,
 double fire_timer, double translation, char* bullet, bool direction);

/**
 *gets the health of the character
 @param character character to check
 */
double character_get_health(character_t *character);

/**
 *sets the health of the character
 @param character character to check
 @param health new value for the health
 */
void character_set_health(character_t *character, double health);

/**
 *gets the type of the character
 @param character character to check
 */
char *character_get_type(character_t *character);

/**
 *sets the type of the character
 @param character character to check
 @param type new type for the character
 */
void character_set_type(character_t *character, char *type);

/**
 *gets the body of the character
 @param character character to check
 */
body_t *character_get_body(character_t *character);

/**
 *sets the body of the new character
 @param character character to check
 @param body new body to set the character to
 */
void character_set_body(character_t *character, body_t *body);

/**
 *frees the character object
 @param character character to check
 */
void character_free(character_t *character);

/**
 *changes health based on bullet damage
 @param character character to check
 @param damage damage taken by character (used to decrement the health)
 */
void character_change_health(character_t *character, double damage);

/**
 *gets whether the character can fire or not
 @param character character to check
 */
bool character_get_fire(character_t *character);

/**
 *sets whether the character can fire or not
 @param character character to check
 @param fire boolean to set it to
 */
void character_set_fire(character_t *character, bool fire);

/**
 *gets the fire rate of the character
 @param character character to check
 */
double character_get_fire_time(character_t *character);

/**
 *gets the translation of the new character
 @param character character to check
 */
double character_get_translation(character_t *character);

/**
 *sets the new translation of the character
 @param character character to check
 @param translation new translation for the character
 */
void character_set_translation(character_t *character, double translation);

/**
 *sets the fire rate of the character
 @param character character to check
 @param time new fire rate for the character
 */
void character_set_fire_time(character_t *character, double time);

/**
 *sets invincibility status of the character
 @param character character to check
 @param invince whether the character should be set to invincible
 */
void character_set_invince(character_t *character, bool invince);

/**
 *gets invincibility status of the character
 @param character character to check
 */
bool character_get_invince(character_t *character);

/**
 *gets the power-up time for the character
 @param character character to check
 */
double character_get_power_time(character_t *character);

/**
 *sets the new power-up time for the character
 @param character character to check
 @param power_time the new power-up time for the character
 */
void character_set_power_time(character_t *character, double power_time);

/**
 *sets the ability of the character
 @param character character to check
 @param ability new ability for the character
 */
void character_set_ability(character_t *character, char *ability);

/**
 *gets ability status of the character
 @param character character to check
 */
char *character_get_ability(character_t *character);

/**
 *gets the power-up time of the character
 @param character character to check
 */
double character_get_power_time(character_t *character);

/**
 *gets the bullet type of the character
 @param character character to check
 */
char *character_get_bullet_type(character_t *character);

/**
 *sets the bullet type of the character
 @param character character to check
 @param bullet the new bullet type
 */
void character_set_bullet_type(character_t *character, const char* bullet);

/**
 *gets the direction the character is facing
 @param character character to check
 */
bool character_get_direction(character_t *character);

/**
 *sets the direction that the character would be facing
 @param character character to check
 @param direction new direction for the character
 */
void character_set_direction(character_t *character, bool direction);

/**
 *gets the hit time of the character
 @param character character to check
 */
double character_get_hit_time(character_t *character);

/**
 *sets the hit time of the character
 @param character character to check
 @param hit_time new hit time to set it to
 */
void character_set_hit_time(character_t *character, double hit_time);

/**
 *set the health boost of the character
 @param character character to check
 @param health_boost = the health boost to be changed for the character
 */
void character_set_health_boost(character_t *character, bool health_boost);

/**
 *gets health boost status of the character
 @param character character to check
 */
bool character_get_health_boost(character_t *character);