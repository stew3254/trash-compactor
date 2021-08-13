CC = gcc
YACC = yacc
LEX = flex
CFLAGS = -O3 -std=c11
LFLAGS = -l
YFLAGS = -dv
TEST_OBJECTS = hashmap.o tree.o
DRAGON_OBJECTS = lex.yy.o y.tab.o avl.o list.o tree.o hashmap.o
LDFLAGS ="-L/usr/local/opt/flex/lib"
LDLIBS = -lfl

.PHONY: clean dragon

dragon: $(DRAGON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

run: dragon
	./dragon

test: $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

y.tab.h y.tab.c: pc.y
	$(YACC) $(YFLAGS) pc.y

lex.yy.c: y.tab.h pc.l
	$(LEX) $(LFLAGS) pc.l

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm dragon test lex.yy.c y.* *.o

