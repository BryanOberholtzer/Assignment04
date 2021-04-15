cmake_minimum_required(VERSION 3.17)
project(Assignment04 C)

set(CMAKE_C_STANDARD 99)

add_executable(Assignment04 main.c main.h sigusr_handler.c reporter.c sigusr_generator.c)