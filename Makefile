CXX = clang++
IMGUI_DIR = ./imgui
CXXFLAGS = -Wall -Iinclude -I/opt/homebrew/include -std=c++23 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL
SRC = src/main.cpp
SRC += src/glad.c
SRC += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SRC += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OUT = ikeloa

all: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) -o $(OUT) $(LDFLAGS)

run: all
	./$(OUT)
