export EXPERIMENT_ROOT = $(CURDIR)

.PHONY: all
all: progs

#prog_dirs := $(shell find ./progs -type d -links 2) # leaf directories under ./progs/

prog_dirs := $(dir $(shell find ./progs -name 'Makefile'))

.PHONY: progs
progs: $(prog_dirs)

.PHONY: $(prog_dirs)
$(prog_dirs):
	$(MAKE) -C $@
