# How to compile and run the program

# Parallel version
1) cd ~/ACA---MPI_Ricerca_Genoma-/MPI/ACA/Libraries/
2) mpicc -c *.c -lbsd
3) cd ..
4) mpicc -o main main.c Libraries/*.o -lbsd
5) mpirun main [file_with_genome.txt] pattern.txt

# Serial version
1) cd ~/ACA---MPI_Ricerca_Genoma-/Serial/ACA/Libraries/
2) gcc -c *.c -lbsd
3) cd ..
4) gcc -o serial serial.c Libraries/*.o -lbsd
5) ./serial [file_with_genome.txt] pattern.txt

# Developers
Michele di Frisco Ramirez and Marco Bernazzani are Computer Engineering students at University of Pavia.

This project was made for the Advanced Computer Architecture course.

Special thanks to Vito Giacalone (https://github.com/GigioMagno) for suggesting the structure of the project and the Utilities library.
