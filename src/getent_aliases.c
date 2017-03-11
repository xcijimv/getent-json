#include <aliases.h>
#include <json-c/json.h>

#include "getent_aliases.h"

json_object *getent_aliases() {
  struct aliasent *e;
  struct json_object *j_arr = json_object_new_array();
  char **member;

  setaliasent();

  while ((e = getaliasent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->alias_name));
    json_object_object_add(o, "local", json_object_new_int(e->alias_local));

    struct json_object *j_arr_members = json_object_new_array();
    member = e->alias_members;
    for (; *member != NULL; member++) {
      json_object_array_add(j_arr_members, json_object_new_string(*member));
    }
    json_object_object_add(o, "members", j_arr_members);

    json_object_array_add(j_arr, o);
  }

  endaliasent();

  return j_arr;
}
