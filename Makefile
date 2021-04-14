CC = gcc
YACC = yacc
LEX = flex
CFLAGS = -g
LFLAGS = -l
YFLAGS = -dv
OBJECTS =
LDFLAGS = #"-L/usr/local/opt/flex/lib"
LDLIBS = -lfl

.PHONY: all

debug:
	gdb dragon

run: all
	./dragon pascal/t1-1a.p

all: dragon

dragon: $(OBJECTS) y.tab.o lex.yy.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

y.tab.c: pc.y
	$(YACC) $(YFLAGS) $^

lex.yy.c: pc.l
	$(LEX) $(LFLAGS) $^

.PHONY: clean
clean:
	rm dragon lex.yy.c y.* *.o

