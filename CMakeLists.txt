cmake_minimum_required(VERSION 4.0)
project(ACA___MPI_Ricerca_Genoma_)

set(CMAKE_CXX_STANDARD 14)

include_directories(MPI/ACA/Libraries)

add_executable(ACA___MPI_Ricerca_Genoma_
        MPI/ACA/Libraries/hash_funct.c
        MPI/ACA/Libraries/hash_funct.h
        Serial/ACA/readme
        README.md)
