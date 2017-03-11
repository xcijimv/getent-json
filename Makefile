CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -O2 -g -std=gnu99
LDFLAGS=-ljson-c
MAIN=src/getent-json.c
SOURCES=\
				src/getent_passwd.c \
				src/getent_group.c \
				src/getent_shadow.c \
				src/getent_gshadow.c \
				src/getent_hosts.c \
				src/getent_aliases.c \
				src/getent_protocols.c \
				src/getent_services.c \
				src/getent_rpc.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY_NAME=getent-json
LIBRARY=build/libgetent-json.so
EXECUTABLE=bin/getent-json

all: lib bin

lib: $(LIBRARY)
$(LIBRARY): $(OBJECTS)
	@mkdir -p build
	$(CC) -shared $(LDFLAGS) $^ -o $@

bin: $(EXECUTABLE)
$(EXECUTABLE): $(MAIN)
	@mkdir -p bin
	$(CC) $(LDFLAGS) -Lbuild -l$(LIBRARY_NAME) $^ -o $@

verify:
	declare -a databases=("passwd" "group" "shadow" "gshadow" "hosts" "aliases" "protocols" "services" "rpc") ; \
	for database in "$${databases[@]}" ; do \
	  LD_LIBRARY_PATH=build "$(EXECUTABLE)" "$$database" ; \
	done
	LD_LIBRARY_PATH=build "$(EXECUTABLE)" --help || exit 0
	LD_LIBRARY_PATH=build "$(EXECUTABLE)" --version || exit 0

.c.o:
	$(CC) -fPIC $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o build/libgetent-json.so bin/getent-json
