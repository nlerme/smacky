include Makefile.common

# Variables
DOC_DIR = doc
SRC_DIR = src

# Rules
all: header compile footer

header:
	@echo "---------------------------------------------------------------------------------------------"
	@echo "Project name : $(PROJECT_NAME)"
	@echo "Description  : $(DESCRIPTION)"
	@echo "License      : $(LICENSE)"
	@echo "Version      : $(VERSION)"
	@echo "Authors      : $(AUTHORS)"
	@echo "Last release : $(LAST_RELEASE)"
	@echo -e "---------------------------------------------------------------------------------------------\n"

compile:
	@$(MAKE) -C $(SRC_DIR) -s all

clean:
	@$(MAKE) -C $(SRC_DIR) -s clean
	@$(MAKE) -C $(DOC_DIR) -s clean

doc:
	@$(MAKE) -C $(DOC_DIR) -s all

footer:
	@echo -e "\n---------------------------------------------------------------------------------------------"

.PHONY: doc clean
