/*************************************************************
    AUTHOR  : Vinicius Gabriel Angelozzi Verona de Resende
    MAIL    : verona.projects@tutanota.com        / 
              angelozv@etu.univ-grenoble-alpes.fr / 
              vinicius.verona@aluno.ufop.edu.br
    YEAR    : 2023
    COURSE  : Introduction to Cryptology
    PROFESSOR: Bruno Grenet
*************************************************************/

#ifndef IMPL_H
#define IMPL_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mc48.h"
#include "xoshiro256starstar.h"

// #define DEBUG
// #define DEBUG_MESSAGE                                     // Uncomment to print the messages in uint8_t format in multicollision

// #define VERBOSE                                           // Uncomment to print the collision
#define BLEN 16                                           // Block Length in Bytes (16 bytes = 128 bits)
#define BLEN_BITS 128                                     // Block Length in Bits
#define HASH_SIZE 48                                      // Hash size in bits
#define HT_SIZE (int)ceil(sqrt(pow(2, (HASH_SIZE - 1))))  // Size of the hash table

// Hash table struct
typedef struct {
    uint8_t hash[6];
    uint8_t message[BLEN];
} HASH_TABLE_ITEM;

typedef struct CELL {
    HASH_TABLE_ITEM* item;
    struct CELL* next;
} HASH_TABLE_CELL;

typedef struct {
    HASH_TABLE_CELL *fst_cell, *lst_cell;
    int size;
} HASH_TABLE_LIST;

typedef struct {
    HASH_TABLE_LIST* list;
} HASH_TABLE;

/**
 * Given a hash, return the key of the hash table
 * In this case the index of the hash table is calculated as follows:
 * The key starts with a value 1, and each byte of the hash is multiplied by the prime 13 and added to the previou value of the key.
 * In the end, the key is modded by the size of the hash table
 */
uint64_t get_hash_key(uint8_t* hash);

/**
 * Compare two byte arrays
 * @param h1 the first byte array
 * @param h2 the second byte array
 * @param size the byte-length of both arrays
 * Output: 0 if the arrays are equal, 1 otherwise
 */
int cmp_hashes(uint8_t* h1, uint8_t* h2, uint8_t size);
void copy_hash(uint8_t* dst, uint8_t* src, uint8_t size);

/**
 * Add an element to the hash table
 */
int add_hash(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, long int idx);

/**
 * Given a hash_table look if h is found in hash_table[key]
 * If it is found, return 1
 * @param size is the length in bytes of the message m
 */
int lookup_hash(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, int key, int size);

/**
 * Add a hash to the list in case of index collision
 */
void add_to_hash_list(HASH_TABLE* hash_table, uint8_t* h, uint8_t* m, long int key);
void free_list(HASH_TABLE_LIST* list);
void free_hash_table(HASH_TABLE* hash_table, int size);
HASH_TABLE* create_hash_table(int size);

/**
 * Creates a function get_message that given a hash and a key, returns the message associated with that hash
*/
uint8_t* get_message(HASH_TABLE* hash_table, long int key, uint8_t* h);

/*****************************************************************/
/***************************** Tasks *****************************/
/*****************************************************************/

/**
 * Given a message, compress the message using the Davies-Meyer compression function
 * @param m the message to be compressed, whose byte-length is required to be a multiple of 'w'
 * @param len the byte-length of 'm'
 * @param h variable in which the result of the compression is written to.
 */
void iterated_tcz48_dm(const uint8_t* m, const size_t len, uint8_t h[6]);

/**
 * Print a byte array in hexadecimal format followed by S
 * @param x the byte array to be printed
 * @param len the byte-length of 'x'
 * @param s the string to be printed after the byte array
 */
void print(const uint8_t* x, const size_t len, const char* s);

/**
 * 1 - Create a hash table
 * 2 - Generate a random message m2 and compute its hash h2
 * 3 - Check if h2 is in the hash table
 * 4 - If h2 is in the hash table, then we found a collision
 * 5 - If h2 is not in the hash table, then add it with the message to the hash table and go to step 2
 */
void collision(uint8_t h[6], uint8_t* m1, uint8_t* m2);

/**
 * Find a multicollision 2^t executing collision t times 
*/
double multicollision(int t);

/**
 * Finds a collision of the form f(h, m1) = F(h, m2) where m2 has len blocks
 * @param h array to store the hash of the collision
 * @param m1 array to store the first message of the collision
 * @param m2 array to store the second message of the collision
*/
double unbalanced_collision(uint8_t h[6], uint8_t m1[16], uint8_t* m2, const size_t len);

/**
 * Computes an expandable message for F of 2^t messages of length t to t + 2^t − 1
*/
double expandable_message(int t);

#endif