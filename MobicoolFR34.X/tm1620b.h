#ifndef TM1620B_H
#define	TM1620B_H

// Segment definitions for digits and most letters
extern const uint8_t hexdigits[];
#define ADD_DOT (0b00000010)
#define MINUS (0b10000000)
#define c_A (hexdigits[0xa])
#define c_b (hexdigits[0xb])
#define c_C (hexdigits[0xc])
#define c_d (hexdigits[0xd])
#define c_E (hexdigits[0xe])
#define c_F (hexdigits[0xf])
#define c_g (hexdigits[9]) // Same as "9"
#define c_h (0b11100001)
#define c_H (0b11101001)
#define c_i (0b00100000)
#define c_J (0b01111000)
// No K/k
#define c_L (0b01010001)
#define c_M (0b01101101) // Really upside down U but that's how M in MEd looks in original fw
#define c_N (c_M) // Nothing else we can do
#define c_o (0b11110000)
#define c_P (0b11001101)
// No Q/q
#define c_r (0b11000000)
#define c_S (hexdigits[5]) // Same as "5"
#define c_t (0b11010001)
#define c_U (0b01111001)
#define c_V (c_U) // Nothing else to do here
// No W/w, X/x, Y/y, Z/z

#define KEY_ONOFF (1 << 3)
#define KEY_SET (1 << 2)
#define KEY_PLUS (1 << 1)
#define KEY_MINUS (1 << 0)

void TM1620B_Update(uint8_t* buf);
uint8_t TM1620B_GetKeys(void);
void TM1620B_Init(void);
uint8_t FormatDigits(uint8_t* outbuf, int16_t inum);

#endif	/* TM1620B_H */
