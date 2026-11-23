# ==========================================
# CONFIGURACIO DEL PROJECTE
# ==========================================

# Al STIC, canvieu-ho per g++ (o g++-13) si cal.
CXX := g++-11

TARGET := penjat.e

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
ESIN_INC_DIR := incl
LIB_DIR := lib

# ==========================================
# OPCIONS DE COMPILACIO
# ==========================================

CPPFLAGS := -I$(INC_DIR) -I$(ESIN_INC_DIR)
CXXFLAGS := -Wall -MMD -MP

# ==========================================
# OPCIONS D'ENLLACAT
# ==========================================

# -rpath: perque l'executable trobi libesin.so en temps d'execucio
LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,'$$ORIGIN/$(LIB_DIR)'
LDLIBS := -lesin

# ==========================================
# DETECCIO AUTOMATICA DELS FITXERS
# ==========================================

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

DEPS := $(OBJECTS:.o=.d)

# ==========================================
# REGLES DE COMPILACIO
# ==========================================

.PHONY: all clean

ifeq ($(strip $(SOURCES)),)

all:
	@echo "No hi ha fitxers .cpp dins de src/."

else

all: $(TARGET)

# Enllacat de tots els fitxers objecte
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilacio individual dels fitxers .cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

endif

# Creacio del directori d'objectes
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Incorporacio de dependencies automatiques
-include $(DEPS)

# ==========================================
# NETEJA
# ==========================================

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET)