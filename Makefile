SOURCE_FILES := $(shell find . -name "*.c")

.PHONY: all clean

all: build/lambda build/debug

build/lambda: $(SOURCE_FILES)
	cc -std=c23 -Wall -Wextra -Wpedantic $^ -o $@

build/debug: $(SOURCE_FILES)
	cc -std=c23 -Wall -Wextra -Wpedantic -g $^ -o $@

clean:
	rm -rf build/*
