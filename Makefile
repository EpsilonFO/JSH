# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -g

# Noms des fichiers source
SRCS = jsh.c prompt.c functions.c redirect.c jobs.c pipe.c substitutions.c commandes.c exit.c kill.c fg.c bg.c cd.c extern_managment.c main.c

# Noms des fichiers objets générés à partir des sources
OBJS = $(SRCS:.c=.o)

# Nom de l'exécutable
TARGET = jsh

READLINE = -lreadline
# Règle par défaut
all: $(TARGET)

# Règle de compilation de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(READLINE)

# Règle de compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
