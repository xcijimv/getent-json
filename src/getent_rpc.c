#include <netdb.h>
#include <json-c/json.h>

#include "getent_rpc.h"

json_object *getent_rpc(int stay_open) {
  struct rpcent *e;
  struct json_object *j_arr = json_object_new_array();
  char **alias;

  if (stay_open < 0 || stay_open >= 1) {
    return NULL;
  }

  setrpcent(stay_open);

  while ((e = getrpcent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->r_name));
    json_object_object_add(
        o, "program_number", json_object_new_int(e->r_number));

    struct json_object *j_arr_aliases = json_object_new_array();
    alias = e->r_aliases;
    for (; *alias != NULL; alias++) {
      json_object_array_add(j_arr_aliases, json_object_new_string(*alias));
    }
    json_object_object_add(o, "aliases", j_arr_aliases);

    json_object_array_add(j_arr, o);
  }

  endrpcent();

  return j_arr;
}
