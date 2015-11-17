CXX = g++
LD = g++

CXXFLAGS = -g -pedantic -Wall -Werror -std=c++11
LDFLAGS = -lGL -lSDL2 -lGLU -lGLEW

SRC_NAME = \
	Main.cpp \
	CGraphic.cpp \
	CVertexAttribArray.cpp \
	CInput.cpp \
	CShaderProgram.cpp \
	CTexture.cpp \
	CIndexBuffer.cpp
TARGET_NAME = opengl

SRC_PATH = ./src/
TARGET_PATH = ./bin/

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
TARGET = $(TARGET_PATH)$(TARGET_NAME)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

run:
	(cd $(TARGET_PATH) && ./$(TARGET_NAME))

clean:
	rm -f $(TARGET)
