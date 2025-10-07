#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "forces.h"
#include "scene.h"

const double BODY_NUMBER = 10;
const double AUX_NUMBER = 20;

struct scene {
  ssize_t num_bodies;
  list_t *bodies;
  list_t *force_creators;
};

typedef struct {
  force_creator_t force_creator;
  void *aux;
  list_t *bodies;
} force_creator_info_t;

force_creator_info_t *force_creator_info_init(force_creator_t force_creator,
                                              void *aux, list_t *bodies) {
  force_creator_info_t *result = malloc(sizeof(force_creator_info_t));
  assert(result != NULL);

  result->force_creator = force_creator;
  result->aux = aux;
  result->bodies = bodies;
  return result;
}

void force_creator_info_free(force_creator_info_t *force_info) {
  list_free(force_info->bodies);
  list_t *bodies_aux = ((body_aux_t *)force_info->aux)->bodies;
  list_free(bodies_aux);
  free(force_info->aux);
  free(force_info);
}

scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->bodies = list_init(BODY_NUMBER, (free_func_t)body_free);
  scene->num_bodies = 0;
  scene->force_creators =
      list_init(AUX_NUMBER, (free_func_t)force_creator_info_free);
  return scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->bodies);
  list_free(scene->force_creators);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return scene->num_bodies; }

body_t *scene_get_body(scene_t *scene, size_t index) {
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
  scene->num_bodies++;
}

void scene_remove_body(scene_t *scene, size_t index) {
  body_t *body = list_get(scene->bodies, index);
  body_remove(body);
}

void scene_add_force_creator(scene_t *scene, force_creator_t force_creator,
                             void *aux) {
  scene_add_bodies_force_creator(scene, force_creator, aux, list_init(0, free));
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies) {
  force_creator_info_t *info = force_creator_info_init(forcer, aux, bodies);
  list_add(scene->force_creators, info);
}

void scene_tick(scene_t *scene, double dt) {
  for (size_t j = 0; j < list_size(scene->force_creators); j++) {
    force_creator_info_t *force_info = list_get(scene->force_creators, j);
    force_info->force_creator(force_info->aux);
  }

  for (ssize_t i = 0; i < scene->num_bodies; i++) {
    body_t *body = list_get(scene->bodies, i);
    if (body_is_removed(body)) {
      for (size_t j = 0; j < list_size(scene->force_creators); j++) {
        force_creator_info_t *info = list_get(scene->force_creators, j);
        list_t *bodies = info->bodies;
        for (size_t k = 0; k < list_size(bodies); k++) {
          if (list_get(bodies, k) == body) {
            list_remove(scene->force_creators, j);
            j--;
            force_creator_info_free(info);
            break;
          }
        }
      }
      list_remove(scene->bodies, i);
      scene->num_bodies--;
      body_free(body);
      i--;
    } else {
      body_tick(body, dt);
    }
  }
}
