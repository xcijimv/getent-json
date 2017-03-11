#include <sys/types.h>
#include <shadow.h>
#include <json-c/json.h>

#include "getent_shadow.h"

json_object *getent_shadow() {
  struct spwd *e;
  struct json_object *j_arr = json_object_new_array();

  setspent();

  while ((e = getspent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "name", json_object_new_string(e->sp_namp));
    json_object_object_add(o, "password", json_object_new_string(e->sp_pwdp));

    json_object_object_add(o, "last_change", json_object_new_int(e->sp_lstchg));
    json_object_object_add(
        o, "days_until_change_required", json_object_new_int(e->sp_min));
    json_object_object_add(
        o, "days_until_change_allowed", json_object_new_int(e->sp_max));
    json_object_object_add(
        o, "days_until_warning", json_object_new_int(e->sp_warn));
    json_object_object_add(
        o, "days_until_inactive", json_object_new_int(e->sp_inact));
    json_object_object_add(
        o, "days_until_inactive", json_object_new_int(e->sp_expire));
    json_object_object_add(o, "future_flag", json_object_new_int(e->sp_flag));

    json_object_array_add(j_arr, o);
  }

  endspent();

  return j_arr;
}
