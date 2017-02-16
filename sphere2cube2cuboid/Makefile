Objs = main.o MyServer.o lz4.o buildMiniCuboid.o cube2cuboid.o \
	   cutIntoFour.o mergeCuboidPieces.o mUtils.o separateCubeMini.o RicohClient.o buffer.o

CC = gcc

override CFLAGS += -Wall -fopenmp $(OPTIMIZATION)
CXXFLAGS = -std=c++11
CXXFLAGS += $(CFLAGS)


all: ${Objs} sphere2cube.cu
	nvcc -arch compute_61 -ccbin ${CC} ${Objs} sphere2cube.cu `pkg-config opencv --cflags --libs`  \
    -Xcompiler -fopenmp -Iinc 

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.out
