
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "BBQ.h"

BatchBufferQueue *bbq_init(size_t capacity)
{
    BatchBufferQueue *bq = malloc(sizeof(BatchBufferQueue));

    bq->capacity = capacity;
    bq->front = 0;
    bq->size = 0;
    bq->data = malloc(capacity);
}

ItemBatchBufferQueue *bbq_peek(BatchBufferQueue *bq)
{
    if (bq->size <= 0)
    {
        // TODO : Error

        return NULL;
    }

    ItemBatchBufferQueue *it = (ItemBatchBufferQueue *)((uint8_t *)bq->data + bq->front);

    return it;
}

void bbq_enqueue(BatchBufferQueue *bq, ItemBatchBufferQueue *it)
{
    size_t total_size_it = sizeof(ItemBatchBufferQueue) + it->size;

    size_t rear = (bq->front + bq->size) % bq->capacity;
    void *rear_ptr = (uint8_t *)bq->data + rear;

    // CAS 1 :
    // [ xxxxxxxxx------ ] --> On veux rajouter 4 'x'
    //   ^front   ^rear

    // CAS 2 :
    // [ --------xxxxxxx-- ] --> On veux rajouter 4 'x'
    //     front ^      ^ rear

    // CAS 3 :
    // [ xxxx----xxxxxxx ] --> On veux rajouter 4 'x'
    //   rear^   ^front

    if (bq->front > rear)
    {
      printf("CAS 3\n");

      if (bq->size + total_size_it > bq->capacity)
      {
           printf("Agrandissement\n");

           size_t old_front_until_overflow = bq->capacity - bq->front;

           size_t new_cap = bq->capacity + total_size_it;
           bq->data = realloc(bq->data, new_cap);
           bq->capacity = new_cap;

           memmove((uint8_t *)bq->data + bq->front + total_size_it, (uint8_t *)bq->data + bq->front, old_front_until_overflow);
           bq->front += total_size_it;

           rear = (bq->front + bq->size) % bq->capacity;
           rear_ptr = (uint8_t *)bq->data + rear;
      }
    
     
      memcpy(rear_ptr, it, total_size_it);
    }
    else
    {
        printf("CAS 1 OU 2 \n");
        /**
         * CAS 1 ou 2
         * 2 memcpy nécessaires quand on overflow le buffer
         * cas où :
         *  [ --------xxxxxxx-- ] --> On veux rajouter 3 'x'
         *      front ^      ^ rear (front + size)
         * L'espace a la fin n'est pas assez grand, il va falloir wrapper
         */

        size_t space_to_end = bq->capacity - rear;

        if (bq->size + total_size_it > bq->capacity)
        {
            printf("Agrandissement\n");

            /**
             * cas où :
             *  [ -xxxxxxxxxxxxxxx- ] --> On veux rajouter 3 'x'
             *     ^front         ^rear
             */
            size_t new_cap = bq->capacity + total_size_it;
            bq->data = realloc(bq->data, new_cap);
            bq->capacity = new_cap;
            space_to_end = bq->capacity - rear;
        }

        size_t first_part = total_size_it;
        bool remaining = false;

        if (total_size_it > space_to_end)
        {
            printf("CAS 2\n");
            first_part = space_to_end;
            remaining = true;
        }
        memcpy(rear_ptr, it, first_part);

        if (remaining)
        {
            size_t second_part = total_size_it - first_part;
            memcpy((uint8_t *)bq->data, ((uint8_t *)it) + first_part, second_part);
        }
    }

    // fprintf(stdout, "bq->size(%ld) + total_size_it(%ld)\n", bq->size, total_size_it);
    bq->size += total_size_it;
    // fprintf(stdout, "bq->size(%ld)\n", bq->size);
}

void bbq_free(BatchBufferQueue *bq)
{
    // Free data
    free(bq->data);
    // Free bq
    free(bq);
}

size_t bbq_dequeue_into(BatchBufferQueue *bq, void *dst, size_t size)
{
    size_t read = 0;
    size_t count = 0;

    void *front_ptr = (uint8_t *)bq->data + bq->front;

    while (read < size && read < bq->size)
    {
        void *read_ptr = (uint8_t *)bq->data + ((bq->front + read) % bq->capacity);

        ItemBatchBufferQueue *it = (ItemBatchBufferQueue *)read_ptr;
        size_t total_size_item = sizeof(ItemBatchBufferQueue) + it->size;

        size_t nread = read + total_size_item;
        if (nread > size)
            break;

        read = nread;
        count++;
    }

    if (count == 0)
        return 0;

    size_t front_end_delta = bq->capacity - bq->front;
    size_t rear = (bq->front + bq->size) % bq->capacity;
    if (bq->front > rear)
    {
        /**
         * 2 memcpy nécessaires quand on overflow le buffer
         * cas ou :
         * [ xxxxxxxx----xx ] --> On veux dequeue 3 'x'
         *       rear^   ^front
         */

        size_t first_part = read;
        bool remaining = false;

        if (read > front_end_delta)
        {
            printf("CAS 2\n");
            first_part = front_end_delta;
            remaining = true;
        }
        memcpy((uint8_t *)dst, front_ptr, first_part);
        if (remaining)
        {
            size_t second_part = read - first_part;

            memcpy((uint8_t *)dst + first_part, (uint8_t *)bq->data, second_part);
        }
    }
    else
    {
        memcpy(dst, front_ptr, read);
    }

    bq->size -= read;
    bq->front = (bq->front + read) % bq->capacity;


    return count;
}

void bbq_print_info(BatchBufferQueue *bq)
{
    fprintf(stdout, "bq->size(%ld), bq->front(%ld), bq->capacity(%ld)\n", bq->size, bq->front, bq->capacity);
}
