/********************************************************************************
 * @brief Demonstration of binary semaphores in C.
 ********************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/********************************************************************************
 * @brief Identifiers for binary semaphores used in the program. These
 *        semaphores are used to make sure that only one thread has access to
 *        any given shared resource at a given time.
 * 
 * @param BINARY_SEM_ID_CONSOLE
 *        Semaphore for reserving the terminal (ID = 0).
 * @param BINARY_SEM_ID_SHARED_MEM
 *        Semaphore for reserving shared variables (ID = 1)
 ********************************************************************************/
#define BINARY_SEM_ID_CONSOLE    (uint8_t)(0)
#define BINARY_SEM_ID_SHARED_MEM (uint8_t)(1)

/********************************************************************************
 * @brief Structure containing thread arguments.
 * 
 * @param id
 *        Unique identifier of the thread.
 * @param print_interval_ms
 *        The time interval between each print, measured in milliseconds.
 ********************************************************************************/
struct thread_args {
    const uint16_t id;
    const uint16_t print_interval_ms;
};

/********************************************************************************
 * @brief Stores the number of performed prints.
 ********************************************************************************/
static uint16_t num_prints = 0;

/********************************************************************************
 * @brief Blocks the calling thread for specified delay time in milliseconds.
 * 
 * @param delay_time_ms
 *        The time to block the calling thread for, measured in milliseconds.
 ********************************************************************************/
static inline void delay_ms(const uint16_t delay_time_ms) {
    usleep(delay_time_ms * 1000);
}

/********************************************************************************
 * @brief Runs the calling thread continuously by printing the thread ID with
 *        specified frequency. Semaphores are used to synchronize the usage
 *        of the shared resources.
 * 
 * @param args
 *        Thread-specific arguments passed as a reference to a thread_args
 *        structure. 
 * @return 
 *        A nullptr, since thread functions in C need to return some kind of
 *        pointer.
 ********************************************************************************/
static void* run_thread(void* args) {
    struct thread_args* self = (struct thread_args*)(args);
    const uint16_t adjusted_print_interval_ms = self->print_interval_ms > 10 ? self->print_interval_ms - 10 : 10;
    while (1) {
        binary_semaphore_take(BINARY_SEM_ID_SHARED_MEM);
        const uint16_t num_prints_copy = ++num_prints;
        binary_semaphore_release(BINARY_SEM_ID_SHARED_MEM);

        binary_semaphore_take(BINARY_SEM_ID_CONSOLE);
        printf("--------------------------------------------------------------------------------\n");
        printf("Running thread with ID %hu!\n", self->id);
        printf("Number of performed prints: %hu\n", num_prints_copy);
        printf("--------------------------------------------------------------------------------\n\n");
        delay_ms(10);
        binary_semaphore_release(BINARY_SEM_ID_CONSOLE);
        delay_ms(adjusted_print_interval_ms);
    }
    return 0;
}

/********************************************************************************
 * @brief Creates and runs two threads with different parameters.
 * 
 * @note
 *        When creating threads, specify the starting routine and it's input
 *        arguments. Synchronize the threads by calling the join function.
 ********************************************************************************/
int main(void) {
    struct thread_args args1 = {1, 1000}, args2 = {2, 1000};
    pthread_t t1, t2;
    pthread_create(&t1, 0, run_thread, &args1);
    pthread_create(&t2, 0, run_thread, &args2);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
    return 0;
}