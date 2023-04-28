/*************************************************************
    AUTHOR  : Vinicius Gabriel Angelozzi Verona de Resende
    MAIL    : verona.projects@tutanota.com        / 
              angelozv@etu.univ-grenoble-alpes.fr / 
              vinicius.verona@aluno.ufop.edu.br
    YEAR    : 2023
    COURSE  : Introduction to Cryptology
    PROFESSOR: Bruno Grenet
*************************************************************/

#include "impl.h"

int SAMPLES = 0;

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/****************************** AUXILIAR FUNCTIONS ******************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

void print_uint8(const uint8_t* x, const size_t len, const char* s) {
    for (int i = 0; i < len; i++) {
        printf("%d ", x[i]);
    }

    printf("%s", s);
}

// function to generate a random message
void random_message(uint8_t* m, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        m[i] = (uint8_t)xoshiro256starstar_random();
    }
}

int cmp_hashes(uint8_t* h1, uint8_t* h2, uint8_t size) {
    for (int i = 0; i < size; i++) {
        if (h1[i] != h2[i]) return 1;
    }

    return 0;
}

void copy_hash(uint8_t* dst, uint8_t* src, uint8_t size) {
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void copy_msg(uint8_t* dst, uint8_t* src, uint8_t size) {
    for (int i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

HASH_TABLE* create_hash_table(int size) {
    HASH_TABLE* hash_table = (HASH_TABLE*)malloc(size * sizeof(HASH_TABLE));

    for (int i = 0; i < size; i++) {
        hash_table[i].list = NULL;
    }

    return hash_table;
}

void free_hash_table(HASH_TABLE* hash_table, int size) {
    for (int i = 0; i < size; i++) {
        if (hash_table[i].list != NULL) {
            free_list(hash_table[i].list);
        }
    }

    free(hash_table);
}

void free_list(HASH_TABLE_LIST* list) {
    HASH_TABLE_CELL* aux = list->fst_cell;

    while (aux != NULL) {
        HASH_TABLE_CELL* aux2 = aux->next;
        free(aux);
        aux = aux2;
    }

    free(list);
}

int lookup_hash(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, int key, int size) {
    HASH_TABLE_CELL* aux;

    if (hash_table[key].list == NULL) return 0;

    aux = hash_table[key].list->fst_cell;

    // While the end of the list is not reached or the element is not found, change aux to the next element
    while (aux != NULL && (cmp_hashes(aux->item->hash, h, 6) != 0 || (size == BLEN && cmp_hashes(aux->item->message, m, BLEN) == 0))) {
        aux = aux->next;
    }

    // If the element was found, return 1
    if (aux != NULL) {
        return 1;
    }

    return 0;
}

void add_to_hash_list(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, long int key) {
    HASH_TABLE_CELL* new_cell = (HASH_TABLE_CELL*)malloc(sizeof(HASH_TABLE_CELL));
    HASH_TABLE_ITEM* new_item = (HASH_TABLE_ITEM*)malloc(sizeof(HASH_TABLE_ITEM));

    copy_hash(new_item->hash, h, 6);
    copy_msg(new_item->message, m, BLEN);

    new_cell->item = new_item;
    new_cell->next = NULL;

    if (hash_table[key].list == NULL) {
        hash_table[key].list = (HASH_TABLE_LIST*)malloc(sizeof(HASH_TABLE_LIST));
        hash_table[key].list->fst_cell = new_cell;
        hash_table[key].list->size = 0;
    } else {
        hash_table[key].list->lst_cell->next = new_cell;
    }

    hash_table[key].list->lst_cell = new_cell;
    hash_table[key].list->size += 1;
}

uint64_t get_hash_key(uint8_t* hash) {
    uint64_t key = 1;
    for (int i = 0; i < 6; i++) {
        key = key * 13 + hash[i];
    }

    return key % HT_SIZE;
}

int add_hash(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, long int key) {
    if (lookup_hash(hash_table, h, m, key, BLEN) != 1) {
        add_to_hash_list(hash_table, h, m, key);
        return 1;
    } else {
        return -1;
    }
    return 0;
}

uint8_t* get_message(HASH_TABLE* hash_table, long int key, uint8_t* h) {
    HASH_TABLE_CELL* aux;

    if (hash_table[key].list == NULL) return NULL;

    aux = hash_table[key].list->fst_cell;

    // While the end of the list is not reached or the element is not found, change aux to the next element
    while (aux != NULL && cmp_hashes(aux->item->hash, h, 6) != 0) {
        aux = aux->next;
    }

    // If the element was found, return 1
    if (aux != NULL) {
        return aux->item->message;
    }

    return NULL;
}


/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/************************************ TASKS *************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void iterated_tcz48_dm(const uint8_t* m, const size_t len, uint8_t h[6]) {
    uint64_t isMultiple = len % BLEN == 0;

    if (!isMultiple) return;

    for (uint64_t b = 0; b < len; b += BLEN) {
        tcz48_dm(m + b, h);
    }
}

void print(const uint8_t* x, const size_t len, const char* s) {
    for (int i = 0; i < len; i++) {
        printf("%02X", x[i]);
    }

    printf("%s", s);
}

double collision(uint8_t h[6], uint8_t* m1, uint8_t* m2) {
    // Hash table used to store all hashes generated and its messages
    HASH_TABLE* hash_table = create_hash_table(HT_SIZE);
    uint8_t null_hash[6] = {0};
    uint8_t init_hash[6]; copy_hash(init_hash, h, 6);

    while (1) {
        SAMPLES++;
        uint8_t h1[6], h2[6];

        // Generate a random message and compute its hash
        copy_hash(h2, init_hash, 6);
        random_message(m2, BLEN);
        iterated_tcz48_dm(m2, BLEN, h2);

        // Check if the hash is already in the hash table
        uint64_t hidx = get_hash_key(h2);

        if (hash_table[hidx].list != NULL) {
            long int idx = lookup_hash(hash_table, h2, m2, hidx, BLEN);

            if (idx) {
                // Collision found, copy hash to h and return
                copy_hash(h, h2, 6);
                copy_msg(m1, get_message(hash_table, hidx, h2), BLEN);
                break;

            } else {
                add_hash(hash_table, h2, m2, hidx);
            }

        } else {
            // Add the hash to the hash table
            add_hash(hash_table, h2, m2, hidx);
        }
    }

    // Free hash_table memory
    free_hash_table(hash_table, HT_SIZE);
    return log2(SAMPLES);
}

double multicollision(int t) {
    uint8_t h[6], m1[BLEN], m2[BLEN];
    uint8_t pairs[2*t][BLEN], hashes[2*t][6];

    if (SAMPLES != 0) SAMPLES = 0;

    h[0] = IVB0;
    h[1] = IVB1;
    h[2] = IVB2;
    h[3] = IVB3;
    h[4] = IVB4;
    h[5] = IVB5;

    for (int i = 0; i < t; i++) {
        collision(h, m1, m2);

        // Store m1 and m2 so that they can be used to create future messages with the same hash
        copy_msg(pairs[i], m1, BLEN);
        copy_msg(pairs[i+t], m2, BLEN);
        copy_hash(hashes[i], h, 6);
    }

    // Print all blocks of messages and the commom hash
    for (int i = 0; i < t; i++) {
        printf("Block %d: ", i+1);
        print(pairs[i], BLEN, " ");
        print(pairs[i+t], BLEN, " ");
        print(hashes[i], 6, "\n");
    }


    #ifdef PRINT_UINT8_MESSAGES
        // Print each of the t pair of messages in uint8_t format
        for (int i = 0; i < t; i++) {
            printf("m1: ");
            print_uint8(pairs[i], BLEN, " || ");
            printf("m2: ");
            print_uint8(pairs[i+t], BLEN, "\n");
        }
    #endif

    return log2(SAMPLES);
}

double unbalanced_collision(uint8_t h[6], uint8_t m1[16], uint8_t* m2, const size_t len) {
    if (len*BLEN % BLEN != 0) {
        printf("Error: len must be a multiple of BLEN\n");
        return 0;
    }
    
    // Hash table used to store all hashes generated and its messages
    HASH_TABLE* hash_table = create_hash_table(HT_SIZE);
    uint8_t null_hash[6] = {0}, h1[6], h2[6]; copy_hash(h1, h, 6); copy_hash(h2, h, 6);

    // generate a random message and compute its hash
    // Add the hash to the hash table
    random_message(m1, BLEN);
    iterated_tcz48_dm(m1, BLEN, h1);
    add_hash(hash_table, h1, m1, get_hash_key(h1));

    // Generate a random message and compute its hash
    random_message(m2, (len-1)*BLEN);

    int count = 0;

    while (1) {
        count++;
        SAMPLES++;
        copy_hash(h1, h, 6); copy_hash(h2, h, 6);

        // Generate a random message and compute its hash
        random_message(m1, BLEN);
        iterated_tcz48_dm(m1, BLEN, h1);

        random_message(m2 + ((len-1)*BLEN), BLEN);
        iterated_tcz48_dm(m2, len*BLEN, h2);

        // Check if the hash is already in the hash table
        uint64_t hidx1 = get_hash_key(h1);
        uint64_t hidx2 = get_hash_key(h2);

        add_hash(hash_table, h1, m1, hidx1);

        if (hash_table[hidx2].list != NULL) {
            long int idx = lookup_hash(hash_table, h2, m2, hidx2, len*BLEN);

            if (idx) {
                // Collision found, copy hash to h and return
                copy_hash(h, h2, 6);
                copy_msg(m1, get_message(hash_table, hidx2, h2), BLEN);
                break;
            }

        }
    }

    #ifdef PRINT_UINT8_MESSAGES
        // Print the pair of messages in uint8_t format
        printf("m1: ");
        print_uint8(m1, BLEN, " || ");
        printf("m2: ");
        print_uint8(m2, len*BLEN, "\n");
    #endif

    // Free hash_table memory
    free_hash_table(hash_table, HT_SIZE);
    return log2(SAMPLES);
}

double expandable_message(int t) {
    uint8_t h[6], m[BLEN], *m2;
    double result;

    m2 = (uint8_t*) malloc((pow(2,t)+1) * BLEN * sizeof(uint8_t));
    SAMPLES = 0;

    h[0]  = IVB0; h[1]  = IVB1; h[2]  = IVB2; h[3]  = IVB3; h[4]  = IVB4; h[5]  = IVB5;

    for (int i = 0; i < t+1; i++) {

        unbalanced_collision(h, m, m2, pow(2,i)+1);
        print(m, BLEN, " ");
        print(m2, (pow(2,i)+1)*BLEN, " - ");
        print(h, 6, "\n");

    }

    // Free hash_table memory
    free(m2);
    
    result = log2(SAMPLES);
    return result;
}