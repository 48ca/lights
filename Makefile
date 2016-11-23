(all):
	mpicc -Ofast lights.c `pkg-config --cflags --libs gl glu libpng` -lm -lglut -o lights
