
# ==========================================
# CONFIGURACIO DEL PROJECTE
# ==========================================

CXX := g++-11

TARGET := penjat.e

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include

# ==========================================
# OPCIONS DE COMPILACIO
# ==========================================

CPPFLAGS := -I$(INC_DIR)
CXXFLAGS := -Wall -MMD -MP

# ==========================================
# OPCIONS D'ENLLACAT
# ==========================================

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
	$(CXX) -o $@ $^ $(LDLIBS)

# Compilacio individual dels fitxers .cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

endif

# Creacio del directori d'objectes
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Incorporacio de dependències automatiques
-include $(DEPS)

# ==========================================
# NETEJA
# ==========================================

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET)
