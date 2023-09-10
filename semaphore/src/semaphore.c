/********************************************************************************
 * @brief Implementation details for binary and counting semaphores in C.
 ********************************************************************************/
#include <semaphore.h>

/********************************************************************************
 * @brief Structure for implementing counting semaphores in C. The structure
 *        is private in this file so that the used cannot alter the reserved
 *        resource count manually.
 * 
 * @param num_reserved_resources
 *        The number of reserved resources.
 * @param num_total_resources
 *        The total number of resources of the counting semaphore.
 ********************************************************************************/
struct counting_semaphore {
    uint16_t num_reserved_resources;
    uint16_t num_total_resources;
};

/********************************************************************************
 * @brief 32 binary semaphores, ID = [0, 31].
 ********************************************************************************/
static uint32_t binary_semaphores = 0; 

/********************************************************************************
 * @note 1. If an invalid ID is specified (sem_id > 31), we return false.
 *       2. We block the calling thread until the semaphore is available, i.e. 
 *          the corresponding bit is cleared.
 *       3. When the semaphore is available, the calling thread reserves it
 *          by setting the corresponding bit.
 *       4. We return true to indicate that the reservation succeeded.
 ********************************************************************************/
bool binary_semaphore_take(const uint8_t sem_id) {
    if (sem_id > BINARY_SEMAPHORE_ID_MAX) return false; 
    while (binary_semaphores & (1ULL << sem_id));       
    binary_semaphores |= (1ULL << sem_id);              
    return true;
}

/********************************************************************************
 * @note 1. If an invalid ID is specified (sem_id > 31), we return false.
 *       2. The calling thread releases the semaphorer by clearing the 
 *          corresponding bit.
 *       3. We return true to indicate that the release succeeded.
 ********************************************************************************/
bool binary_semaphore_release(const uint8_t sem_id) {
    if (sem_id > BINARY_SEMAPHORE_ID_MAX) return false;
    binary_semaphores &= ~(1ULL << sem_id);
    return true;
}


/********************************************************************************
 * @note 1. If an invalid total number of semaphores was specified 
 *          (num_resources = 0), we return a nullptr.
 *       2. Allocates memory for the semaphore on the heap.
 *       3. If the memory allocation failed, we return a nullptr.
 *       4. We initialize the semaphore, i.e. we set the starting values.
 *       5. We return a reference to the counting semaphore.
 ********************************************************************************/
struct counting_semaphore* counting_semaphore_new(const uint16_t num_resources) {
    if (num_resources == 0) return 0;
    struct counting_semaphore* self = (struct counting_semaphore*)malloc(sizeof(struct counting_semaphore));
    if (!self) return 0;
    self->num_reserved_resources = 0;
    self->num_total_resources = num_resources;
    return self;
}

/********************************************************************************
 * @note 1. Deallocates the heap allocated memory.
 *       2. Sets the semaphore pointer to null. The double-pointer makes it
 *          possible to set the "real" pointer to null, else a copy of the
 *          pointer would be passed and set to null and the original would still
 *          point at the adress where the semaphore was allocated previously.
 ********************************************************************************/
void counting_semaphore_delete(struct counting_semaphore** self) {
    free(*self);
    *self = 0;
}

/********************************************************************************
 * @note 1. We return the value of the reserved resources counter.
 ********************************************************************************/
uint16_t counting_semaphore_num_reserved(const struct counting_semaphore* self) {
    return self->num_reserved_resources;
}

/********************************************************************************
 * @note 1. We return the number of available resources by subtracting the total
 *          number of resources with the number of reserved resources.
 ********************************************************************************/
uint16_t counting_semaphore_num_available(const struct counting_semaphore* self) {
    return self->num_total_resources - self->num_reserved_resources;
}

/********************************************************************************
 * @note 1. The calling thread is blocked until at least one resource is
 *          available.
 *       2. The calling thread reserves a resource, we increment the reserved
 *          resources counter.
 ********************************************************************************/
void counting_semaphore_take(struct counting_semaphore* self) {
    while (self->num_reserved_resources >= self->num_total_resources);
    self->num_reserved_resources++;
}

/********************************************************************************
 * @note 1. If at least one resource is reserved, we decrement the reserved
 *          resources counter.
 ********************************************************************************/
void counting_semaphore_release(struct counting_semaphore* self) {
    if (self->num_reserved_resources > 0) {
        self->num_reserved_resources--;
    }
}