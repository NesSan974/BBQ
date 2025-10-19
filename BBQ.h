#ifndef __BBQ_H__
#define __BBQ_H__

typedef struct 
{
    size_t size;        // Payload size
    uint8_t payload[];  // payload
}ItemBatchBufferQueue;

typedef struct {
    size_t capacity;            // taille totale allouée (en octets)
    size_t size;                // taille totale occupé (en octets)
    size_t front;               // index de lecture
    ItemBatchBufferQueue *data;   // zone mémoire contiguë
} BatchBufferQueue;

/// @brief Allocate the necessary memory
/// @param capacity 
/// @return
BatchBufferQueue* bbq_init(size_t capacity);

/// @brief Add / pushh / Enqueue by copying it, an `ItemBatchBufferQueue`
/// @param bq the `BatchBufferQueue` initilized by `bbq_init()`
/// @param it the item to add
void bbq_enqueue(BatchBufferQueue *bq, ItemBatchBufferQueue *it);

/// @brief Get a pointer to the first element
/// @param bq the `BatchBufferQueue` initilized by `bbq_init()`
/// @return pointer to the first item `ItemBatchBufferQueue`
ItemBatchBufferQueue* bbq_peek(BatchBufferQueue *bq);

/// @brief Copy maximum item until the max_size `limit`
/// @param bq the `BatchBufferQueue` initilized by `bbq_init()`
/// @param dst destination buffer where the copy goes
/// @param max_size the maximum size allowed to be copied into `dst`
/// @return 
size_t bbq_dequeue_into(BatchBufferQueue *bq, void *dst, size_t max_size);

/// @brief Basic print of "capacity", "size" and "front" of `bq`
/// @param bq the `BatchBufferQueue` initilized by `bbq_init()`
void bbq_print_info(BatchBufferQueue *bq);

#endif // __BUFFERARRAY_H__
