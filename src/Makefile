# Variables
CXX       = g++
CXXFLAGS  = -O3 -Wall -g
LDFLAGS   = -L/usr/lib -L./lib
INC_DIR   = -I../include -I/usr/include/libxml2
LIBS      = -lm -lxml2
OBJ_DIR   = ../.obj
FILES_EXT = cpp
SRC       = $(wildcard *.$(FILES_EXT))
OBJ       = $(SRC:%.$(FILES_EXT)=$(OBJ_DIR)/%.o)
TARGET    = main

# Rules
all: depend $(OBJ) $(TARGET)

depend:
	@echo -n "+ Generating dependencies : "
ifneq ($(strip $(SRC)),)
	@$(CXX) $(INC_DIR) -MM $(SRC:%=%) \
	| sed -e 's:^[^ ]:$(OBJ_DIR)/&:' > Makefile.depend
endif
	@echo "Ok"

$(OBJ_DIR)/%.o: %.$(FILES_EXT)
	@echo -n "+ Compiling $(notdir $@) : "
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INC_DIR) -c $< -o $@
	@echo "Ok"

$(TARGET): $(OBJ)
	@echo -n "+ Compiling $@ : "
	@$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@ $(LDFLAGS) $(LIBS)
	@\mv $(TARGET) ..
	@echo "Ok"

clean:
	@echo -n "+ Deleting binaries : "
	@\rm -f $(OBJ_DIR)/*.o ../$(TARGET)
	@echo "Ok"

include Makefile.depend

.PHONY: clean
