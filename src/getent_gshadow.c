#include <sys/types.h>
#include <gshadow.h>
#include <json-c/json.h>

#include "getent_gshadow.h"

json_object *getent_gshadow() {
  struct sgrp *e;
  struct json_object *j_arr = json_object_new_array();
  char **member;

  setsgent();

  while ((e = getsgent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->sg_namp));
    json_object_object_add(o, "password", json_object_new_string(e->sg_passwd));

    struct json_object *j_arr_members = json_object_new_array();
    member = e->sg_mem;
    for (; *member != NULL; member++) {
      json_object_array_add(j_arr_members, json_object_new_string(*member));
    }
    json_object_object_add(o, "members", j_arr_members);

    struct json_object *j_arr_admins = json_object_new_array();
    member = e->sg_adm;
    for (; *member != NULL; member++) {
      json_object_array_add(j_arr_admins, json_object_new_string(*member));
    }
    json_object_object_add(o, "admins", j_arr_admins);

    json_object_array_add(j_arr, o);
  }

  endsgent();

  return j_arr;
}
