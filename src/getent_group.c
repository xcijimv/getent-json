#include <grp.h>
#include <json-c/json.h>

#include "getent_group.h"

json_object *getent_group() {
  struct group *e;
  struct json_object *j_arr = json_object_new_array();
  char **member;

  setgrent();

  while ((e = getgrent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "gid", json_object_new_int(e->gr_gid));
    json_object_object_add(o, "name", json_object_new_string(e->gr_name));
    json_object_object_add(o, "password", json_object_new_string(e->gr_passwd));

    struct json_object *j_arr_members = json_object_new_array();
    member = e->gr_mem;
    for (; *member != NULL; member++) {
      json_object_array_add(j_arr_members, json_object_new_string(*member));
    }
    json_object_object_add(o, "members", j_arr_members);

    json_object_array_add(j_arr, o);
  }

  endgrent();

  return j_arr;
}
