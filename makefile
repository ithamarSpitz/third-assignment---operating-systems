# Find all subdirectories
SUBDIRS := $(wildcard */)

# Default target
all: $(SUBDIRS)

# Rule for each subdirectory
$(SUBDIRS):
	$(MAKE) -C $@

# Clean target
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

# Phony targets
.PHONY: all $(SUBDIRS) clean

tests:
	cd test && \
	chmod +x test_server.sh && \
	./test_server.sh && \
	cd ..