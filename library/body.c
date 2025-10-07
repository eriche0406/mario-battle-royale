#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"

const double INITIAL_ROTATION = 0;

struct body {
  polygon_t *poly;

  double mass;
  vector_t force;
  vector_t impulse;
  bool removed;
  void *info;
  free_func_t info_freer;
};

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color,
                            void *info, free_func_t info_freer) {
  assert(shape != NULL);
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);
  body->poly = polygon_init(shape, VEC_ZERO, INITIAL_ROTATION, color.r, color.g,
                            color.b);
  body->mass = mass;
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
  body->removed = false;
  body->info = info;
  body->info_freer = info_freer;
  return body;
}

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  body_t *body = body_init_with_info(shape, mass, color, NULL, NULL);
  return body;
}

void body_free(body_t *body) {
  if (body->info_freer != NULL) {
    body->info_freer(body->info);
  }
  polygon_free(body->poly);
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *shape = polygon_get_points(body->poly);
  list_t *points = list_init(list_size(shape), free);
  for (size_t i = 0; i < list_size(shape); i++) {
    vector_t *point = malloc(sizeof(vector_t));
    assert(point != NULL);
    *point = *(vector_t *)list_get(shape, i);
    list_add(points, point);
  }
  return points;
}

vector_t body_get_centroid(body_t *body) {
  return polygon_get_center(body->poly);
}

vector_t body_get_velocity(body_t *body) {
  return *polygon_get_velocity(body->poly);
}

rgb_color_t *body_get_color(body_t *body) {
  return polygon_get_color(body->poly);
}

polygon_t *body_get_polygon(body_t *body) { return body->poly; }

void *body_get_info(body_t *body) { return body->info; }

void body_set_color(body_t *body, rgb_color_t *col) {
  polygon_set_color(body->poly, col);
}

void body_set_centroid(body_t *body, vector_t x) {
  polygon_set_center(body->poly, x);
}

void body_set_velocity(body_t *body, vector_t v) {
  polygon_set_velocity(body->poly, v);
}

double body_get_rotation(body_t *body) {
  return polygon_get_rotation(body->poly);
}

void body_set_rotation(body_t *body, double angle) {
  polygon_set_rotation(body->poly, angle);
}

void body_tick(body_t *body, double dt) {
  if (body->poly == NULL) {
    return;
  }
  // Acceleration = Force / Mass
  vector_t acc = vec_multiply(1.0 / body->mass, body->force);
  vector_t velocity_change = vec_multiply(dt, acc);
  vector_t poly_velocity = *polygon_get_velocity(body->poly);
  vector_t momentum_change = vec_multiply(1.0 / body->mass, body->impulse);
  vector_t net_move = vec_add(momentum_change, velocity_change);
  polygon_set_velocity(body->poly, vec_add(poly_velocity, net_move));
  vector_t new_velocity = *polygon_get_velocity(body->poly);
  vector_t average_velocity =
      vec_multiply(dt / 2, vec_add(new_velocity, poly_velocity));
  polygon_translate(body->poly, average_velocity);
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
}

double body_get_mass(body_t *body) { return body->mass; }

void body_add_force(body_t *body, vector_t force) {
  body->force = vec_add(body->force, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulse = vec_add(body->impulse, impulse);
}

void body_remove(body_t *body) { body->removed = true; }

bool body_is_removed(body_t *body) { return body->removed; }

void body_reset(body_t *body) {
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
}

// double body_get_health(body_t *body) {
//   return body->health;
// }

// double body_set_health(body_t *body, double health) {
//   body->health = health;
// }