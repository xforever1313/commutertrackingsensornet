#include <stdint.h>
#include <string.h>

#include "sha1.h"

typedef uint32_t uint32;

static void blk_SHA1_Block(blk_SHA_CTX *ctx, const unsigned int *data)
{
 unsigned int A,B,C,D,E;
 unsigned int array[16];

 A = ctx->H[0];
 B = ctx->H[1];
 C = ctx->H[2];
 D = ctx->H[3];
 E = ctx->H[4];


 do { unsigned int TEMP = ( (*((unsigned char *)(data + 0) + 0) << 24) | (*((unsigned char *)(data + 0) + 1) << 16) | (*((unsigned char *)(data + 0) + 2) << 8) | (*((unsigned char *)(data + 0) + 3) << 0) ); ((array[(0)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((((C^D)&B)^D)) + (0x5a827999); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 1) + 0) << 24) | (*((unsigned char *)(data + 1) + 1) << 16) | (*((unsigned char *)(data + 1) + 2) << 8) | (*((unsigned char *)(data + 1) + 3) << 0) ); ((array[(1)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((((B^C)&A)^C)) + (0x5a827999); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 2) + 0) << 24) | (*((unsigned char *)(data + 2) + 1) << 16) | (*((unsigned char *)(data + 2) + 2) << 8) | (*((unsigned char *)(data + 2) + 3) << 0) ); ((array[(2)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((((A^B)&E)^B)) + (0x5a827999); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 3) + 0) << 24) | (*((unsigned char *)(data + 3) + 1) << 16) | (*((unsigned char *)(data + 3) + 2) << 8) | (*((unsigned char *)(data + 3) + 3) << 0) ); ((array[(3)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((((E^A)&D)^A)) + (0x5a827999); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 4) + 0) << 24) | (*((unsigned char *)(data + 4) + 1) << 16) | (*((unsigned char *)(data + 4) + 2) << 8) | (*((unsigned char *)(data + 4) + 3) << 0) ); ((array[(4)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((((D^E)&C)^E)) + (0x5a827999); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 5) + 0) << 24) | (*((unsigned char *)(data + 5) + 1) << 16) | (*((unsigned char *)(data + 5) + 2) << 8) | (*((unsigned char *)(data + 5) + 3) << 0) ); ((array[(5)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((((C^D)&B)^D)) + (0x5a827999); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 6) + 0) << 24) | (*((unsigned char *)(data + 6) + 1) << 16) | (*((unsigned char *)(data + 6) + 2) << 8) | (*((unsigned char *)(data + 6) + 3) << 0) ); ((array[(6)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((((B^C)&A)^C)) + (0x5a827999); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 7) + 0) << 24) | (*((unsigned char *)(data + 7) + 1) << 16) | (*((unsigned char *)(data + 7) + 2) << 8) | (*((unsigned char *)(data + 7) + 3) << 0) ); ((array[(7)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((((A^B)&E)^B)) + (0x5a827999); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 8) + 0) << 24) | (*((unsigned char *)(data + 8) + 1) << 16) | (*((unsigned char *)(data + 8) + 2) << 8) | (*((unsigned char *)(data + 8) + 3) << 0) ); ((array[(8)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((((E^A)&D)^A)) + (0x5a827999); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 9) + 0) << 24) | (*((unsigned char *)(data + 9) + 1) << 16) | (*((unsigned char *)(data + 9) + 2) << 8) | (*((unsigned char *)(data + 9) + 3) << 0) ); ((array[(9)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((((D^E)&C)^E)) + (0x5a827999); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 10) + 0) << 24) | (*((unsigned char *)(data + 10) + 1) << 16) | (*((unsigned char *)(data + 10) + 2) << 8) | (*((unsigned char *)(data + 10) + 3) << 0) ); ((array[(10)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((((C^D)&B)^D)) + (0x5a827999); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 11) + 0) << 24) | (*((unsigned char *)(data + 11) + 1) << 16) | (*((unsigned char *)(data + 11) + 2) << 8) | (*((unsigned char *)(data + 11) + 3) << 0) ); ((array[(11)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((((B^C)&A)^C)) + (0x5a827999); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 12) + 0) << 24) | (*((unsigned char *)(data + 12) + 1) << 16) | (*((unsigned char *)(data + 12) + 2) << 8) | (*((unsigned char *)(data + 12) + 3) << 0) ); ((array[(12)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((((A^B)&E)^B)) + (0x5a827999); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 13) + 0) << 24) | (*((unsigned char *)(data + 13) + 1) << 16) | (*((unsigned char *)(data + 13) + 2) << 8) | (*((unsigned char *)(data + 13) + 3) << 0) ); ((array[(13)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((((E^A)&D)^A)) + (0x5a827999); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 14) + 0) << 24) | (*((unsigned char *)(data + 14) + 1) << 16) | (*((unsigned char *)(data + 14) + 2) << 8) | (*((unsigned char *)(data + 14) + 3) << 0) ); ((array[(14)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((((D^E)&C)^E)) + (0x5a827999); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ( (*((unsigned char *)(data + 15) + 0) << 24) | (*((unsigned char *)(data + 15) + 1) << 16) | (*((unsigned char *)(data + 15) + 2) << 8) | (*((unsigned char *)(data + 15) + 3) << 0) ); ((array[(15)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((((C^D)&B)^D)) + (0x5a827999); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);


 do { unsigned int TEMP = ((((array[(16 +13)&15]) ^ (array[(16 +8)&15]) ^ (array[(16 +2)&15]) ^ (array[(16)&15])) << (1)) | (((array[(16 +13)&15]) ^ (array[(16 +8)&15]) ^ (array[(16 +2)&15]) ^ (array[(16)&15])) >> (32-(1)))); ((array[(16)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((((B^C)&A)^C)) + (0x5a827999); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(17 +13)&15]) ^ (array[(17 +8)&15]) ^ (array[(17 +2)&15]) ^ (array[(17)&15])) << (1)) | (((array[(17 +13)&15]) ^ (array[(17 +8)&15]) ^ (array[(17 +2)&15]) ^ (array[(17)&15])) >> (32-(1)))); ((array[(17)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((((A^B)&E)^B)) + (0x5a827999); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(18 +13)&15]) ^ (array[(18 +8)&15]) ^ (array[(18 +2)&15]) ^ (array[(18)&15])) << (1)) | (((array[(18 +13)&15]) ^ (array[(18 +8)&15]) ^ (array[(18 +2)&15]) ^ (array[(18)&15])) >> (32-(1)))); ((array[(18)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((((E^A)&D)^A)) + (0x5a827999); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(19 +13)&15]) ^ (array[(19 +8)&15]) ^ (array[(19 +2)&15]) ^ (array[(19)&15])) << (1)) | (((array[(19 +13)&15]) ^ (array[(19 +8)&15]) ^ (array[(19 +2)&15]) ^ (array[(19)&15])) >> (32-(1)))); ((array[(19)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((((D^E)&C)^E)) + (0x5a827999); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);


 do { unsigned int TEMP = ((((array[(20 +13)&15]) ^ (array[(20 +8)&15]) ^ (array[(20 +2)&15]) ^ (array[(20)&15])) << (1)) | (((array[(20 +13)&15]) ^ (array[(20 +8)&15]) ^ (array[(20 +2)&15]) ^ (array[(20)&15])) >> (32-(1)))); ((array[(20)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0x6ed9eba1); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(21 +13)&15]) ^ (array[(21 +8)&15]) ^ (array[(21 +2)&15]) ^ (array[(21)&15])) << (1)) | (((array[(21 +13)&15]) ^ (array[(21 +8)&15]) ^ (array[(21 +2)&15]) ^ (array[(21)&15])) >> (32-(1)))); ((array[(21)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0x6ed9eba1); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(22 +13)&15]) ^ (array[(22 +8)&15]) ^ (array[(22 +2)&15]) ^ (array[(22)&15])) << (1)) | (((array[(22 +13)&15]) ^ (array[(22 +8)&15]) ^ (array[(22 +2)&15]) ^ (array[(22)&15])) >> (32-(1)))); ((array[(22)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0x6ed9eba1); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(23 +13)&15]) ^ (array[(23 +8)&15]) ^ (array[(23 +2)&15]) ^ (array[(23)&15])) << (1)) | (((array[(23 +13)&15]) ^ (array[(23 +8)&15]) ^ (array[(23 +2)&15]) ^ (array[(23)&15])) >> (32-(1)))); ((array[(23)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0x6ed9eba1); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(24 +13)&15]) ^ (array[(24 +8)&15]) ^ (array[(24 +2)&15]) ^ (array[(24)&15])) << (1)) | (((array[(24 +13)&15]) ^ (array[(24 +8)&15]) ^ (array[(24 +2)&15]) ^ (array[(24)&15])) >> (32-(1)))); ((array[(24)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0x6ed9eba1); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(25 +13)&15]) ^ (array[(25 +8)&15]) ^ (array[(25 +2)&15]) ^ (array[(25)&15])) << (1)) | (((array[(25 +13)&15]) ^ (array[(25 +8)&15]) ^ (array[(25 +2)&15]) ^ (array[(25)&15])) >> (32-(1)))); ((array[(25)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0x6ed9eba1); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(26 +13)&15]) ^ (array[(26 +8)&15]) ^ (array[(26 +2)&15]) ^ (array[(26)&15])) << (1)) | (((array[(26 +13)&15]) ^ (array[(26 +8)&15]) ^ (array[(26 +2)&15]) ^ (array[(26)&15])) >> (32-(1)))); ((array[(26)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0x6ed9eba1); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(27 +13)&15]) ^ (array[(27 +8)&15]) ^ (array[(27 +2)&15]) ^ (array[(27)&15])) << (1)) | (((array[(27 +13)&15]) ^ (array[(27 +8)&15]) ^ (array[(27 +2)&15]) ^ (array[(27)&15])) >> (32-(1)))); ((array[(27)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0x6ed9eba1); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(28 +13)&15]) ^ (array[(28 +8)&15]) ^ (array[(28 +2)&15]) ^ (array[(28)&15])) << (1)) | (((array[(28 +13)&15]) ^ (array[(28 +8)&15]) ^ (array[(28 +2)&15]) ^ (array[(28)&15])) >> (32-(1)))); ((array[(28)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0x6ed9eba1); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(29 +13)&15]) ^ (array[(29 +8)&15]) ^ (array[(29 +2)&15]) ^ (array[(29)&15])) << (1)) | (((array[(29 +13)&15]) ^ (array[(29 +8)&15]) ^ (array[(29 +2)&15]) ^ (array[(29)&15])) >> (32-(1)))); ((array[(29)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0x6ed9eba1); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(30 +13)&15]) ^ (array[(30 +8)&15]) ^ (array[(30 +2)&15]) ^ (array[(30)&15])) << (1)) | (((array[(30 +13)&15]) ^ (array[(30 +8)&15]) ^ (array[(30 +2)&15]) ^ (array[(30)&15])) >> (32-(1)))); ((array[(30)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0x6ed9eba1); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(31 +13)&15]) ^ (array[(31 +8)&15]) ^ (array[(31 +2)&15]) ^ (array[(31)&15])) << (1)) | (((array[(31 +13)&15]) ^ (array[(31 +8)&15]) ^ (array[(31 +2)&15]) ^ (array[(31)&15])) >> (32-(1)))); ((array[(31)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0x6ed9eba1); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(32 +13)&15]) ^ (array[(32 +8)&15]) ^ (array[(32 +2)&15]) ^ (array[(32)&15])) << (1)) | (((array[(32 +13)&15]) ^ (array[(32 +8)&15]) ^ (array[(32 +2)&15]) ^ (array[(32)&15])) >> (32-(1)))); ((array[(32)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0x6ed9eba1); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(33 +13)&15]) ^ (array[(33 +8)&15]) ^ (array[(33 +2)&15]) ^ (array[(33)&15])) << (1)) | (((array[(33 +13)&15]) ^ (array[(33 +8)&15]) ^ (array[(33 +2)&15]) ^ (array[(33)&15])) >> (32-(1)))); ((array[(33)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0x6ed9eba1); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(34 +13)&15]) ^ (array[(34 +8)&15]) ^ (array[(34 +2)&15]) ^ (array[(34)&15])) << (1)) | (((array[(34 +13)&15]) ^ (array[(34 +8)&15]) ^ (array[(34 +2)&15]) ^ (array[(34)&15])) >> (32-(1)))); ((array[(34)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0x6ed9eba1); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(35 +13)&15]) ^ (array[(35 +8)&15]) ^ (array[(35 +2)&15]) ^ (array[(35)&15])) << (1)) | (((array[(35 +13)&15]) ^ (array[(35 +8)&15]) ^ (array[(35 +2)&15]) ^ (array[(35)&15])) >> (32-(1)))); ((array[(35)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0x6ed9eba1); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(36 +13)&15]) ^ (array[(36 +8)&15]) ^ (array[(36 +2)&15]) ^ (array[(36)&15])) << (1)) | (((array[(36 +13)&15]) ^ (array[(36 +8)&15]) ^ (array[(36 +2)&15]) ^ (array[(36)&15])) >> (32-(1)))); ((array[(36)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0x6ed9eba1); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(37 +13)&15]) ^ (array[(37 +8)&15]) ^ (array[(37 +2)&15]) ^ (array[(37)&15])) << (1)) | (((array[(37 +13)&15]) ^ (array[(37 +8)&15]) ^ (array[(37 +2)&15]) ^ (array[(37)&15])) >> (32-(1)))); ((array[(37)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0x6ed9eba1); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(38 +13)&15]) ^ (array[(38 +8)&15]) ^ (array[(38 +2)&15]) ^ (array[(38)&15])) << (1)) | (((array[(38 +13)&15]) ^ (array[(38 +8)&15]) ^ (array[(38 +2)&15]) ^ (array[(38)&15])) >> (32-(1)))); ((array[(38)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0x6ed9eba1); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(39 +13)&15]) ^ (array[(39 +8)&15]) ^ (array[(39 +2)&15]) ^ (array[(39)&15])) << (1)) | (((array[(39 +13)&15]) ^ (array[(39 +8)&15]) ^ (array[(39 +2)&15]) ^ (array[(39)&15])) >> (32-(1)))); ((array[(39)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0x6ed9eba1); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);


 do { unsigned int TEMP = ((((array[(40 +13)&15]) ^ (array[(40 +8)&15]) ^ (array[(40 +2)&15]) ^ (array[(40)&15])) << (1)) | (((array[(40 +13)&15]) ^ (array[(40 +8)&15]) ^ (array[(40 +2)&15]) ^ (array[(40)&15])) >> (32-(1)))); ((array[(40)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(41 +13)&15]) ^ (array[(41 +8)&15]) ^ (array[(41 +2)&15]) ^ (array[(41)&15])) << (1)) | (((array[(41 +13)&15]) ^ (array[(41 +8)&15]) ^ (array[(41 +2)&15]) ^ (array[(41)&15])) >> (32-(1)))); ((array[(41)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(42 +13)&15]) ^ (array[(42 +8)&15]) ^ (array[(42 +2)&15]) ^ (array[(42)&15])) << (1)) | (((array[(42 +13)&15]) ^ (array[(42 +8)&15]) ^ (array[(42 +2)&15]) ^ (array[(42)&15])) >> (32-(1)))); ((array[(42)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(43 +13)&15]) ^ (array[(43 +8)&15]) ^ (array[(43 +2)&15]) ^ (array[(43)&15])) << (1)) | (((array[(43 +13)&15]) ^ (array[(43 +8)&15]) ^ (array[(43 +2)&15]) ^ (array[(43)&15])) >> (32-(1)))); ((array[(43)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(44 +13)&15]) ^ (array[(44 +8)&15]) ^ (array[(44 +2)&15]) ^ (array[(44)&15])) << (1)) | (((array[(44 +13)&15]) ^ (array[(44 +8)&15]) ^ (array[(44 +2)&15]) ^ (array[(44)&15])) >> (32-(1)))); ((array[(44)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(45 +13)&15]) ^ (array[(45 +8)&15]) ^ (array[(45 +2)&15]) ^ (array[(45)&15])) << (1)) | (((array[(45 +13)&15]) ^ (array[(45 +8)&15]) ^ (array[(45 +2)&15]) ^ (array[(45)&15])) >> (32-(1)))); ((array[(45)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(46 +13)&15]) ^ (array[(46 +8)&15]) ^ (array[(46 +2)&15]) ^ (array[(46)&15])) << (1)) | (((array[(46 +13)&15]) ^ (array[(46 +8)&15]) ^ (array[(46 +2)&15]) ^ (array[(46)&15])) >> (32-(1)))); ((array[(46)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(47 +13)&15]) ^ (array[(47 +8)&15]) ^ (array[(47 +2)&15]) ^ (array[(47)&15])) << (1)) | (((array[(47 +13)&15]) ^ (array[(47 +8)&15]) ^ (array[(47 +2)&15]) ^ (array[(47)&15])) >> (32-(1)))); ((array[(47)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(48 +13)&15]) ^ (array[(48 +8)&15]) ^ (array[(48 +2)&15]) ^ (array[(48)&15])) << (1)) | (((array[(48 +13)&15]) ^ (array[(48 +8)&15]) ^ (array[(48 +2)&15]) ^ (array[(48)&15])) >> (32-(1)))); ((array[(48)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(49 +13)&15]) ^ (array[(49 +8)&15]) ^ (array[(49 +2)&15]) ^ (array[(49)&15])) << (1)) | (((array[(49 +13)&15]) ^ (array[(49 +8)&15]) ^ (array[(49 +2)&15]) ^ (array[(49)&15])) >> (32-(1)))); ((array[(49)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(50 +13)&15]) ^ (array[(50 +8)&15]) ^ (array[(50 +2)&15]) ^ (array[(50)&15])) << (1)) | (((array[(50 +13)&15]) ^ (array[(50 +8)&15]) ^ (array[(50 +2)&15]) ^ (array[(50)&15])) >> (32-(1)))); ((array[(50)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(51 +13)&15]) ^ (array[(51 +8)&15]) ^ (array[(51 +2)&15]) ^ (array[(51)&15])) << (1)) | (((array[(51 +13)&15]) ^ (array[(51 +8)&15]) ^ (array[(51 +2)&15]) ^ (array[(51)&15])) >> (32-(1)))); ((array[(51)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(52 +13)&15]) ^ (array[(52 +8)&15]) ^ (array[(52 +2)&15]) ^ (array[(52)&15])) << (1)) | (((array[(52 +13)&15]) ^ (array[(52 +8)&15]) ^ (array[(52 +2)&15]) ^ (array[(52)&15])) >> (32-(1)))); ((array[(52)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(53 +13)&15]) ^ (array[(53 +8)&15]) ^ (array[(53 +2)&15]) ^ (array[(53)&15])) << (1)) | (((array[(53 +13)&15]) ^ (array[(53 +8)&15]) ^ (array[(53 +2)&15]) ^ (array[(53)&15])) >> (32-(1)))); ((array[(53)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(54 +13)&15]) ^ (array[(54 +8)&15]) ^ (array[(54 +2)&15]) ^ (array[(54)&15])) << (1)) | (((array[(54 +13)&15]) ^ (array[(54 +8)&15]) ^ (array[(54 +2)&15]) ^ (array[(54)&15])) >> (32-(1)))); ((array[(54)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(55 +13)&15]) ^ (array[(55 +8)&15]) ^ (array[(55 +2)&15]) ^ (array[(55)&15])) << (1)) | (((array[(55 +13)&15]) ^ (array[(55 +8)&15]) ^ (array[(55 +2)&15]) ^ (array[(55)&15])) >> (32-(1)))); ((array[(55)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + (((B&C)+(D&(B^C)))) + (0x8f1bbcdc); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(56 +13)&15]) ^ (array[(56 +8)&15]) ^ (array[(56 +2)&15]) ^ (array[(56)&15])) << (1)) | (((array[(56 +13)&15]) ^ (array[(56 +8)&15]) ^ (array[(56 +2)&15]) ^ (array[(56)&15])) >> (32-(1)))); ((array[(56)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + (((A&B)+(C&(A^B)))) + (0x8f1bbcdc); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(57 +13)&15]) ^ (array[(57 +8)&15]) ^ (array[(57 +2)&15]) ^ (array[(57)&15])) << (1)) | (((array[(57 +13)&15]) ^ (array[(57 +8)&15]) ^ (array[(57 +2)&15]) ^ (array[(57)&15])) >> (32-(1)))); ((array[(57)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + (((E&A)+(B&(E^A)))) + (0x8f1bbcdc); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(58 +13)&15]) ^ (array[(58 +8)&15]) ^ (array[(58 +2)&15]) ^ (array[(58)&15])) << (1)) | (((array[(58 +13)&15]) ^ (array[(58 +8)&15]) ^ (array[(58 +2)&15]) ^ (array[(58)&15])) >> (32-(1)))); ((array[(58)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + (((D&E)+(A&(D^E)))) + (0x8f1bbcdc); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(59 +13)&15]) ^ (array[(59 +8)&15]) ^ (array[(59 +2)&15]) ^ (array[(59)&15])) << (1)) | (((array[(59 +13)&15]) ^ (array[(59 +8)&15]) ^ (array[(59 +2)&15]) ^ (array[(59)&15])) >> (32-(1)))); ((array[(59)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + (((C&D)+(E&(C^D)))) + (0x8f1bbcdc); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);


 do { unsigned int TEMP = ((((array[(60 +13)&15]) ^ (array[(60 +8)&15]) ^ (array[(60 +2)&15]) ^ (array[(60)&15])) << (1)) | (((array[(60 +13)&15]) ^ (array[(60 +8)&15]) ^ (array[(60 +2)&15]) ^ (array[(60)&15])) >> (32-(1)))); ((array[(60)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0xca62c1d6); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(61 +13)&15]) ^ (array[(61 +8)&15]) ^ (array[(61 +2)&15]) ^ (array[(61)&15])) << (1)) | (((array[(61 +13)&15]) ^ (array[(61 +8)&15]) ^ (array[(61 +2)&15]) ^ (array[(61)&15])) >> (32-(1)))); ((array[(61)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0xca62c1d6); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(62 +13)&15]) ^ (array[(62 +8)&15]) ^ (array[(62 +2)&15]) ^ (array[(62)&15])) << (1)) | (((array[(62 +13)&15]) ^ (array[(62 +8)&15]) ^ (array[(62 +2)&15]) ^ (array[(62)&15])) >> (32-(1)))); ((array[(62)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0xca62c1d6); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(63 +13)&15]) ^ (array[(63 +8)&15]) ^ (array[(63 +2)&15]) ^ (array[(63)&15])) << (1)) | (((array[(63 +13)&15]) ^ (array[(63 +8)&15]) ^ (array[(63 +2)&15]) ^ (array[(63)&15])) >> (32-(1)))); ((array[(63)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0xca62c1d6); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(64 +13)&15]) ^ (array[(64 +8)&15]) ^ (array[(64 +2)&15]) ^ (array[(64)&15])) << (1)) | (((array[(64 +13)&15]) ^ (array[(64 +8)&15]) ^ (array[(64 +2)&15]) ^ (array[(64)&15])) >> (32-(1)))); ((array[(64)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0xca62c1d6); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(65 +13)&15]) ^ (array[(65 +8)&15]) ^ (array[(65 +2)&15]) ^ (array[(65)&15])) << (1)) | (((array[(65 +13)&15]) ^ (array[(65 +8)&15]) ^ (array[(65 +2)&15]) ^ (array[(65)&15])) >> (32-(1)))); ((array[(65)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0xca62c1d6); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(66 +13)&15]) ^ (array[(66 +8)&15]) ^ (array[(66 +2)&15]) ^ (array[(66)&15])) << (1)) | (((array[(66 +13)&15]) ^ (array[(66 +8)&15]) ^ (array[(66 +2)&15]) ^ (array[(66)&15])) >> (32-(1)))); ((array[(66)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0xca62c1d6); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(67 +13)&15]) ^ (array[(67 +8)&15]) ^ (array[(67 +2)&15]) ^ (array[(67)&15])) << (1)) | (((array[(67 +13)&15]) ^ (array[(67 +8)&15]) ^ (array[(67 +2)&15]) ^ (array[(67)&15])) >> (32-(1)))); ((array[(67)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0xca62c1d6); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(68 +13)&15]) ^ (array[(68 +8)&15]) ^ (array[(68 +2)&15]) ^ (array[(68)&15])) << (1)) | (((array[(68 +13)&15]) ^ (array[(68 +8)&15]) ^ (array[(68 +2)&15]) ^ (array[(68)&15])) >> (32-(1)))); ((array[(68)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0xca62c1d6); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(69 +13)&15]) ^ (array[(69 +8)&15]) ^ (array[(69 +2)&15]) ^ (array[(69)&15])) << (1)) | (((array[(69 +13)&15]) ^ (array[(69 +8)&15]) ^ (array[(69 +2)&15]) ^ (array[(69)&15])) >> (32-(1)))); ((array[(69)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0xca62c1d6); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(70 +13)&15]) ^ (array[(70 +8)&15]) ^ (array[(70 +2)&15]) ^ (array[(70)&15])) << (1)) | (((array[(70 +13)&15]) ^ (array[(70 +8)&15]) ^ (array[(70 +2)&15]) ^ (array[(70)&15])) >> (32-(1)))); ((array[(70)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0xca62c1d6); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(71 +13)&15]) ^ (array[(71 +8)&15]) ^ (array[(71 +2)&15]) ^ (array[(71)&15])) << (1)) | (((array[(71 +13)&15]) ^ (array[(71 +8)&15]) ^ (array[(71 +2)&15]) ^ (array[(71)&15])) >> (32-(1)))); ((array[(71)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0xca62c1d6); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(72 +13)&15]) ^ (array[(72 +8)&15]) ^ (array[(72 +2)&15]) ^ (array[(72)&15])) << (1)) | (((array[(72 +13)&15]) ^ (array[(72 +8)&15]) ^ (array[(72 +2)&15]) ^ (array[(72)&15])) >> (32-(1)))); ((array[(72)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0xca62c1d6); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(73 +13)&15]) ^ (array[(73 +8)&15]) ^ (array[(73 +2)&15]) ^ (array[(73)&15])) << (1)) | (((array[(73 +13)&15]) ^ (array[(73 +8)&15]) ^ (array[(73 +2)&15]) ^ (array[(73)&15])) >> (32-(1)))); ((array[(73)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0xca62c1d6); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(74 +13)&15]) ^ (array[(74 +8)&15]) ^ (array[(74 +2)&15]) ^ (array[(74)&15])) << (1)) | (((array[(74 +13)&15]) ^ (array[(74 +8)&15]) ^ (array[(74 +2)&15]) ^ (array[(74)&15])) >> (32-(1)))); ((array[(74)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0xca62c1d6); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(75 +13)&15]) ^ (array[(75 +8)&15]) ^ (array[(75 +2)&15]) ^ (array[(75)&15])) << (1)) | (((array[(75 +13)&15]) ^ (array[(75 +8)&15]) ^ (array[(75 +2)&15]) ^ (array[(75)&15])) >> (32-(1)))); ((array[(75)&15]) = (TEMP)); E += TEMP + (((A) << (5)) | ((A) >> (32-(5)))) + ((B^C^D)) + (0xca62c1d6); B = (((B) << (32-(2))) | ((B) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(76 +13)&15]) ^ (array[(76 +8)&15]) ^ (array[(76 +2)&15]) ^ (array[(76)&15])) << (1)) | (((array[(76 +13)&15]) ^ (array[(76 +8)&15]) ^ (array[(76 +2)&15]) ^ (array[(76)&15])) >> (32-(1)))); ((array[(76)&15]) = (TEMP)); D += TEMP + (((E) << (5)) | ((E) >> (32-(5)))) + ((A^B^C)) + (0xca62c1d6); A = (((A) << (32-(2))) | ((A) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(77 +13)&15]) ^ (array[(77 +8)&15]) ^ (array[(77 +2)&15]) ^ (array[(77)&15])) << (1)) | (((array[(77 +13)&15]) ^ (array[(77 +8)&15]) ^ (array[(77 +2)&15]) ^ (array[(77)&15])) >> (32-(1)))); ((array[(77)&15]) = (TEMP)); C += TEMP + (((D) << (5)) | ((D) >> (32-(5)))) + ((E^A^B)) + (0xca62c1d6); E = (((E) << (32-(2))) | ((E) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(78 +13)&15]) ^ (array[(78 +8)&15]) ^ (array[(78 +2)&15]) ^ (array[(78)&15])) << (1)) | (((array[(78 +13)&15]) ^ (array[(78 +8)&15]) ^ (array[(78 +2)&15]) ^ (array[(78)&15])) >> (32-(1)))); ((array[(78)&15]) = (TEMP)); B += TEMP + (((C) << (5)) | ((C) >> (32-(5)))) + ((D^E^A)) + (0xca62c1d6); D = (((D) << (32-(2))) | ((D) >> (2))); } while (0);
 do { unsigned int TEMP = ((((array[(79 +13)&15]) ^ (array[(79 +8)&15]) ^ (array[(79 +2)&15]) ^ (array[(79)&15])) << (1)) | (((array[(79 +13)&15]) ^ (array[(79 +8)&15]) ^ (array[(79 +2)&15]) ^ (array[(79)&15])) >> (32-(1)))); ((array[(79)&15]) = (TEMP)); A += TEMP + (((B) << (5)) | ((B) >> (32-(5)))) + ((C^D^E)) + (0xca62c1d6); C = (((C) << (32-(2))) | ((C) >> (2))); } while (0);

 ctx->H[0] += A;
 ctx->H[1] += B;
 ctx->H[2] += C;
 ctx->H[3] += D;
 ctx->H[4] += E;
}

void blk_SHA1_Init(blk_SHA_CTX *ctx)
{
 ctx->size = 0;


 ctx->H[0] = 0x67452301;
 ctx->H[1] = 0xefcdab89;
 ctx->H[2] = 0x98badcfe;
 ctx->H[3] = 0x10325476;
 ctx->H[4] = 0xc3d2e1f0;
}

void blk_SHA1_Update(blk_SHA_CTX *ctx, const void *data, unsigned long len)
{
 unsigned int lenW = ctx->size & 63;

 ctx->size += len;


 if (lenW) {
  unsigned int left = 64 - lenW;
  if (len < left)
   left = len;
  memcpy(lenW + (char *)ctx->W, data, left);
  lenW = (lenW + left) & 63;
  len -= left;
  data = ((const char *)data + left);
  if (lenW)
   return;
  blk_SHA1_Block(ctx, ctx->W);
 }
 while (len >= 64) {
  blk_SHA1_Block(ctx, data);
  data = ((const char *)data + 64);
  len -= 64;
 }
 if (len)
  memcpy(ctx->W, data, len);
}

void blk_SHA1_Final(unsigned char hashout[20], blk_SHA_CTX *ctx)
{
 static const unsigned char pad[64] = { 0x80 };
 unsigned int padlen[2];
 int i;


 padlen[0] = ((((uint32)((uint32_t)(ctx->size >> 29)) & 0xff000000) >> 24) | (((uint32)((uint32_t)(ctx->size >> 29)) & 0x00ff0000) >> 8) | (((uint32)((uint32_t)(ctx->size >> 29)) & 0x0000ff00) << 8) | (((uint32)((uint32_t)(ctx->size >> 29)) & 0x000000ff) << 24));
 padlen[1] = ((((uint32)((uint32_t)(ctx->size << 3)) & 0xff000000) >> 24) | (((uint32)((uint32_t)(ctx->size << 3)) & 0x00ff0000) >> 8) | (((uint32)((uint32_t)(ctx->size << 3)) & 0x0000ff00) << 8) | (((uint32)((uint32_t)(ctx->size << 3)) & 0x000000ff) << 24));

 i = ctx->size & 63;
 blk_SHA1_Update(ctx, pad, 1+ (63 & (55 - i)));
 blk_SHA1_Update(ctx, padlen, 8);


 for (i = 0; i < 5; i++)
  do { unsigned int __v = (ctx->H[i]); *((unsigned char *)(hashout + i*4) + 0) = __v >> 24; *((unsigned char *)(hashout + i*4) + 1) = __v >> 16; *((unsigned char *)(hashout + i*4) + 2) = __v >> 8; *((unsigned char *)(hashout + i*4) + 3) = __v >> 0; } while (0);
}
