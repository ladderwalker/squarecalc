all : squarecalc squarecalc_ogl

squarecalc : main.c
	gcc -o $@ $^ -I. -lX11 -lpthread -lXinerama -lXext -lGL -g -lm -ldl

squarecalc_ogl : main.c
	gcc -o $@ $^ -I. -lX11 -lpthread -lXinerama -lXext -lGL -g -DCNFGOGL -lm -ldl

clean :
	rm -rf *.o *~ squarecalc squarecalc_ogl 
