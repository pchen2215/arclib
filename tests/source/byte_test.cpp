#include <arclib/byte.h>
#include <arclib/verify.h>

using namespace arcl;

int main(int, char**) {
    try {

        { // Construction and casting
            byte b1;
            byte b2(42);
            byte b3 = b2;

            verify((uint8)b1 == 0);
            verify((uint8)b2 == 42);
            verify((uint8)b3 == 42);
        }

        { // Bitwise operations
            byte b(0b11001100);

            b &= byte(0b11110000);
            verify((uint8)b == 0b11000000);

            b |= byte(0b00000011);
            verify((uint8)b == 0b11000011);

            b ^= byte(0b11111111);
            verify((uint8)b == 0b00111100);

            b <<= byte(2);
            verify((uint8)b == 0b11110000);

            b >>= byte(6);
            verify((uint8)b == 0b00000011);
        }

        { // Bit operations
            byte b(0);

            b[0] = true;
            b[2] = true;
            b[5] = true;
            b[7] = true;
            verify((uint8)b == 0b10100101);

            verify(b[0] == true);
            verify(b[1] == false);
            verify(b[2] == true);
            verify(b[3] == false);
            verify(b[4] == false);
            verify(b[5] == true);
            verify(b[6] == false);
            verify(b[7] == true);

            b[0] = b[7] = false;
            verify((uint8)b == 0b00100100);
        }

        { // Const correctness
            const byte cb(0b10000001);

            verify(cb[0] == true);
            verify(cb[7] == true);

            byte b(0b11111111);
            bitref r = b[0];
            const_bitref cr = r;
            verify(cr == true);
        }

        { // Comparisons
            byte a(0b01100001);
            byte b(0b01100001);
            byte c(0b11100010);

            verify(a == b);
            verify(a != c);
            verify(!(a == c));

            verify(a[0] == b[0]);
            verify(a[0] != b[7]);
            verify(a[0] != c[0]);
            verify(b[7] != c[7]);
        }

    } catch (...) { return EXIT_FAILURE; }
    return EXIT_SUCCESS;
}
