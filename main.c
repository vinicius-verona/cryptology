#include <stdio.h>

#include "impl.h"

void static_test_t2();
void static_test_t4();
void static_test_unbalanced();
#define STATIC_TEST

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
    // collision(h, m1, m2);

    printf("\n************* MULTICOLLISION *************\n\n");
    // int samples_t1 = multicollision(1);
    // printf("Samples for t = 1: %d\n\n", samples_t1);

    // int samples_t4 = multicollision(4);
    // printf("Samples for t = 4: %d\n", samples_t4);

    printf("\n************* Expandable message *************\n\n");
    double r_unbalanced_collision  = unbalanced_collision(h, m1, m3, 32);
    // double r_expandable_message_t1 = expandable_message(1);
    // double r_expandable_message_t4 = expandable_message(4);

#endif
}

void static_test_t2() {
    printf(" Static test for t = 2\n");

    // Creates messages and hash using iterated_tcz48_dm
    uint8_t h[6];
    uint8_t m_1[16] = {217, 133, 119, 218, 148, 130, 50, 30, 253, 202, 110, 144, 2, 253, 112, 105};
    uint8_t m_2[16] = {140, 51, 196, 146, 245, 143, 236, 201, 117, 243, 60, 117, 241, 186, 194, 64};
    uint8_t m_3[16] = {243, 52, 202, 116, 34, 76, 107, 21, 160, 179, 97, 205, 107, 11, 109, 117};
    uint8_t m_4[16] = {65, 211, 141, 176, 10, 226, 248, 193, 97, 251, 15, 82, 3, 85, 91, 87};

    // for each of the 4 messages, print the message and hash
    for (int i = 0; i < 4; i++) {
        uint8_t* m = i == 0 ? m_1 : i == 1 ? m_2
                                : i == 2   ? m_3
                                           : m_4;
        iterated_tcz48_dm(m, 16, h);
        print(m, 16, "  -> ");
        print(h, 6, "\n");
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

    uint8_t m1_m3_h[6]; iterated_tcz48_dm(m1_m3, 32, m1_m3_h);
    uint8_t m2_m4_h[6]; iterated_tcz48_dm(m2_m4, 32, m2_m4_h);
    uint8_t m1_m4_h[6]; iterated_tcz48_dm(m1_m4, 32, m1_m4_h);
    uint8_t m2_m3_h[6]; iterated_tcz48_dm(m2_m3, 32, m2_m3_h);

    print(m1_m3, 32, " -> m1 concat m3 -> "); print(m1_m3_h, 6, "\n");
    print(m2_m4, 32, " -> m2 concat m4 -> "); print(m2_m4_h, 6, "\n");
    print(m1_m4, 32, " -> m1 concat m4 -> "); print(m1_m4_h, 6, "\n");
    print(m2_m3, 32, " -> m2 concat m3 -> "); print(m2_m3_h, 6, "\n");
}

void static_test_t4() {
    printf("\n\n Static test t = 4\n\n");

    // Creates messages and hash using iterated_tcz48_dm
    uint8_t h[6];
    uint8_t m_1[16] = {79, 81, 130, 178, 193, 114, 209, 78, 143, 220, 8, 207, 129, 128, 49, 148};
    uint8_t m_2[16] = {73, 24, 212, 31, 175, 155, 181, 238, 160, 251, 149, 209, 232, 47, 50, 199};
    uint8_t m_3[16] = {243, 173, 39, 242, 44, 112, 42, 135, 204, 184, 145, 240, 96, 237, 125, 10};
    uint8_t m_4[16] = {34, 224, 71, 55, 129, 228, 189, 142, 107, 211, 235, 52, 154, 170, 21, 140};
    uint8_t m_5[16] = {176, 38, 34, 94, 159, 73, 173, 129, 95, 82, 0, 249, 253, 36, 244, 179};
    uint8_t m_6[16] = {90, 130, 146, 29, 10, 205, 181, 47, 6, 64, 218, 87, 193, 148, 193, 48};
    uint8_t m_7[16] = {73, 203, 148, 38, 66, 174, 149, 44, 143, 157, 233, 179, 48, 64, 142, 175};
    uint8_t m_8[16] = {209, 156, 76, 208, 195, 47, 208, 210, 203, 107, 216, 211, 153, 38, 178, 73};

    // for each of the 4 messages, print the message and hash
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
    }

    printf("\n########################\n\n");

    // Create a code that combines m_i with m_j such that we have 8 i,j and j cannot be i+1 if i+1 is pair
    uint8_t m1_m3[32], m2_m4[32], m1_m4[32], m2_m3[32], m1_m5[32], m2_m6[32], m1_m6[32], m2_m5[32], m1_m7[32], m2_m8[32], m1_m8[32], m2_m7[32], m3_m5[32], m4_m6[32], m3_m6[32], m4_m5[32], m3_m7[32], m4_m8[32], m3_m8[32], m4_m7[32], m5_m7[32], m6_m8[32], m5_m8[32], m6_m7[32];

    // create a hash variable of size 6 for each of the combination and use iterated_tcz48_dm to hash the combination
    uint8_t m1_m3_h[6], m2_m4_h[6], m1_m4_h[6], m2_m3_h[6], m1_m5_h[6], m2_m6_h[6], m1_m6_h[6], m2_m5_h[6], m1_m7_h[6], m2_m8_h[6], m1_m8_h[6], m2_m7_h[6], m3_m5_h[6], m4_m6_h[6], m3_m6_h[6], m4_m5_h[6], m3_m7_h[6], m4_m8_h[6], m3_m8_h[6], m4_m7_h[6], m5_m7_h[6], m6_m8_h[6], m5_m8_h[6], m6_m7_h[6];

    for (int i = 0; i < 16; i++) {
        m1_m3[i] = m_1[i]; m1_m3[i + 16] = m_3[i];
        m2_m4[i] = m_2[i]; m2_m4[i + 16] = m_4[i];
        m1_m4[i] = m_1[i]; m1_m4[i + 16] = m_4[i];
        m2_m3[i] = m_2[i]; m2_m3[i + 16] = m_3[i];
        m1_m5[i] = m_1[i]; m1_m5[i + 16] = m_5[i];
        m2_m6[i] = m_2[i]; m2_m6[i + 16] = m_6[i];
        m1_m6[i] = m_1[i]; m1_m6[i + 16] = m_6[i];
        m2_m5[i] = m_2[i]; m2_m5[i + 16] = m_5[i];
        m1_m7[i] = m_1[i]; m1_m7[i + 16] = m_7[i];
        m2_m8[i] = m_2[i]; m2_m8[i + 16] = m_8[i];
        m1_m8[i] = m_1[i]; m1_m8[i + 16] = m_8[i];
        m2_m7[i] = m_2[i]; m2_m7[i + 16] = m_7[i];
        m3_m5[i] = m_3[i]; m3_m5[i + 16] = m_5[i];
        m4_m6[i] = m_4[i]; m4_m6[i + 16] = m_6[i];
        m3_m6[i] = m_3[i]; m3_m6[i + 16] = m_6[i];
        m4_m5[i] = m_4[i]; m4_m5[i + 16] = m_5[i];
        m3_m7[i] = m_3[i]; m3_m7[i + 16] = m_7[i];
        m4_m8[i] = m_4[i]; m4_m8[i + 16] = m_8[i];
        m3_m8[i] = m_3[i]; m3_m8[i + 16] = m_8[i];
        m4_m7[i] = m_4[i]; m4_m7[i + 16] = m_7[i];
        m5_m7[i] = m_5[i]; m5_m7[i + 16] = m_7[i];
        m6_m8[i] = m_6[i]; m6_m8[i + 16] = m_8[i];
        m5_m8[i] = m_5[i]; m5_m8[i + 16] = m_8[i];
        m6_m7[i] = m_6[i]; m6_m7[i + 16] = m_7[i];
    }

    iterated_tcz48_dm(m1_m3, 32, m1_m3_h);
    iterated_tcz48_dm(m2_m4, 32, m2_m4_h);
    iterated_tcz48_dm(m1_m4, 32, m1_m4_h);
    iterated_tcz48_dm(m2_m3, 32, m2_m3_h);
    iterated_tcz48_dm(m1_m5, 32, m1_m5_h);
    iterated_tcz48_dm(m2_m6, 32, m2_m6_h);
    iterated_tcz48_dm(m1_m6, 32, m1_m6_h);
    iterated_tcz48_dm(m2_m5, 32, m2_m5_h);
    iterated_tcz48_dm(m1_m7, 32, m1_m7_h);
    iterated_tcz48_dm(m2_m8, 32, m2_m8_h);
    iterated_tcz48_dm(m1_m8, 32, m1_m8_h);
    iterated_tcz48_dm(m2_m7, 32, m2_m7_h);
    iterated_tcz48_dm(m3_m5, 32, m3_m5_h);
    iterated_tcz48_dm(m4_m6, 32, m4_m6_h);
    iterated_tcz48_dm(m3_m6, 32, m3_m6_h);
    iterated_tcz48_dm(m4_m5, 32, m4_m5_h);
    iterated_tcz48_dm(m3_m7, 32, m3_m7_h);
    iterated_tcz48_dm(m4_m8, 32, m4_m8_h);
    iterated_tcz48_dm(m3_m8, 32, m3_m8_h);
    iterated_tcz48_dm(m4_m7, 32, m4_m7_h);
    iterated_tcz48_dm(m5_m7, 32, m5_m7_h);
    iterated_tcz48_dm(m6_m8, 32, m6_m8_h);
    iterated_tcz48_dm(m5_m8, 32, m5_m8_h);
    iterated_tcz48_dm(m6_m7, 32, m6_m7_h);

    // Print each of the hash value followed by the combination using the function print
    printf("m1_m3: "); print(m1_m3, 32, " -> "); print(m1_m3_h, 6, "\n");
    printf("m2_m4: "); print(m2_m4, 32, " -> "); print(m2_m4_h, 6, "\n");
    printf("m1_m4: "); print(m1_m4, 32, " -> "); print(m1_m4_h, 6, "\n");
    printf("m2_m3: "); print(m2_m3, 32, " -> "); print(m2_m3_h, 6, "\n");
    printf("m1_m5: "); print(m1_m5, 32, " -> "); print(m1_m5_h, 6, "\n");
    printf("m2_m6: "); print(m2_m6, 32, " -> "); print(m2_m6_h, 6, "\n");
    printf("m1_m6: "); print(m1_m6, 32, " -> "); print(m1_m6_h, 6, "\n");
    printf("m2_m5: "); print(m2_m5, 32, " -> "); print(m2_m5_h, 6, "\n");
    printf("m1_m7: "); print(m1_m7, 32, " -> "); print(m1_m7_h, 6, "\n");
    printf("m2_m8: "); print(m2_m8, 32, " -> "); print(m2_m8_h, 6, "\n");
    printf("m1_m8: "); print(m1_m8, 32, " -> "); print(m1_m8_h, 6, "\n");
    printf("m2_m7: "); print(m2_m7, 32, " -> "); print(m2_m7_h, 6, "\n");
    printf("m3_m5: "); print(m3_m5, 32, " -> "); print(m3_m5_h, 6, "\n");
    printf("m4_m6: "); print(m4_m6, 32, " -> "); print(m4_m6_h, 6, "\n");
    printf("m3_m6: "); print(m3_m6, 32, " -> "); print(m3_m6_h, 6, "\n");
    printf("m4_m5: "); print(m4_m5, 32, " -> "); print(m4_m5_h, 6, "\n");
    printf("m3_m7: "); print(m3_m7, 32, " -> "); print(m3_m7_h, 6, "\n");
    printf("m4_m8: "); print(m4_m8, 32, " -> "); print(m4_m8_h, 6, "\n");
    printf("m3_m8: "); print(m3_m8, 32, " -> "); print(m3_m8_h, 6, "\n");
    printf("m4_m7: "); print(m4_m7, 32, " -> "); print(m4_m7_h, 6, "\n");
    printf("m5_m7: "); print(m5_m7, 32, " -> "); print(m5_m7_h, 6, "\n");
    printf("m6_m8: "); print(m6_m8, 32, " -> "); print(m6_m8_h, 6, "\n");
    printf("m5_m8: "); print(m5_m8, 32, " -> "); print(m5_m8_h, 6, "\n");
    printf("m6_m7: "); print(m6_m7, 32, " -> "); print(m6_m7_h, 6, "\n");
}

void static_test_unbalanced() {

    uint8_t h1[6], h2[6];
    uint8_t m1[16] = {221, 168, 140, 69, 67, 73, 203, 240, 131, 149, 33, 243, 252, 220, 52, 20}; // DA8444CF892FFD314
    uint8_t m2[32] = {45, 74, 30, 206, 32, 219, 168, 157, 76, 2, 241, 153, 95, 47, 197, 58, 37, 61, 123, 183, 15, 46, 147, 132, 193, 54, 200, 108, 243, 64, 66, 239}; // 241C2DA940F952C3237B0298C3C6F44EF

    // Compute the hash value of the message
    iterated_tcz48_dm(m1, BLEN, h1);
    iterated_tcz48_dm(m2, BLEN*2, h2);

    // Print the message and the hash value
    printf("\n\n Static test unbalanced\n\n");
    printf("m1: "); print(m1, 16, " -> "); print(h1, 6, "\n");
    printf("m2: "); print(m2, 32, " -> "); print(h2, 6, "\n");

}