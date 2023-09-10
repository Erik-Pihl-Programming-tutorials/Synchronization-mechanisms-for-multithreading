/********************************************************************************
 * @brief Demonstration of counting semaphores in C. 
 ********************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

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
 * @brief Counting semaphores for synchronized usage of the terminal and
 *        shared memory. The max count of the semaphores are set to 1, which
 *        makes them function as binary semaphores in this case, but we can
 *        set the max count to everyting between [1, 65 535].
 ********************************************************************************/
static struct counting_semaphore *sem_console, *sem_shared_mem;

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
        counting_semaphore_take(sem_shared_mem);
        const uint16_t num_prints_copy = ++num_prints;
        counting_semaphore_release(sem_shared_mem);

        counting_semaphore_take(sem_console);
        printf("--------------------------------------------------------------------------------\n");
        printf("Running thread with ID %hu!\n", self->id);
        printf("Number of performed prints: %hu\n", num_prints_copy);
        printf("--------------------------------------------------------------------------------\n\n");
        delay_ms(10);
        counting_semaphore_release(sem_console);
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
    sem_console = counting_semaphore_new(1);
    sem_shared_mem = counting_semaphore_new(1);

    pthread_create(&t1, 0, run_thread, &args1);
    pthread_create(&t2, 0, run_thread, &args2);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
    counting_semaphore_delete(&sem_console);
    counting_semaphore_delete(&sem_shared_mem);
    return 0;
}