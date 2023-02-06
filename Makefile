include Makefile.common

#--- Variables --------------------------------------------------------------------------------------------
DOC_DIR = doc
SRC_DIR = src

#--- Rules ------------------------------------------------------------------------------------------------
all: header compile footer

header:
	@echo "---------------------------------------------------------------------------------------------"
	@echo "Project name : $(PROJECT_NAME)"
	@echo "Description  : $(PROJECT_DESCRIPTION)"
	@echo "License      : $(PROJECT_LICENSE)"
	@echo "Version      : $(PROJECT_VERSION)"
	@echo "Authors      : $(PROJECT_AUTHORS)"
	@echo "Last release : $(PROJECT_LAST_RELEASE)"
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
