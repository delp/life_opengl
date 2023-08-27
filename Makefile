default:
	gcc pixels.c -lglut -lGLU -lGL -o conway

run:
	gcc pixels.c -lglut -lGLU -lGL -o conway
	./conway
