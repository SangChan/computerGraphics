//LIBS = -lGL -lGLU -lglut
ifeq ($(shell uname),Darwin)
	LIBS = -framework Carbon -framework OpenGL -framework GLUT
else
	LIBS = -lGL -lGLU -lglut
endif

${PROG} : ${PROG}.c
	g++ -o $@ $< glm.c ${LIBS}

clean:
	rm -rf *.o ${PROG}
