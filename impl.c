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

/**
 * Given a uint8_t array, return a string representation of the array
*/
void get_string_hex(uint8_t* h, char* hash_string, int size) {
    // There is an error here
    for (int i = 0; i < size; i++) {
        sprintf(hash_string + i, "%02X", h[i]);
    }
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
        #ifdef VERBOSE

            char message_string1[2*BLEN]; get_string_hex(aux->item->message, message_string1, BLEN);
            char message_string2[2*size]; get_string_hex(m, message_string2, size);

            printf("-: %s %s %lf\n", message_string1, message_string2, log2(SAMPLES));

        #endif
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

    #ifdef DEBUG
    // simple IV
    // h[0] = IVB0;
    // h[1] = IVB1;
    // h[2] = IVB2;
    // h[3] = IVB3;
    // h[4] = IVB4;
    // h[5] = IVB5;
    #endif

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

    int count = 0;


    while (1) {
        count++;
        SAMPLES++;
        uint8_t h1[6], h2[6];

        // Generate a random message and compute its hash
        copy_hash(h2, init_hash, 6);
        random_message(m2, BLEN);
        iterated_tcz48_dm(m2, BLEN, h2);

        // Check if the hash is already in the hash table
        uint64_t hidx = get_hash_key(h2);

        #ifdef DEBUG
            if ((count - 1) % 1000000 == 0) {
                printf("Count: %d\n", (count - 1) / 1000000);
            }
        #endif

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

        print(m1, BLEN, " ");
        print(m2, BLEN, " ");
        print(h, 6, "\n");
    }

    // Print each of the t pair of messages with the same hash
    #ifdef VERBOSE
        for (int i = 0; i < t; i++) {
            char message_string1[2*BLEN]; get_string_hex(pairs[i], message_string1, BLEN);
            char message_string2[2*BLEN]; get_string_hex(pairs[i+t], message_string2, BLEN);

            printf("%s %s\n", message_string1, message_string2);
        }
    #endif

    // Print all possible combination of pairs of messages with the same hash, in a way that each pair is only printed once
    for (int i = 0; i < t; i++) {
        for (int j = i+1; j < t; j++) {
            char message_string1[2*BLEN]; get_string_hex(pairs[i], message_string1, BLEN);
            char message_string2[2*BLEN]; get_string_hex(pairs[i+t], message_string2, BLEN);
            char message_string3[2*BLEN]; get_string_hex(pairs[j], message_string3, BLEN);
            char message_string4[2*BLEN]; get_string_hex(pairs[j+t], message_string4, BLEN);

            printf("H(%s || %s) == H(%s || %s) => ", message_string1, message_string3, message_string2, message_string4);
            print(h, 6, "\n");
        }
    }


    #ifdef DEBUG_MESSAGE
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

    #ifdef DEBUG_MESSAGE
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


// Geting stuc for some reason
double expandable_message(int t) {
    uint8_t h[6], h1[6], h2[6], m[BLEN], *m2;
    double result;

    m2   = (uint8_t*) malloc((pow(2,t)+1) * BLEN * sizeof(uint8_t));
    SAMPLES = 0;

    h[0]  = IVB0; h[1]  = IVB1; h[2]  = IVB2; h[3]  = IVB3; h[4]  = IVB4; h[5]  = IVB5;
    h1[0] = IVB0; h1[1] = IVB1; h1[2] = IVB2; h1[3] = IVB3; h1[4] = IVB4; h1[5] = IVB5;
    h2[0] = IVB0; h2[1] = IVB1; h2[2] = IVB2; h2[3] = IVB3; h2[4] = IVB4; h2[5] = IVB5;

    // look for 1-block message collision
    unbalanced_collision(h, m, m2, 1);
    print(m, BLEN, " - ");
    print(h, 6, "\n");
    print(m2, BLEN, " - ");
    print(h, 6, "\n");

    
    ucollision(h,m2,2);

    // for (int i = 2; i <= pow(2,t)+1; i++) {
        // ucollision(h,m2,i);

        // int counter = 0;
        // while(1) {
        //     SAMPLES++;
        //     counter++;
        //     copy_hash(h2, h1, 6);

        //     if(counter % 1000000 == 0) {
        //         printf("counter: %d\n", counter / 1000000);
        //     }

        //     // look for collision between h and a message of length 1 + 2^i
        //     random_message(m2 + ((i-1)*BLEN), BLEN);
        //     iterated_tcz48_dm(m2, i*BLEN, h2);

        //     if (cmp_hashes(h,h2, 6) == 0) {
                // print(m2, i*BLEN, " - ");
                print(m2, 1*BLEN, " - ");
                print(h2, 6, "\n");
        //         break;
        //     }
        // }
    // }

    result = log2(SAMPLES);
    return result;
}