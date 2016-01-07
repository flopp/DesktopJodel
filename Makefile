BUILD_DIR       = build
MAKEFLAGS      += s

FILTERED_GOALS  = $(filter-out all debug release config veryclean $(BUILD_DIR), $(MAKECMDGOALS))
THIS_MAKEFILE   = $(lastword $(MAKEFILE_LIST))
BASE_DIR        = $(dir $(realpath $(THIS_MAKEFILE)))
BUILD_DIR      := $(BASE_DIR)$(BUILD_DIR)

.PHONY: all veryclean config 

all $(FILTERED_GOALS): $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR) $@
	
debug release: $(BUILD_DIR)
	@echo "-- Configuring build type $@"
	cmake $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$@
	
veryclean:
	@echo "-- Cleaning up"
	rm -rf $(BUILD_DIR) bin
	
config: $(BUILD_DIR)
	$(MAKE) edit_cache
	
$(BUILD_DIR):
	@echo "-- Creating build directory: $@"
	mkdir -p $@
	(cd $@; cmake $(BASE_DIR))
	
