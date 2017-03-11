#include <netdb.h>
#include <arpa/inet.h>
#include <json-c/json.h>

#include "getent_hosts.h"

json_object *getent_hosts(int stay_open) {
  struct hostent *e;
  struct json_object *j_arr = json_object_new_array();
  char **alias;
  struct in_addr **address;

  if (stay_open < 0 || stay_open >= 1) {
    return NULL;
  }

  sethostent(stay_open);

  while ((e = gethostent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->h_name));
    json_object_object_add(
        o, "address_type", json_object_new_int(e->h_addrtype));

    struct json_object *j_arr_addresses = json_object_new_array();
    address = (struct in_addr **) e->h_addr_list;
    for (; *address != NULL; address++) {
      json_object_array_add(
          j_arr_addresses, json_object_new_string(
            inet_ntoa(*(*address))));
    }
    json_object_object_add(o, "addresses", j_arr_addresses);

    struct json_object *j_arr_aliases = json_object_new_array();
    alias = e->h_aliases;
    for (; *alias != NULL; alias++) {
      json_object_array_add(j_arr_aliases, json_object_new_string(*alias));
    }
    json_object_object_add(o, "aliases", j_arr_aliases);

    json_object_array_add(j_arr, o);
  }

  endhostent();

  return j_arr;
}
