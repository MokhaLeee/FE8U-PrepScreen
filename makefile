.SUFFIXES:
.PHONY: hack wizardry clean


# Tools
TOOLS_DIR := c:/FE-Tools
include $(TOOLS_DIR)/tools.mk

# EA Input File
EVENT_MAIN := $(realpath .)/Main.event

# ROMs
ROM_TARGET := $(realpath .)/Prep.gba


include make_wizardry.mk



CLEAN_FILES :=
CLEAN_DIRS  :=


# ===============
# = MAIN TARGET =
# ===============

hack: $(ROM_TARGET)


# =================
# = THE BUILDFILE =
# =================

# EA depends
EVENT_DEPENDS := $(shell $(EADEP) $(EVENT_MAIN) -I $(EA_DIR) --add-missings)


$(ROM_TARGET): $(EVENT_MAIN) $(EVENT_DEPENDS) $(ROM_SOURCE)
	$(NOTIFY_PROCESS)
	@cp -f $(ROM_SOURCE) $(ROM_TARGET)
	@$(EA) A FE8 -output:$(ROM_TARGET) -input:$(EVENT_MAIN) 
	@rm -f $(OBJ_TARGET)


# ==============
# = MAKE CLEAN =
# ==============


CLEAN_FILES += $(ROM_TARGET)

# wizardry
CLEAN_FILES += $(ASM_C_GENERATED)


clean:
	@rm -f $(CLEAN_FILES)
	@rm -rf $(CLEAN_DIRS)

	@echo all clean!

.PHONY: clean
