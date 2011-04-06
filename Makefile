CC = g++

final_triangle : final_triangle.o
	$(CC) $(CFLAGS) -o $@ $< 

run : final_triangle
	./final_triangle
