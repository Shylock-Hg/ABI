#CC = gcc
LN = ln
INSTALL = install
RM = rm
MKDIR = mkdir

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

CFLAGS_LOCAL = -Wall -g -std=gnu99 -coverage
CFLAGS_LOCAL += $(CFLAGS)

prefix = /usr/local

DIR_BUILD = .build

APP_SOURCE = abi.c
APP_OBJECT = abi.o

LIB_SOURCES = source/abi_bf.c \
        source/abi_tokens.c
LIB_OBJECTS = $(patsubst %.c, %.o, $(notdir $(LIB_SOURCES)))
LIBVERSION = 0.0.1
LIBNAME = abi
LIB_SO_ABI = lib$(LIBNAME).so.$(LIBVERSION)

APP = abi

DEPFILES = $(patsubst %.o, %.d, $(addprefix $(DIR_BUILD)/, $(LIB_OBJECTS)) $(DIR_BUILD)/$(APP_OBJECT))

# set c sources search path
vpath %.c $(sort $(dir $(LIB_SOURCES)))

.PHONY : all clean install uninstall test
all : $(DIR_BUILD) $(DIR_BUILD)/$(APP)

$(DIR_BUILD)/$(APP) : $(DIR_BUILD)/$(APP_OBJECT) $(DIR_BUILD)/$(LIB_SO_ABI) | Makefile
	$(CC) $(CFLAGS_LOCAL) -o $@ $< -L$(shell pwd)/$(DIR_BUILD) -l$(LIBNAME)

$(DIR_BUILD)/$(APP_OBJECT) : $(APP_SOURCE) | Makefile 
	$(CC) -MT $@ -MMD -MP -MF $*.d $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/$(LIB_SO_ABI) : $(addprefix $(DIR_BUILD)/, $(LIB_OBJECTS)) | Makefile
	$(CC) $(CFLAGS_LOCAL) -shared -o $@ $(addprefix $(DIR_BUILD)/, $(LIB_OBJECTS))
	$(LN) -sf $(shell pwd)/$(DIR_BUILD)/$(LIB_SO_ABI) $(DIR_BUILD)/lib$(LIBNAME).so

$(DIR_BUILD)/%.o : %.c | Makefile
	$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -fPIC -c $< -o $@

#$(OBJECTS): $(DIR_BUILD)/%.o: %.c
	#$(MKDIR) -p $(@D)
	#$(CC) $(PPFLAGS) $(CFLAGS_LOCAL) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

$(DIR_BUILD) : 
	$(MKDIR) -p $(DIR_BUILD)

install : all
	$(INSTALL) -d "$(prefix)/lib"
	$(INSTALL) "$(DIR_BUILD)/$(LIB_SO_ABI)" "$(prefix)/lib"
	$(LN) -sf "$(prefix)/lib/$(LIB_SO_ABI)" "$(prefix)/lib/lib$(LIBNAME).so"
	$(INSTALL) -d "$(prefix)/bin"
	$(INSTALL) "$(DIR_BUILD)/$(APP)" "$(prefix)/bin"

uninstall : 
	$(RM) -f "$(prefix)/lib/$(LIB_SO_ABI)"
	$(RM) -f "$(prefix)/lib/lib$(LIBNAME).so"
	$(RM) -f "$(prefix)/bin/$(APP)"

test :
	echo 0 | $(APP) -f examples/196.bf
	echo 9 | $(APP) -f examples/196.bf
	echo 'hello world' | $(APP) -f examples/echo.bf
	$(APP) -f examples/ascii.bf
	$(APP) -f examples/hello.bf
	$(APP) -f examples/test.bf

clean : 
	$(RM) -rf $(DIR_BUILD)

include $(DEPFILES)
