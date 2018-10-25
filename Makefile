CFLAGS = -std=c99 -Wall -g -coverage

DIR_BUILD = ./build
DIR_SOURCE = ./source

PPFLAGS = -MT $@ -MMD -MP -MF $(DIR_BUILD)/$*.d

SOURCES = $(wildcard source/*.c)

OBJS = $(addprefix $(DIR_BUILD)/, $(patsubst %.c, %.o, $(notdir $(SOURCES))))

TARGET = test

DEPFILES = $(patsubst %.o, %.d, $(OBJS))

# core header
#vpath %.h ../../inc/core
#vpath %.h ../../inc/toolkit

.PHONY : all clean
all : $(DIR_BUILD)/$(TARGET)

$(DIR_BUILD)/$(TARGET) : $(OBJS) $(DIR_BUILD)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/%.o : $(DIR_SOURCE)/%.c $(DIR_BUILD)/%.d $(DIR_BUILD)
	$(CC) $(PPFLAGS) $(CFLAGS) -c $< -o $@

$(DIR_BUILD)/%.d : ;
.PRECIOUS : $(DIR_BUILD)/%.d

#create build directory
$(DIR_BUILD) : 
	mkdir -p $(DIR_BUILD)

#clean outputs
clean : 
	rm -rf $(DIR_BUILD)

-include $(DEPFILES)

