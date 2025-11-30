CC = gcc
CFLAGS = -Wall -Wextra -g

SIM_DIR = simulador
BARR_DIR = barrera

SIM_SRC = $(SIM_DIR)/simulador_memoria_virtual.c
BARR_SRC = $(BARR_DIR)/barrera_reutilizable.c

SIM_BIN = $(SIM_DIR)/sim
BARR_BIN = $(BARR_DIR)/barrera

.PHONY: all compile build-sim build-barrera run-sim run-barrera clean

all: compile

compile: build-sim build-barrera

build-sim:
	@echo "Compilando simulador..."
	$(CC) $(CFLAGS) $(SIM_SRC) -o $(SIM_BIN)

build-barrera:
	@echo "Compilando barrera..."
	$(CC) $(CFLAGS) $(BARR_SRC) -o $(BARR_BIN)

run-sim: build-sim
	@cd $(SIM_DIR) && ./sim $(ARGS)

run-barrera: build-barrera
	@cd $(BARR_DIR) && ./barrera $(ARGS)

clean:
	@echo "Limpiando binarios..."
	-rm -f $(SIM_BIN) $(BARR_BIN)