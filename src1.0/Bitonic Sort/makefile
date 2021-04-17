say_hello:
	@echo "Compiling Bitonic sort files..."
	gcc Serial_Bitonic.c -o SerialBitonic
	gcc -fopenmp OpenMP_Bitonic.c -o OMPBitonic
	mpicc MPI_Bitonic.c -o MPIBitonic

clean:
	@echo "cleaning up..."

