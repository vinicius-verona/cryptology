#include <stdio.h>
#include "impl.h"
// #define STATIC_TEST

void static_test_t2();
void static_test_t4();
void static_test_unbalanced();

uint8_t init_h[6] = {IVB0, IVB1, IVB2, IVB3, IVB4, IVB5};

void reset_h(uint8_t* h, uint8_t* init_h) {
    for (int i = 0; i < 6; i++) {
        h[i] = init_h[i];
    }
}

int main(int argc, char** argv) {
#ifdef STATIC_TEST
    static_test_t2();
    static_test_t4();
    static_test_unbalanced();
#endif
#ifndef STATIC_TEST

    char msg1[33];
    char msg2[33];

    // Hash a message using the ht48 function implemented in mc48.c
    uint8_t h[6];
    uint8_t m1[16];
    uint8_t m2[16];
    uint8_t m3[32];

    printf("************* COLLISION *************\n\n");
    // double r_collision = collision(h, m1, m2);
    // printf("[ Collision ] ");
    // print(m1, BLEN, " ");
    // print(m2, BLEN, " ");
    // printf("%lf\n", r_collision);

    printf("\n************* MULTICOLLISION *************\n\n");
    // int samples_t1 = multicollision(1);
    // printf("[ Multicollision ] Samples for t = 1: %d\n", samples_t1);
    
    int samples_t2 = multicollision(2);
    printf("[ Multicollision ] Samples for t = 2: %d\n", samples_t2);
    
    // int samples_t3 = multicollision(3);
    // printf("[ Multicollision ] Samples for t = 3: %d\n", samples_t3);

    // int samples_t4 = multicollision(4);
    // printf("[ Multicollision ] Samples for t = 4: %d\n", samples_t4);

    printf("\n************* Expandable message *************\n\n");
    reset_h(h, init_h);
    // double r_unbalanced_collision  = unbalanced_collision(h, m1, m3, 2);
    // printf("[ Unbalanced Collision ] Results for l = 2: %lf\n\n", r_unbalanced_collision);

    // double r_expandable_message_t1 = expandable_message(1);
    // double r_expandable_message_t2 = expandable_message(2);
    // double r_expandable_message_t3 = 0;//expandable_message(3);
    // double r_expandable_message_t4 = 0;//expandable_message(4);
    // printf("[ Expandable Message ] Results for t = 1, t = 2. t = 3 and t = 4: %lf, %lf, %lf, %lf\n", 
            // r_expandable_message_t1, r_expandable_message_t2, r_expandable_message_t3, r_expandable_message_t4);

#endif
}

void concatenate_arrays(uint8_t *arr1, uint8_t *arr2, uint8_t *result, size_t arr1_size, size_t arr2_size) {
    memcpy(result, arr1, arr1_size);
    memcpy(result + arr1_size, arr2, arr2_size);
}

void static_test_t2() {
    printf(" Static test for t = 2\n");

    // Creates messages and hash using iterated_tcz48_dm
    uint8_t h[6]; reset_h(h, init_h);
    uint8_t m_1[16] = {34, 9, 96, 16, 180, 75, 8, 222, 250, 33, 125, 106, 73, 226, 67, 193};
    uint8_t m_2[16] = {66, 0, 38, 173, 55, 121, 0, 250, 13, 93, 93, 103, 169, 248, 107, 60};
    uint8_t m_3[16] = {8, 40, 4, 235, 201, 129, 240, 112, 138, 161, 121, 243, 166, 217, 238, 126};
    uint8_t m_4[16] = {180, 102, 203, 226, 92, 89, 210, 124, 26, 79, 175, 173, 172, 30, 36, 48};

    // for each of the 4 messages, print the message and hash
    for (int i = 0; i < 4; i++) {
        uint8_t* m = i == 0 ? m_1 : i == 1 ? m_2
                                : i == 2   ? m_3
                                           : m_4;
        iterated_tcz48_dm(m, 16, h);
        printf("m%d: ", i + 1);
        print(m, 16, "  -> ");
        print(h, 6, "\n");

        reset_h(h, init_h);
    }

    printf("\n########################\n\n");

    // combinate the 4 messages in the following way m1 concatentated with m3 and m2 concatenated with m4, m1 concatentated with m4 and m2 concatenated with m3
    // generate the hash for each of the 4 combinations and print them with the combination
    uint8_t m1_m3[32];
    uint8_t m2_m4[32];
    uint8_t m1_m4[32];
    uint8_t m2_m3[32];

    for (int i = 0; i < 16; i++) {
        m1_m3[i] = m_1[i]; m1_m3[i + 16] = m_3[i];
        m2_m4[i] = m_2[i]; m2_m4[i + 16] = m_4[i];
        m1_m4[i] = m_1[i]; m1_m4[i + 16] = m_4[i];
        m2_m3[i] = m_2[i]; m2_m3[i + 16] = m_3[i];
    }


    uint8_t m1_m3_h[6]; reset_h(m1_m3_h, init_h);
    uint8_t m2_m4_h[6]; reset_h(m2_m4_h, init_h);
    uint8_t m1_m4_h[6]; reset_h(m1_m4_h, init_h);
    uint8_t m2_m3_h[6]; reset_h(m2_m3_h, init_h);

    iterated_tcz48_dm(m_1, BLEN, m1_m3_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_h);

    print(m1_m3, 32, " -> m1 concat m3 -> "); print(m1_m3_h, 6, "\n");
    print(m2_m4, 32, " -> m2 concat m4 -> "); print(m2_m4_h, 6, "\n");
    print(m1_m4, 32, " -> m1 concat m4 -> "); print(m1_m4_h, 6, "\n");
    print(m2_m3, 32, " -> m2 concat m3 -> "); print(m2_m3_h, 6, "\n");
}

void static_test_t4() {
    printf("\n\n Static test t = 4\n\n");

    // Creates messages and hash using iterated_tcz48_dm
    uint8_t h[6]; reset_h(h, init_h);
    uint8_t m_1[16] = {227, 225, 100, 217, 241, 96, 19, 106, 183, 98, 48, 185, 189, 28, 132, 10};
    uint8_t m_2[16] = {166, 171, 175, 192, 38, 133, 157, 23, 165, 21, 180, 173, 214, 162, 72, 125};

    uint8_t m_3[16] = {42, 19, 26, 224, 61, 99, 229, 46, 60, 251, 102, 170, 252, 1, 110, 188};
    uint8_t m_4[16] = {152, 14, 159, 118, 168, 24, 27, 169, 23, 197, 89, 91, 27, 117, 29, 202};
    
    uint8_t m_5[16] = {219, 82, 146, 197, 68, 5, 237, 19, 94, 51, 182, 123, 97, 213, 126, 214};
    uint8_t m_6[16] = {161, 52, 144, 158, 204, 163, 42, 220, 224, 102, 243, 224, 62, 143, 214, 238};
    
    uint8_t m_7[16] = {92, 218, 174, 12, 83, 161, 91, 156, 24, 138, 127, 84, 59, 178, 90, 8};
    uint8_t m_8[16] = {192, 41, 185, 14, 45, 102, 171, 97, 53, 102, 144, 57, 152, 95, 225, 216};

    // for each of the 8 messages, print the message and hash
    for (int i = 0; i < 8; i++) {
        uint8_t* m = i == 0 ? m_1 : i == 1 ? m_2
                                : i == 2   ? m_3
                                : i == 3   ? m_4
                                : i == 4   ? m_5
                                : i == 5   ? m_6
                                : i == 6   ? m_7
                                           : m_8;
        iterated_tcz48_dm(m, 16, h);
        print(m, 16, "  -> ");
        print(h, 6, "\n");

        // reset h
        reset_h(h, init_h);
    }

    printf("\n########################\n\n");

    uint8_t m1_m3[32], m1_m3_h[6];             reset_h(m1_m3_h, init_h);       concatenate_arrays(m_1, m_3, m1_m3, 32 - 16, 16);
    uint8_t m2_m3[32], m2_m3_h[6];             reset_h(m2_m3_h, init_h);       concatenate_arrays(m_2, m_3, m2_m3, 32 - 16, 16);
    uint8_t m1_m4[32], m1_m4_h[6];             reset_h(m1_m4_h, init_h);       concatenate_arrays(m_1, m_4, m1_m4, 32 - 16, 16);
    uint8_t m2_m4[32], m2_m4_h[6];             reset_h(m2_m4_h, init_h);       concatenate_arrays(m_2, m_4, m2_m4, 32 - 16, 16);
    uint8_t m1_m3_m5[48], m1_m3_m5_h[6];       reset_h(m1_m3_m5_h, init_h);    concatenate_arrays(m1_m3, m_5, m1_m3_m5, 48 - 16, 16);
    uint8_t m2_m3_m5[48], m2_m3_m5_h[6];       reset_h(m2_m3_m5_h, init_h);    concatenate_arrays(m2_m3, m_5, m2_m3_m5, 48 - 16, 16);
    uint8_t m1_m4_m5[48], m1_m4_m5_h[6];       reset_h(m1_m4_m5_h, init_h);    concatenate_arrays(m1_m4, m_5, m1_m4_m5, 48 - 16, 16);
    uint8_t m2_m4_m5[48], m2_m4_m5_h[6];       reset_h(m2_m4_m5_h, init_h);    concatenate_arrays(m2_m4, m_5, m2_m4_m5, 48 - 16, 16);
    uint8_t m1_m3_m6[48], m1_m3_m6_h[6];       reset_h(m1_m3_m6_h, init_h);    concatenate_arrays(m1_m3, m_6, m1_m3_m6, 48 - 16, 16);
    uint8_t m2_m3_m6[48], m2_m3_m6_h[6];       reset_h(m2_m3_m6_h, init_h);    concatenate_arrays(m2_m3, m_6, m2_m3_m6, 48 - 16, 16);
    uint8_t m1_m4_m6[48], m1_m4_m6_h[6];       reset_h(m1_m4_m6_h, init_h);    concatenate_arrays(m1_m4, m_6, m1_m4_m6, 48 - 16, 16);
    uint8_t m2_m4_m6[48], m2_m4_m6_h[6];       reset_h(m2_m4_m6_h, init_h);    concatenate_arrays(m2_m4, m_6, m2_m4_m6, 48 - 16, 16);
    uint8_t m1_m3_m5_m7[64], m1_m3_m5_m7_h[6]; reset_h(m1_m3_m5_m7_h, init_h); concatenate_arrays(m1_m3_m5, m_7, m1_m3_m5_m7, 64 - 16, 16);
    uint8_t m2_m3_m5_m7[64], m2_m3_m5_m7_h[6]; reset_h(m2_m3_m5_m7_h, init_h); concatenate_arrays(m2_m3_m5, m_7, m2_m3_m5_m7, 64 - 16, 16);
    uint8_t m1_m4_m5_m7[64], m1_m4_m5_m7_h[6]; reset_h(m1_m4_m5_m7_h, init_h); concatenate_arrays(m1_m4_m5, m_7, m1_m4_m5_m7, 64 - 16, 16);
    uint8_t m2_m4_m5_m7[64], m2_m4_m5_m7_h[6]; reset_h(m2_m4_m5_m7_h, init_h); concatenate_arrays(m2_m4_m5, m_7, m2_m4_m5_m7, 64 - 16, 16);
    uint8_t m1_m3_m6_m7[64], m1_m3_m6_m7_h[6]; reset_h(m1_m3_m6_m7_h, init_h); concatenate_arrays(m1_m3_m6, m_7, m1_m3_m6_m7, 64 - 16, 16);
    uint8_t m2_m3_m6_m7[64], m2_m3_m6_m7_h[6]; reset_h(m2_m3_m6_m7_h, init_h); concatenate_arrays(m2_m3_m6, m_7, m2_m3_m6_m7, 64 - 16, 16);
    uint8_t m1_m4_m6_m7[64], m1_m4_m6_m7_h[6]; reset_h(m1_m4_m6_m7_h, init_h); concatenate_arrays(m1_m4_m6, m_7, m1_m4_m6_m7, 64 - 16, 16);
    uint8_t m2_m4_m6_m7[64], m2_m4_m6_m7_h[6]; reset_h(m2_m4_m6_m7_h, init_h); concatenate_arrays(m2_m4_m6, m_7, m2_m4_m6_m7, 64 - 16, 16);
    uint8_t m1_m3_m5_m8[64], m1_m3_m5_m8_h[6]; reset_h(m1_m3_m5_m8_h, init_h); concatenate_arrays(m1_m3_m5, m_8, m1_m3_m5_m8, 64 - 16, 16);
    uint8_t m2_m3_m5_m8[64], m2_m3_m5_m8_h[6]; reset_h(m2_m3_m5_m8_h, init_h); concatenate_arrays(m2_m3_m5, m_8, m2_m3_m5_m8, 64 - 16, 16);
    uint8_t m1_m4_m5_m8[64], m1_m4_m5_m8_h[6]; reset_h(m1_m4_m5_m8_h, init_h); concatenate_arrays(m1_m4_m5, m_8, m1_m4_m5_m8, 64 - 16, 16);
    uint8_t m2_m4_m5_m8[64], m2_m4_m5_m8_h[6]; reset_h(m2_m4_m5_m8_h, init_h); concatenate_arrays(m2_m4_m5, m_8, m2_m4_m5_m8, 64 - 16, 16);
    uint8_t m1_m3_m6_m8[64], m1_m3_m6_m8_h[6]; reset_h(m1_m3_m6_m8_h, init_h); concatenate_arrays(m1_m3_m6, m_8, m1_m3_m6_m8, 64 - 16, 16);
    uint8_t m2_m3_m6_m8[64], m2_m3_m6_m8_h[6]; reset_h(m2_m3_m6_m8_h, init_h); concatenate_arrays(m2_m3_m6, m_8, m2_m3_m6_m8, 64 - 16, 16);
    uint8_t m1_m4_m6_m8[64], m1_m4_m6_m8_h[6]; reset_h(m1_m4_m6_m8_h, init_h); concatenate_arrays(m1_m4_m6, m_8, m1_m4_m6_m8, 64 - 16, 16);
    uint8_t m2_m4_m6_m8[64], m2_m4_m6_m8_h[6]; reset_h(m2_m4_m6_m8_h, init_h); concatenate_arrays(m2_m4_m6, m_8, m2_m4_m6_m8, 64 - 16, 16); 

    // hash combinations
    iterated_tcz48_dm(m_1, BLEN, m1_m3_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m5_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m5_h); iterated_tcz48_dm(m_5, BLEN, m1_m3_m5_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m5_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m5_h); iterated_tcz48_dm(m_5, BLEN, m2_m3_m5_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m5_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m5_h); iterated_tcz48_dm(m_5, BLEN, m1_m4_m5_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m5_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m5_h); iterated_tcz48_dm(m_5, BLEN, m2_m4_m5_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m6_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m6_h); iterated_tcz48_dm(m_6, BLEN, m1_m3_m6_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m6_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m6_h); iterated_tcz48_dm(m_6, BLEN, m2_m3_m6_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m6_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m6_h); iterated_tcz48_dm(m_6, BLEN, m1_m4_m6_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m6_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m6_h); iterated_tcz48_dm(m_6, BLEN, m2_m4_m6_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m5_m7_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m5_m7_h); iterated_tcz48_dm(m_5, BLEN, m1_m3_m5_m7_h); iterated_tcz48_dm(m_7, BLEN, m1_m3_m5_m7_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m5_m7_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m5_m7_h); iterated_tcz48_dm(m_5, BLEN, m2_m3_m5_m7_h); iterated_tcz48_dm(m_7, BLEN, m2_m3_m5_m7_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m5_m7_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m5_m7_h); iterated_tcz48_dm(m_5, BLEN, m1_m4_m5_m7_h); iterated_tcz48_dm(m_7, BLEN, m1_m4_m5_m7_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m5_m7_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m5_m7_h); iterated_tcz48_dm(m_5, BLEN, m2_m4_m5_m7_h); iterated_tcz48_dm(m_7, BLEN, m2_m4_m5_m7_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m6_m7_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m6_m7_h); iterated_tcz48_dm(m_6, BLEN, m1_m3_m6_m7_h); iterated_tcz48_dm(m_7, BLEN, m1_m3_m6_m7_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m6_m7_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m6_m7_h); iterated_tcz48_dm(m_6, BLEN, m2_m3_m6_m7_h); iterated_tcz48_dm(m_7, BLEN, m2_m3_m6_m7_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m6_m7_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m6_m7_h); iterated_tcz48_dm(m_6, BLEN, m1_m4_m6_m7_h); iterated_tcz48_dm(m_7, BLEN, m1_m4_m6_m7_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m6_m7_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m6_m7_h); iterated_tcz48_dm(m_6, BLEN, m2_m4_m6_m7_h); iterated_tcz48_dm(m_7, BLEN, m2_m4_m6_m7_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m5_m8_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m5_m8_h); iterated_tcz48_dm(m_5, BLEN, m1_m3_m5_m8_h); iterated_tcz48_dm(m_8, BLEN, m1_m3_m5_m8_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m5_m8_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m5_m8_h); iterated_tcz48_dm(m_5, BLEN, m2_m3_m5_m8_h); iterated_tcz48_dm(m_8, BLEN, m2_m3_m5_m8_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m5_m8_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m5_m8_h); iterated_tcz48_dm(m_5, BLEN, m1_m4_m5_m8_h); iterated_tcz48_dm(m_8, BLEN, m1_m4_m5_m8_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m5_m8_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m5_m8_h); iterated_tcz48_dm(m_5, BLEN, m2_m4_m5_m8_h); iterated_tcz48_dm(m_8, BLEN, m2_m4_m5_m8_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m3_m6_m8_h); iterated_tcz48_dm(m_3, BLEN, m1_m3_m6_m8_h); iterated_tcz48_dm(m_6, BLEN, m1_m3_m6_m8_h); iterated_tcz48_dm(m_8, BLEN, m1_m3_m6_m8_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m3_m6_m8_h); iterated_tcz48_dm(m_3, BLEN, m2_m3_m6_m8_h); iterated_tcz48_dm(m_6, BLEN, m2_m3_m6_m8_h); iterated_tcz48_dm(m_8, BLEN, m2_m3_m6_m8_h);
    iterated_tcz48_dm(m_1, BLEN, m1_m4_m6_m8_h); iterated_tcz48_dm(m_4, BLEN, m1_m4_m6_m8_h); iterated_tcz48_dm(m_6, BLEN, m1_m4_m6_m8_h); iterated_tcz48_dm(m_8, BLEN, m1_m4_m6_m8_h);
    iterated_tcz48_dm(m_2, BLEN, m2_m4_m6_m8_h); iterated_tcz48_dm(m_4, BLEN, m2_m4_m6_m8_h); iterated_tcz48_dm(m_6, BLEN, m2_m4_m6_m8_h); iterated_tcz48_dm(m_8, BLEN, m2_m4_m6_m8_h);

    // print combinations
    printf("m1_m3: ");       print(m1_m3, 32, " => ");       print(m1_m3_h, 6, "\n");
    printf("m2_m3: ");       print(m2_m3, 32, " => ");       print(m2_m3_h, 6, "\n");
    printf("m1_m4: ");       print(m1_m4, 32, " => ");       print(m1_m4_h, 6, "\n");
    printf("m2_m4: ");       print(m2_m4, 32, " => ");       print(m2_m4_h, 6, "\n");
    printf("m1_m3_m5: ");    print(m1_m3_m5, 48, " => ");    print(m1_m3_m5_h, 6, "\n");
    printf("m2_m3_m5: ");    print(m2_m3_m5, 48, " => ");    print(m2_m3_m5_h, 6, "\n");
    printf("m1_m4_m5: ");    print(m1_m4_m5, 48, " => ");    print(m1_m4_m5_h, 6, "\n");
    printf("m2_m4_m5: ");    print(m2_m4_m5, 48, " => ");    print(m2_m4_m5_h, 6, "\n");
    printf("m1_m3_m6: ");    print(m1_m3_m6, 48, " => ");    print(m1_m3_m6_h, 6, "\n");
    printf("m2_m3_m6: ");    print(m2_m3_m6, 48, " => ");    print(m2_m3_m6_h, 6, "\n");
    printf("m1_m4_m6: ");    print(m1_m4_m6, 48, " => ");    print(m1_m4_m6_h, 6, "\n");
    printf("m2_m4_m6: ");    print(m2_m4_m6, 48, " => ");    print(m2_m4_m6_h, 6, "\n");
    printf("m1_m3_m5_m7: "); print(m1_m3_m5_m7, 64, " => "); print(m1_m3_m5_m7_h, 6, "\n");
    printf("m2_m3_m5_m7: "); print(m2_m3_m5_m7, 64, " => "); print(m2_m3_m5_m7_h, 6, "\n");
    printf("m1_m4_m5_m7: "); print(m1_m4_m5_m7, 64, " => "); print(m1_m4_m5_m7_h, 6, "\n");
    printf("m2_m4_m5_m7: "); print(m2_m4_m5_m7, 64, " => "); print(m2_m4_m5_m7_h, 6, "\n");
    printf("m1_m3_m6_m7: "); print(m1_m3_m6_m7, 64, " => "); print(m1_m3_m6_m7_h, 6, "\n");
    printf("m2_m3_m6_m7: "); print(m2_m3_m6_m7, 64, " => "); print(m2_m3_m6_m7_h, 6, "\n");
    printf("m1_m4_m6_m7: "); print(m1_m4_m6_m7, 64, " => "); print(m1_m4_m6_m7_h, 6, "\n");
    printf("m2_m4_m6_m7: "); print(m2_m4_m6_m7, 64, " => "); print(m2_m4_m6_m7_h, 6, "\n");
    printf("m1_m3_m5_m8: "); print(m1_m3_m5_m8, 64, " => "); print(m1_m3_m5_m8_h, 6, "\n");
    printf("m2_m3_m5_m8: "); print(m2_m3_m5_m8, 64, " => "); print(m2_m3_m5_m8_h, 6, "\n");
    printf("m1_m4_m5_m8: "); print(m1_m4_m5_m8, 64, " => "); print(m1_m4_m5_m8_h, 6, "\n");
    printf("m2_m4_m5_m8: "); print(m2_m4_m5_m8, 64, " => "); print(m2_m4_m5_m8_h, 6, "\n");
    printf("m1_m3_m6_m8: "); print(m1_m3_m6_m8, 64, " => "); print(m1_m3_m6_m8_h, 6, "\n");
    printf("m2_m3_m6_m8: "); print(m2_m3_m6_m8, 64, " => "); print(m2_m3_m6_m8_h, 6, "\n");
    printf("m1_m4_m6_m8: "); print(m1_m4_m6_m8, 64, " => "); print(m1_m4_m6_m8_h, 6, "\n");
    printf("m2_m4_m6_m8: "); print(m2_m4_m6_m8, 64, " => "); print(m2_m4_m6_m8_h, 6, "\n");

}

void static_test_unbalanced() {

    uint8_t h1[6], h2[6]; reset_h(h1, init_h); reset_h(h2, init_h);
    uint8_t m1[16] = {89, 51, 139, 247, 210, 219, 112, 91, 122, 212, 249, 54, 158, 38, 222, 253};
    uint8_t m2[32] = {137, 132, 167, 90, 18, 134, 97, 148, 87, 68, 100, 173, 160, 32, 47, 205, 153, 23, 105, 41, 134, 118, 17, 230, 58, 95, 159, 167, 140, 242, 195, 105};

    // Compute the hash value of the message
    iterated_tcz48_dm(m1, BLEN, h1);
    iterated_tcz48_dm(m2, BLEN*2, h2);

    // Print the message and the hash value
    printf("\n\n Static test unbalanced\n\n");
    printf("m1: "); print(m1, 16, " -> "); print(h1, 6, "\n");
    printf("m2: "); print(m2, 32, " -> "); print(h2, 6, "\n");

}