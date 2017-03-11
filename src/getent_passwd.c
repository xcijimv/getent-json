#include <sys/types.h>
#include <pwd.h>
#include <json-c/json.h>

#include "getent_passwd.h"

json_object *getent_passwd() {
  struct passwd *e;
  struct json_object *j_arr = json_object_new_array();

  setpwent();

  while ((e = getpwent()) != NULL) {
    struct json_object *o = json_object_new_object();
    json_object_object_add(o, "uid", json_object_new_int(e->pw_uid));
    json_object_object_add(o, "gid", json_object_new_int(e->pw_gid));
    json_object_object_add(o, "username", json_object_new_string(e->pw_name));
    json_object_object_add(o, "password", json_object_new_string(e->pw_passwd));
    json_object_object_add(o, "comment", json_object_new_string(e->pw_gecos));
    json_object_object_add(o, "home", json_object_new_string(e->pw_dir));
    json_object_object_add(o, "shell", json_object_new_string(e->pw_shell));

    json_object_array_add(j_arr, o);
  }

  endpwent();

  return j_arr;
}
