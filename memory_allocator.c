#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

typedef char ALIGN[16];

union header {
    struct {
        size_t size;
        unsigned is_free;
        union header *next;
    } s;
    ALIGN stub;
};
typedef union header header_t;

header_t *head = NULL;
header_t *tail = NULL;

header_t *get_free_block(size_t size) {
    header_t *curr = head;
    while (curr) {
        if (curr->s.is_free && curr->s.size >= size) {
            return curr;
        }
        curr = curr->s.next;
    }
    return NULL;
}

void *custom_malloc(size_t size) {
    header_t *header;
    size_t total_size;
    void *block;

    if (!size) {
        return NULL;
    }

    header = get_free_block(size);
    if (header) {
        header->s.is_free = 0;
        return (void *)(header + 1);
    }

    total_size = sizeof(header_t) + size;
    block = sbrk(total_size);
    if (block == (void *)-1) {
        return NULL;
    }

    header = (header_t *)block;
    header->s.size = size;
    header->s.is_free = 0;
    header->s.next = NULL;

    if (!head) {
        head = header;
    }
    if (tail) {
        tail->s.next = header;
    }
    tail = header;

    return (void *)(header + 1);
}

void custom_free(void *block) {
    header_t *header;
    void *program_break;

    if (!block) {
        return;
    }

    header = (header_t *)block - 1;
    program_break = sbrk(0);

    if ((char *)block + header->s.size == program_break) {
        if (head == tail) {
            head = tail = NULL;
        } else {
            header_t *tmp = head;
            while (tmp->s.next != tail) {
                tmp = tmp->s.next;
            }
            tmp->s.next = NULL;
            tail = tmp;
        }
        sbrk(0 - sizeof(header_t) - header->s.size);
    } else {
        header->s.is_free = 1;
    }
}

void *custom_calloc(size_t num, size_t nsize) {
    size_t size;
    void *block;

    if (!num || !nsize) {
        return NULL;
    }

    size = num * nsize;
    block = custom_malloc(size);

    if (!block) {
        return NULL;
    }

    memset(block, 0, size);
    return block;
}

void *custom_realloc(void *block, size_t size) {
    header_t *header;
    void *ret;

    if (!block || !size) {
        return custom_malloc(size);
    }

    header = (header_t *)block - 1;
    if (header->s.size >= size) {
        return block;
    }

    ret = custom_malloc(size);
    if (ret) {
        memcpy(ret, block, header->s.size);
        custom_free(block);
    }

    return ret;
}

/* --- Verification Driver --- */
int main() {
    printf("Initializing verification for Custom Memory Allocator...\n");

    int *arr = (int *)custom_malloc(5 * sizeof(int));
    if (arr == NULL) {
        printf("Allocation Failed!\n");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
        printf("Allocated Value at Index %d: %d\n", i, arr[i]);
    }

    custom_free(arr);
    printf("Memory freed successfully.\n");
    return 0;
}
