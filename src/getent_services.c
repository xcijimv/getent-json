#include <netdb.h>
#include <json-c/json.h>

#include "getent_services.h"

json_object *getent_services(int stay_open) {
  struct servent *e;
  struct json_object *j_arr = json_object_new_array();
  char **alias;

  if (stay_open < 0 || stay_open >= 1) {
    return NULL;
  }

  setservent(stay_open);

  while ((e = getservent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->s_name));
    json_object_object_add(o, "proto", json_object_new_string(e->s_proto));
    json_object_object_add(o, "port", json_object_new_int(e->s_port));

    struct json_object *j_ars_aliases = json_object_new_array();
    alias = e->s_aliases;
    for (; *alias != NULL; alias++) {
      json_object_array_add(j_ars_aliases, json_object_new_string(*alias));
    }
    json_object_object_add(o, "aliases", j_ars_aliases);

    json_object_array_add(j_arr, o);
  }

  endservent();

  return j_arr;
}
