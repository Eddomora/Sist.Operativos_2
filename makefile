CC = gcc
CFLAGS = -Wall -Wextra -g

SIM_DIR = simulador
BARR_DIR = barrera

SIM_SRC = $(SIM_DIR)/simulador_memoria_virtual.c
BARR_SRC = $(BARR_DIR)/barrera_reutilizable.c

SIM_BIN = $(SIM_DIR)/sim
BARR_BIN = $(BARR_DIR)/barrera

.PHONY: all compile build-sim build-barrera run-sim run-barrera test clean

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

test: build-sim
	@echo "===== PRUEBAS DEL SIMULADOR ====="
	@echo ""
	@echo "--- trace1.txt (tamaño de marco: 8) ---"
	@echo "Prueba 1: 8 marcos"
	@cd $(SIM_DIR) && ./sim 8 8 trace1.txt
	@echo ""
	@echo "Prueba 2: 16 marcos"
	@cd $(SIM_DIR) && ./sim 16 8 trace1.txt
	@echo ""
	@echo "Prueba 3: 32 marcos"
	@cd $(SIM_DIR) && ./sim 32 8 trace1.txt
	@echo ""
	@echo "--- trace2.txt (tamaño de marco: 4096) ---"
	@echo "Prueba 4: 8 marcos"
	@cd $(SIM_DIR) && ./sim 8 4096 trace2.txt
	@echo ""
	@echo "Prueba 5: 16 marcos"
	@cd $(SIM_DIR) && ./sim 16 4096 trace2.txt
	@echo ""
	@echo "Prueba 6: 32 marcos"
	@cd $(SIM_DIR) && ./sim 32 4096 trace2.txt
	@echo ""
	@echo "===== PRUEBAS COMPLETADAS ====="

clean:
	@echo "Limpiando binarios..."
	-rm -f $(SIM_BIN) $(BARR_BIN)