LD = $(CXX)

GENERIC_CFLAGS = -Wall -Wextra -pedantic -Werror -O3 -march=native
CFLAGS += -std=c99 $(GENERIC_CFLAGS)
CXXFLAGS += $(GENERIC_CFLAGS)

TESTAPP = test_ujpeg
OBJS = test_ujpeg.o ujpeg.o

all: $(TESTAPP)

ujpeg.c: ujpeg.h
test_ujpeg.cpp: ujpeg.h

$(TESTAPP): $(OBJS)
	$(LD) -o $@ $^

clean:
	rm -f $(OBJS) $(TESTAPP)

.PHONY: all clean
