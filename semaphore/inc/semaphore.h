/********************************************************************************
 * @brief Contains binary and counting semaphores for usage in C and C++.
 *        The binary semaphore interface is shared between C and C++, while
 *        separate interfaces are implemented for counting semaphores.
 ********************************************************************************/
#pragma once

/********************************************************************************
 * @brief The code within the extern "C" directive is compiled as C code if
 *        if a C++ compiler is used. This code is compatible with C and C++.
 ********************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/********************************************************************************
 * @brief Parameters for binary semaphores.
 * 
 * @param BINARY_SEMAPHORE_LIMIT
 *        The number of available binary semaphores (32).
 * @param BINARY_SEMAPHORE_ID_MIN
 *        The lowest permitted semaphore ID (0).
 * @param BINARY_SEMAPHORE_ID_MAX
 *        The highest permitted semaphore ID (31).
 ********************************************************************************/
#define BINARY_SEMAPHORE_LIMIT  (uint8_t)(32)
#define BINARY_SEMAPHORE_ID_MIN (uint8_t)(0)
#define BINARY_SEMAPHORE_ID_MAX (uint8_t)(BINARY_SEMAPHORE_LIMIT - 1)

/********************************************************************************
 * @brief Reseves semaphore with specified ID. If the semaphore is reserved,
 *        the calling thread is blocked until the semaphore is available.
 * 
 * @param sem_id
 *        Identifier of the semaphore to reserve (0 - 31).
 * @return 
 *        True upon successful reservation, false if an invalid semaphore
 *        identifier was specified (sem_id >= 32).
 ********************************************************************************/
bool binary_semaphore_take(const uint8_t sem_id);

/********************************************************************************
 * @brief Releases semaphore with specified ID. 
 * 
 * @param sem_id
 *        Identifier of the semaphore to release (0 - 31).
 * @return 
 *        True upon successful release, false if an invalid semaphore
 *        identifier was specified (sem_id >= 32).
 ********************************************************************************/
bool binary_semaphore_release(const uint8_t sem_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

 /********************************************************************************
 * @note The following code is only available in C.
 ********************************************************************************/
#ifndef __cplusplus 

#include <stdlib.h>

/********************************************************************************
 * @brief Predeclaration of counting semaphore. This structure is hidden in
 *        the corresponding source file to make the semaphore counter private.
 ********************************************************************************/
struct counting_semaphore;

/********************************************************************************
 * @brief Creates a new dynamically allocated counting semaphore.
 * 
 * @param num_resources
 *        The number of resources available for the counting semaphore.
 * @return 
 *        A reference to the counting semaphore, nullptr if the memory allocation
 *        failed or if an invalid number of resources was specified 
 *        (num_resources = 0).
 ********************************************************************************/
struct counting_semaphore* counting_semaphore_new(const uint16_t num_resources);

/********************************************************************************
 * @brief Deletes dynamically allocated counting semaphore and sets the
 *        corresponding pointer to null.
 * 
 * @param self
 *        The address of the counting semaphore pointer.
 ********************************************************************************/
void counting_semaphore_delete(struct counting_semaphore** self);

/********************************************************************************
 * @brief Provides the number of reserved resources of counting semaphore.
 * 
 * @param self
 *        Reference to the counting semaphore.
 * @return
 *        The number of reserved resources.
 ********************************************************************************/
uint16_t counting_semaphore_num_reserved(const struct counting_semaphore* self);

/********************************************************************************
 * @brief Provides the number of available resources of counting semaphore.
 * 
 * @param self
 *        Reference to the counting semaphore.
 * @return
 *        The number of available resources.
 ********************************************************************************/
uint16_t counting_semaphore_num_available(const struct counting_semaphore* self);

/********************************************************************************
 * @brief Reserves a resource of referenced counting semaphore. The calling 
 *        thread will be temporarily blocked if all resources are reserved. 
 * 
 * @param self
 *        Reference to the counting semaphore.
 ********************************************************************************/
void counting_semaphore_take(struct counting_semaphore* self);

/********************************************************************************
 * @brief Releases a resource of referenced counting semaphore.
 * 
 * @param self
 *        Reference to the counting semaphore.
 ********************************************************************************/
void counting_semaphore_release(struct counting_semaphore* self);

/********************************************************************************
 * @note The following code is only available in C++.
 ********************************************************************************/
#else 

/********************************************************************************
 * @brief Class for implementing counting semaphores in C++.
 ********************************************************************************/
template <uint16_t num_resources>
class counting_semaphore {
    static_assert(num_resources > 0, "The number of resources for a counting semaphore cannot be 0!");
  public:

    /********************************************************************************
     * @brief Creates new counting semaphore.
     ********************************************************************************/
    counting_semaphore(void) = default;

    /********************************************************************************
     * @brief Provides the number of reserved resources of counting semaphore.
     * 
     * @return
     *        The number of reserved resources.
     ********************************************************************************/
    uint16_t num_reserved_resources(void) const { return num_reserved_resources_; }

    /********************************************************************************
     * @brief Provides the number of available resources of counting semaphore.
     * 
     * @return
     *        The number of available resources.
     ********************************************************************************/
    uint16_t num_available_resources(void) const { return num_resources - num_reserved_resources_; }

    /********************************************************************************
     * @brief Reserves a resource of referenced counting semaphore. The calling 
     *        thread will be temporarily blocked if all resources are reserved. 
     ********************************************************************************/
    void take(void) {
        while (num_reserved_resources_ >= num_resources);
        num_reserved_resources_++;
    }

    /********************************************************************************
     * @brief Releases a resource of referenced counting semaphore. 
     ********************************************************************************/
    void release(void) {
        if (num_reserved_resources_ > 0) {
            num_reserved_resources_--;
        }
    }

  private:
    uint16_t num_reserved_resources_{}; /* Counts the number of reserved resources. */
};

#endif /* ifndef __cplusplus */