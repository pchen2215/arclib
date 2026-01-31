#include <arclib/bitfield.h>
#include <arclib/verify.h>

using namespace arcl;

int main(int, char**) {
    try { // Test basic bitfield construction
        bitfield bf1;
        verify(bf1.size() == 0);

        bitfield bf2(1);
        verify(bf2.size() == 8);
        for (uint64 i = 0; i < bf2.size(); i++) { verify(bf2[i] == false); }

    } catch (...) { return EXIT_FAILURE; }
    
    try { // Test bit manipulation
        bitfield bf(1);

        bf[0] = true;
        bf[2] = true;
        bf[4] = true;
        verify(bf[0] == true);
        verify(bf[2] == true);
        verify(bf[4] == true);

        bf[0] = bf[2] = false;
        bf[1] = bf[3] = true;
        verify(bf[0] == false);
        verify(bf[1] == true);
        verify(bf[2] == false);
        verify(bf[3] == true);

        bitfield::bitref bit6 = bf[6];
        bitfield::bitref bit7 = bf[7];
        verify(bit6 == false);
        verify(bit7 == false);
        verify(bit6 == bit7);

        bit6 = true;
        verify(bit6 == true);
        verify(bf[6] == true);
        verify(bit6 != bit7);

        bit7 = !bit7;
        verify(bit7 == true);
        verify(bf[7] == true);
        verify(bit6 == bit7);

    } catch (...) { return EXIT_FAILURE; }

    try { // Test resizing functionality
        bitfield bf1;

        for (uint64 i = 0; i < 1024; i++) {
            bf1.resize(i);
            verify(bf1.size() == i * 8);
        }

        bitfield bf2(2);
        for (uint64 i = 0; i < 16; i++) { bf2[i] = true; }

        bf2.resize(1);
        for (uint64 i = 0; i < 8; i++) { verify(bf2[i] == true); }

        bf2.resize(2);
        for (uint64 i = 0; i < 8; i++) { verify(bf2[i] == true); }
        for (uint64 i = 8; i < 16; i++) { verify(bf2[i] == false); }

    } catch (...) { return EXIT_FAILURE; }
    
    try { // Test copy and move functionality

        /* TODO: write tests */

    } catch (...) { return EXIT_FAILURE; }

    // Passed all tests
    return EXIT_SUCCESS;
}
