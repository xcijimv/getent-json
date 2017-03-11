#include <netdb.h>
#include <json-c/json.h>

#include "getent_protocols.h"

json_object *getent_protocols(int stay_open) {
  struct protoent *e;
  struct json_object *j_arr = json_object_new_array();
  char **alias;

  if (stay_open < 0 || stay_open >= 1) {
    return NULL;
  }

  setprotoent(stay_open);

  while ((e = getprotoent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->p_name));
    json_object_object_add(o, "number", json_object_new_int(e->p_proto));

    struct json_object *j_arp_aliases = json_object_new_array();
    alias = e->p_aliases;
    for (; *alias != NULL; alias++) {
      json_object_array_add(j_arp_aliases, json_object_new_string(*alias));
    }
    json_object_object_add(o, "aliases", j_arp_aliases);

    json_object_array_add(j_arr, o);
  }

  endprotoent();

  return j_arr;
}
