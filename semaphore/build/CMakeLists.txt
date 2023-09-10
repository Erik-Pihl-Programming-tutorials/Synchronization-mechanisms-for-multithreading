cmake_minimum_required(VERSION 3.20)
project(semaphore_examples)
include_directories(../inc)

add_executable(run_binary_sem_c ../src/main_binary_sem.c ../src/semaphore.c)
target_compile_options(run_binary_sem_c PRIVATE -Wall -Werror)
target_link_libraries(run_binary_sem_c pthread)
set_target_properties(run_binary_sem_c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

add_executable(run_counting_sem_c ../src/main_counting_sem.c ../src/semaphore.c)
target_compile_options(run_counting_sem_c PRIVATE -Wall -Werror)
target_link_libraries(run_counting_sem_c pthread)
set_target_properties(run_counting_sem_c PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)

add_executable(run_counting_sem_cpp ../src/main_counting_sem.cpp ../src/semaphore.c)
target_compile_options(run_counting_sem_cpp PRIVATE -Wall -Werror)
target_link_libraries(run_counting_sem_cpp pthread)
set_target_properties(run_counting_sem_cpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)