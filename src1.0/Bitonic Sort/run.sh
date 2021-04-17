export MPI_NUM_THREADS=8

./MPIBitonic

mpirun -np -2 ./OMPBitonic

./SerialBitonic
