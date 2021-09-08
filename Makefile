CC = gcc
YACC = yacc
LEX = flex
CFLAGS = -std=c11 -g
LFLAGS = -l
YFLAGS = -dv
TEST_OBJECTS = avl.o list.o hashmap.o tree.o
DRAGON_OBJECTS = lex.yy.o y.tab.o avl.o list.o tree.o hashmap.o table_stack.o
#LDFLAGS = "-L/usr/local/opt/flex/lib"
LDLIBS = -lfl

.PHONY: clean dragon

dragon: $(DRAGON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

run: dragon
	./dragon pascal/boo.p

debug: dragon
	gdb ./dragon

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

