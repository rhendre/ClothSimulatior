INCLUDES = -I lib

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 ${INCLUDES}

OBJS =		 Sphere.o Shader.o Triangle.o Particle.o Constraint.o Cloth.o Main.o

LIBS =     -lm  -lopengl32 -lglew32 -lfreeglut  -lglu32

TARGET =	 Main

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
