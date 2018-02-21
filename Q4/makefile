CC = gcc
CFLAGS = -g -pthread -Wall

PROG = printmanager
SOURCES = printmanager.c printqueue.c

OBJDIR = object
OBJECTS = $(OBJDIR)/printmanager.o $(OBJDIR)/printqueue.o

$(PROG): $(OBJECTS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/printmanager.o: printmanager.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/printqueue.o: printqueue.c $(HEADERS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(PROG) $(OBJECTS)
