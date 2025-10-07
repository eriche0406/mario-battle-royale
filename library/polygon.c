#include "polygon.h"
#include "vector.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

typedef struct polygon {
  list_t *points;
  vector_t velocity;
  double rotation_speed;
  rgb_color_t *color;
} polygon_t;

polygon_t *polygon_init(list_t *points, vector_t initial_velocity,
                        double rotation_speed, double red, double green,
                        double blue) {

  polygon_t *new = malloc(sizeof(polygon_t));
  assert(new != NULL);

  new->points = points;
  new->velocity = initial_velocity;
  new->rotation_speed = rotation_speed;
  new->color = color_init(red, green, blue);

  return new;
}

list_t *polygon_get_points(polygon_t *polygon) {
  assert(polygon != NULL);
  return polygon->points;
}

void polygon_move(polygon_t *polygon, double time_elapsed) {
  assert(polygon != NULL);

  vector_t translate = vec_multiply(time_elapsed, polygon->velocity);
  polygon_translate(polygon, translate);

  double angle = polygon->rotation_speed * time_elapsed;
  vector_t centroid = polygon_centroid(polygon);
  polygon_rotate(polygon, angle, centroid);
}

void polygon_set_velocity(polygon_t *polygon, vector_t vel) {
  assert(polygon != NULL);
  polygon->velocity = vel;
}

void polygon_free(polygon_t *polygon) {
  assert(polygon != NULL);
  list_free(polygon->points);
  color_free(polygon->color);
  free(polygon);
}

vector_t *polygon_get_velocity(polygon_t *polygon) {
  assert(polygon != NULL);
  return &polygon->velocity;
}

double polygon_area(polygon_t *polygon) {
  assert(polygon != NULL);

  double area = 0.0;
  size_t size = list_size(polygon->points);

  if (size < 3) {
    return area;
  }

  for (size_t i = 0; i < size; i++) {
    vector_t area1 = *(vector_t *)list_get(polygon->points, i);
    vector_t area2 = *(vector_t *)list_get(polygon->points, (i + 1) % size);
    area += area1.x * area2.y - area2.x * area1.y;
  }
  area = fabs(area) / 2.0;
  return area;
}

vector_t polygon_centroid(polygon_t *polygon) {
  assert(polygon != NULL);

  vector_t centroid = (vector_t){.x = 0.0, .y = 0.0};
  size_t size = list_size(polygon->points);
  double area = polygon_area(polygon);
  for (size_t i = 0; i < size; i++) {
    vector_t vec1 = *((vector_t *)(list_get(polygon->points, i)));
    vector_t vec2 = *((vector_t *)(list_get(polygon->points, (i + 1) % size)));
    centroid.x += (vec1.x + vec2.x) * vec_cross(vec1, vec2);
    centroid.y += (vec1.y + vec2.y) * vec_cross(vec1, vec2);
  }

  return vec_multiply(1 / (6.0 * area), centroid);
}

void polygon_translate(polygon_t *polygon, vector_t translation) {
  assert(polygon != NULL);

  list_t *points = polygon_get_points(polygon);
  size_t size = list_size(points);
  for (size_t i = 0; i < size; i++) {
    vector_t *point = list_get(points, i);
    *point = vec_add(*point, translation);
  }
}

void polygon_rotate(polygon_t *polygon, double angle, vector_t point) {
  assert(polygon != NULL);

  list_t *points = polygon->points;
  size_t size = list_size(points);

  for (size_t i = 0; i < size; i++) {
    vector_t *vec = list_get(points, i);
    vector_t translated = vec_subtract(*vec, point);
    vector_t rotated = vec_rotate(translated, angle);
    *vec = vec_add(rotated, point);
  }
}

rgb_color_t *polygon_get_color(polygon_t *polygon) {
  assert(polygon != NULL);
  return polygon->color;
}

void polygon_set_color(polygon_t *polygon, rgb_color_t *color) {
  polygon->color = color;
}

void polygon_set_center(polygon_t *polygon, vector_t centroid) {
  vector_t center = polygon_get_center(polygon);
  polygon_translate(polygon, vec_subtract(centroid, center));
}

vector_t polygon_get_center(polygon_t *polygon) {
  return polygon_centroid(polygon);
}

void polygon_set_rotation(polygon_t *polygon, double rot) {
  polygon_rotate(polygon, rot - polygon->rotation_speed,
                 polygon_get_center(polygon));
}

double polygon_get_rotation(polygon_t *polygon) {
  return polygon->rotation_speed;
}
