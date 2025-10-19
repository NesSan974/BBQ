// gcc  main.c BBQ.c -I. -o bbq_test.out
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "BBQ.h"

int main(int argc, char **argv)
{

    // Set-up
    uint8_t item_buffer_16[16];
    ItemBatchBufferQueue *it_10b = (ItemBatchBufferQueue *)item_buffer_16;
    it_10b->size = 2;
    it_10b->payload[0] = 5;
    it_10b->payload[1] = 20;

    uint8_t item_buffer_32[32];
    ItemBatchBufferQueue *it_30b = (ItemBatchBufferQueue *)item_buffer_32;
    it_30b->size = 22;
    for (size_t i = 0; i < 22; i++)
        it_30b->payload[i] = i;

    // --------

    // Init
    BatchBufferQueue *bbq_buffer = bbq_init(21);
    assert(bbq_buffer != NULL);
    assert(bbq_buffer->capacity == 21);
    assert(bbq_buffer->front == 0);
    assert(bbq_buffer->size == 0);
    assert(bbq_buffer->data != NULL);

    bbq_enqueue(bbq_buffer, it_10b);
    printf("add %ld\n", sizeof(ItemBatchBufferQueue) + it_10b->size);
    // [ |xxxxxxxxxx,----------- ]

    assert(bbq_buffer->size == 10);
    assert(bbq_buffer->data[0].size == it_10b->size);

    // CAS 1 :
    bbq_enqueue(bbq_buffer, it_10b);
    printf("add %ld\n", sizeof(ItemBatchBufferQueue) + it_10b->size);
    bbq_print_info(bbq_buffer);
    // [ |xxxxxxxxxx,xxxxxxxxxx- ]

    assert(bbq_buffer->size == 20);


    uint8_t dequeue_buffer_16[16];
    int n = bbq_dequeue_into(bbq_buffer, (void *)dequeue_buffer_16, 11);
    printf("dequeu %d data\n", n);
    bbq_print_info(bbq_buffer);
    // [----------|xxxxxxxxxx- ]

    assert(bbq_buffer->size == 10);
    assert(bbq_buffer->front == 10);


    // CAS 2 :
    bbq_enqueue(bbq_buffer, it_10b);
    printf("add %ld\n", sizeof(ItemBatchBufferQueue) + it_10b->size);
    bbq_print_info(bbq_buffer);
    // [ xxxxxxxxx|xxxxxxxxxx,x ]

    assert(bbq_buffer->size == 20);


    n = bbq_dequeue_into(bbq_buffer, (void *)dequeue_buffer_16, 11);
    printf("dequeu %d data\n", n);
    bbq_print_info(bbq_buffer);
    // [ xxxxxxxxx-----------|x ]

    assert(bbq_buffer->size == 10);
    assert(bbq_buffer->front == 20);


    // CAS 3 :
    bbq_enqueue(bbq_buffer, it_10b);
    printf("add %ld\n", sizeof(ItemBatchBufferQueue) + it_10b->size);
    bbq_print_info(bbq_buffer);
    // [ xxxxxxxxx,xxxxxxxxxx-|x ]

    assert(bbq_buffer->size == 20);


    // CAS 3 + Agrandissement
    bbq_enqueue(bbq_buffer, it_10b);
    printf("add %ld\n", sizeof(ItemBatchBufferQueue) + it_10b->size);
    bbq_print_info(bbq_buffer);
    // [ xxxxxxxxx,xxxxxxxxxx,xxxxxxxxxx-|x ]

    assert(bbq_buffer->size == 30);
    assert(bbq_buffer->front == 30);
    assert(bbq_buffer->capacity == 31);


    n = bbq_dequeue_into(bbq_buffer, (void *)dequeue_buffer_16, 11);
    printf("dequeu %d data\n", n);
    bbq_print_info(bbq_buffer);
    // [ ---------|xxxxxxxxxx,xxxxxxxxxx-- ]

    assert(bbq_buffer->size == 20);
    assert(bbq_buffer->front == 9);


    return 0;
}