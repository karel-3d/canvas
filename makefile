program=canvas
OBJECTS=canvas.o curves/circle.o main.o moved_arrays.o png_paint.o point_o RGBa.o shape.o
HEADS=all_shapes.h canvas.h curve.h curves/circle.h interval.h matrix.h moved_arrays.h plane.h png_paint.h point.h RGBa.h shape.h types.h


CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I./headers 
LDFLAGS=-lpng


REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: ${program}

${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	g++ -o $@ ${REAL_OBJECTS} $(LDFLAGS)

clean:
	rm -f objects/*.o ${program}
