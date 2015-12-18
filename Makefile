CXX = g++
LD = g++

CXXFLAGS = -g -pedantic -Wall -Werror -std=c++11
LDFLAGS = -lGL -lSDL2 -lGLU -lGLEW -lSDL2_image

SRC_NAME = \
	Main.cpp \
	CGraphic.cpp \
	CVertexAttribArray.cpp \
	CInput.cpp \
	CShaderProgram.cpp \
	CTexture.cpp \
	CIndexBuffer.cpp \
	CCamera.cpp \
	CTerrain.cpp \
	CTransform.cpp

TARGET_NAME = opengl

SRC_PATH = ./src/
TARGET_PATH = ./bin/

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(SRC:.cpp=.o)
TARGET = $(TARGET_PATH)$(TARGET_NAME)

$(TARGET): $(OBJ)
	$(LD) -o $@ $(OBJ) $(LDFLAGS)

run:
	(cd $(TARGET_PATH) && ./$(TARGET_NAME))

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
