# Makefile
# Artur Miguel Dias, 2022
#
# LAP - Projeto


##############################################################################
# Base configuration
##############################################################################

APP = EyeBeast
VERSION = 0.00.1
CC = gcc
CPP = g++
CFLAGS = -O1 -w -Wno-invalid-source-encoding
ONLYCFLAGS = # -Wmissing-prototypes
DEFINES =
LIBS= # -lm
OBJ_DIR = obj
DEBUG =
NAMES = wxTiny $(APP)
OS = $(shell uname | cut -c 1-7)
EXTRA = -std=c11


##############################################################################
# Computed configuration
##############################################################################

define OBJS
	$(addprefix $(OBJ_DIR)/,											\
		$(addsuffix .o, $(foreach file, $(NAMES), $(file)))				\
	)
endef

ZFLAGS = $(DEBUG)
ZDEFINES = $(EXTRA) -DOS=$(OS) $(DEFINES)
ZLIBS = $(EXTRA) $(LIBS)
ZPRE =

ifeq ($(EVAL),1)
	CFLAGS += -DEVAL=1
endif
 
ifeq ($(OS),MINGW32)
ZDEFINES += -DUSE_WXWIDGETS												\
			-mthreads -pipe -fmessage-length=0							\
			-DHAVE_W32API_H -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE	\
			-IC:/SourceCode/Libraries/wxWidgets2.8/lib/gcc_dll/mswu		\
			-IC:/SourceCode/Libraries/wxWidgets2.8/include
ZLIBS += -enable-auto-import -export-all-symbols -mthreads				\
			-LC:/SourceCode/Libraries/wxWidgets2.8/lib/gcc_dll -lwxmsw28u

else # Assume Unix/MacOS
ZDEFINES += -DUSE_WXWIDGETS $(shell wx-config --unicode --cxxflags)
ZLIBS += $(shell wx-config --unicode --libs)
ZPRE +=																	\
 if ! type wx-config >/dev/null 2>&1; then								\
  echo "\n";															\
  echo "*************************************************************"; \
  echo "Missing WXWIDGETS dependency:";									\
  echo "  The developement library \"libwxgtk2.8-dev\" is missing!!!";  \
  echo "  For WXWIDGETS support, you need to install it yourself.";		\
  echo "  In Debian-based systems it is enough to issue the command:";  \
  echo "		sudo apt-get install libwxgtk2.8-dev";					\
  echo "*************************************************************"; \
  echo "\n";															\
  exit 1;																\
 fi;
endif


##############################################################################
# Build as an application
##############################################################################

$(APP): $(OBJ_DIR) $(OBJS)
	$(CPP) $(CFLAGS) -o $(APP) $(OBJS) $(ZLIBS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(ZFLAGS) $(ZDEFINES) $(ONLYCFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CPP) $(CFLAGS) $(ZFLAGS) $(ZDEFINES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

EyeBeast0: $(OBJ_DIR) $(OBJS) $(OBJ_DIR)/wxTiny0.o
	$(CPP) $(CFLAGS) -o EyeBeast0 $(OBJ_DIR)/EyeBeast.o $(OBJ_DIR)/wxTiny0.o $(ZLIBS)


##############################################################################
# Utilities
##############################################################################

.PHONY : clean
clean:
	rm -rf $(APP) $(OBJ_DIR)
#	@find submissions/* -executable -delete > /dev/null 2>&1
#	@find submissions -name "*.txt" -exec rm {} \; > /dev/null 2>&1

run: $(APP)
	./$(APP)

.PHONY : compall
compall:
	rm -f EyeBeast.c EyeBeast ; \
	for f in *.c ; do \
		echo "+++++ Compilando $$f" ; \
		rm -f EyeBeast.c ; \
		cp $$f EyeBeast.c ; \
		rm -f EyeBeast ; \
		$(MAKE) -s EyeBeast ; \
		mv EyeBeast `basename $$f .c` ; \
		sleep 3 ; \
	done ;

.PHONY : eval
eval:
	@if type $(APP).cpp >/dev/null 2>&1; \
	then \
		echo "Ignored! Respecting the file \"$(APP).cpp\"" ; \
	else \
		for f in submissions/*.cpp ; do \
			echo "$$f" ; \
			rm -f $(APP) $(APP).cpp obj/$(APP).o ; \
			ln -s -T "$$f" $(APP).cpp ; \
			EVAL=1 $(MAKE) -s $(APP) > "$${f%.*}.txt" 2>&1 ; \
			mv $(APP) "$${f%.*}" ; \
		done ; \
	fi

all: $(APP)


##############################################################################
# THE END
##############################################################################