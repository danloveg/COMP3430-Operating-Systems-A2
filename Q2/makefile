CC = gcc
CFLAGS = -g -pthread -Wall

PROG = question2
SOURCES = question2.c

OBJDIR = object
OBJECTS = $(OBJDIR)/question2.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/question2.o: question2.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)
	rm q2output.txt

