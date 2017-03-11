#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <json-c/json.h>

#include "getent_passwd.h"
#include "getent_group.h"
#include "getent_shadow.h"
#include "getent_gshadow.h"
#include "getent_hosts.h"
#include "getent_aliases.h"
#include "getent_protocols.h"
#include "getent_services.h"
#include "getent_rpc.h"

const char *app_name = "getent-json";
const char *app_version = "0.1.0";

void print_version() {
  printf("%s %s\n", app_name, app_version);
}

void print_help() {
  printf("Usage: %s DATABASE\n\n", app_name);
  printf("Options:\n\n");
  printf("  --help    Print help\n");
  printf("  --version Print version\n");
  printf("\n");
  printf("Supported databases:\n\n");
  printf("  passwd group shadow gshadow hosts aliases protocols services rpc");
  printf("\n");
  printf("\nVersion: %s %s\n", app_name, app_version);
}

int main(int argc, char **argv) {
  int c;
  static int version_flag;
  static int help_flag;
  int option_index;

  int stay_open = 0;
  char *database_name = NULL;
  struct json_object *j_obj = NULL;
  const char *j_str = NULL;

  while (1) {
    static struct option long_options[] = {
      {"version", no_argument, &version_flag, 1},
      {"help", no_argument, &help_flag, 1},
      {NULL, 0, NULL, 0},
    };
    c = getopt_long(argc, argv, "", long_options, &option_index);

    if (c == -1) {
      break;
    }
  }

  if (help_flag || argc != 2) {
    print_help();
    exit(EXIT_FAILURE);
  } else if (version_flag) {
    print_version();
    exit(EXIT_FAILURE);
  }

  database_name = (char *) malloc(strlen(argv[1]) + 1);
  strcpy(database_name, argv[1]);

  if (database_name == NULL) {
    print_help();
    exit(EXIT_FAILURE);
  }

  if (strcmp(database_name, "passwd") == 0) {
    j_obj = getent_passwd();
  } else if (strcmp(database_name, "group") == 0) {
    j_obj = getent_group();
  } else if (strcmp(database_name, "shadow") == 0) {
    j_obj = getent_shadow();
  } else if (strcmp(database_name, "gshadow") == 0) {
    j_obj = getent_gshadow();
  } else if (strcmp(database_name, "hosts") == 0) {
    j_obj = getent_hosts(stay_open);
  } else if (strcmp(database_name, "aliases") == 0) {
    j_obj = getent_aliases();
  } else if (strcmp(database_name, "protocols") == 0) {
    j_obj = getent_protocols(stay_open);
  } else if (strcmp(database_name, "services") == 0) {
    j_obj = getent_services(stay_open);
  } else if (strcmp(database_name, "rpc") == 0) {
    j_obj = getent_rpc(stay_open);
  }

  if (database_name) { free(database_name); }

  if (j_obj == NULL) {
    printf("database not found or not supported\n");
    exit(EXIT_FAILURE);
  }

  j_str = json_object_to_json_string(j_obj);
  printf("%s\n", j_str);

  if (j_obj) { free(j_obj); }

  exit(EXIT_SUCCESS);
}
