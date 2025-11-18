CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lreadline
SRCDIR = src
OBJDIR = obj
SOURCES = $(SRCDIR)/Main.c #$(SRCDIR)/navegacion.c $(SRCDIR)/Comando_personalizado.c $(SRCDIR)/Comando_pipes.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
TARGET = shell

# Crear carpeta obj si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Regla principal: compilar todos los objetos y luego enlazar
compile: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilar cada .c a .o, asegurando que obj exista
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla: correr el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: compile run clean