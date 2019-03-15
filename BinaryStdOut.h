#pragma once
#include<iostream>
#include<fstream>
#include<exception>
#include<string>
using namespace std;
using byte = unsigned char;
/**
*  <i>Binary standard output</i>. This class provides methods for converting
*  primtive type variables ({@code boolean}, {@code byte}, {@code char},
*  {@code int}, {@code long}, {@code float}, and {@code double})
*  to sequences of bits and writing them to standard output.
*  Uses big-endian (most-significant byte first).
*  <p>
*  The client must {@code flush()} the output stream when finished writing bits.
*  <p>
*  The client should not intermix calls to {@code BinaryStdOut} with calls
*  to {@code StdOut} or {@code System.out}; otherwise unexpected behavior
*  will result.
*
*  @author Robert Sedgewick
*  @author Kevin Wayne
*/
class BinaryStdOut {
private:
    ofstream out;  // output stream (standard output)
    byte buffer;                // 8-bit buffer of bits to write
    int n;                     // number of bits remaining in buffer
    bool isInitialized=false;     // has BinaryStdOut been called for first time?

                            // initialize BinaryStdOut
    void initialize() {
        out.clear();
        buffer = 0;
        n = 0;
        isInitialized = true;
    }
    /**
    * Writes the specified bit to standard output.
    */
    void writeBit(bool bit) {
        if (!isInitialized) initialize();
        // add bit to buffer
        buffer <<= 1;
        if (bit) buffer |= 1;
        // if buffer is full (8 bits), write out as a single byte
        n++;
        if (n == 8) clearBuffer();
    }

    /**
    * Writes the 8-bit byte to standard output.
    */
    void writeByte(byte x) {

        if (!isInitialized) initialize();
        // optimized if byte-aligned
        if (n == 0) {
            //use this to show in binary mode
            /*
            for (int i = 0; i < 8; i++)
                cout << ((x >> (8 - i - 1)) & 1);*/
            out << x;
            return;
        }
        // otherwise write one bit at a time
        for (int i = 0; i < 8; i++) {
            bool bit = ((x >> (8 - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }
public:

    // write out any remaining bits in buffer to standard output, padding with 0s
    void clearBuffer() {
        if (!isInitialized) initialize();
        if (n == 0) return;
        if (n > 0) buffer <<= (8 - n);
        for (int i = 0; i < 8; i++)
            cout << ((buffer >> (8 - i - 1)) & 1);
        out << buffer;
        n = 0;
        buffer = 0;
    }
    // initialize BinaryStdOut
    BinaryStdOut()
    {
    }
    /**
    * Flushes standard output, padding 0s if number of bits written so far
    * is not a multiple of 8.
    */
    void flush() {
        clearBuffer();
        out.flush();
    }

    /**
    open the file
    */
    void open(string s) {
        out.open(s, ios::out | ios::binary);
    }


    /**
    * Flushes and closes standard output. Once standard output is closed, you can no
    * longer write bits to it.
    */
    void close() {
        flush();
        out.close();
        isInitialized = false;
    }


    /**
    * Writes the specified bit to standard output.
    * @param x the {@code boolean} to write.
    */
    void write(bool x) {
        writeBit(x);
    }

    /**
    * Writes the 8-bit byte to standard output.
    * @param x the {@code byte} to write.
    */
    void write(byte x) {
        writeByte(x & 0xff);
    }

    /**
    * Writes the 32-bit int to standard output.
    * @param x the {@code int} to write.
    */
    void write(int x) {
        writeByte((x >>  24) & 0xff);
        writeByte((x >>  16) & 0xff);
        writeByte((x >>   8) & 0xff);
        writeByte((x >>   0) & 0xff);
    }

    /**
    * Writes the r-bit int to standard output.
    * @param x the {@code int} to write.
    * @param r the number of relevant bits in the char.
    * @throws IllegalArgumentException if {@code r} is not between 1 and 32.
    * @throws IllegalArgumentException if {@code x} is not between 0 and 2<sup>r</sup> - 1.
    */
    void write(int x, int r) {
        if (r == 32) {
            write(x);
            return;
        }
        for (int i = 0; i < r; i++) {
            bool bit = ((x >> (r - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }

    /**
    * Writes the 8-bit char to standard output.
    * @param x the {@code char} to write.
    * @throws IllegalArgumentException if {@code x} is not betwen 0 and 255.
    */
    void write(char x) {
        writeByte(x);
    }

    /**
    * Writes the r-bit char to standard output.
    * @param x the {@code char} to write.
    * @param r the number of relevant bits in the char.
    * @throws IllegalArgumentException if {@code r} is not between 1 and 16.
    * @throws IllegalArgumentException if {@code x} is not between 0 and 2<sup>r</sup> - 1.
    */
    void write(char x, int r) {
        if (r == 8) {
            write(x);
            return;
        }
        for (int i = 0; i < r; i++) {
            bool bit = ((x >> (r - i - 1)) & 1) == 1;
            writeBit(bit);
        }
    }

    /**
    * Writes the string of 8-bit characters to standard output.
    * @param s the {@code String} to write.
    * @throws IllegalArgumentException if any character in the string is not
    * between 0 and 255.
    */
    void write(string s) {
        for (int i = 0; i < s.length(); i++)
            write(s[i]);
    }

    /**
    * Writes the string of r-bit characters to standard output.
    * @param s the {@code String} to write.
    * @param r the number of relevants bits in each character.
    * @throws IllegalArgumentException if r is not between 1 and 16.
    * @throws IllegalArgumentException if any character in the string is not
    * between 0 and 2<sup>r</sup> - 1.
    */
    void write(string s, int r) {
        for (int i = 0; i < s.length(); i++)
            write(s[i], r);
    }

};

/******************************************************************************
*  Copyright 2002-2016, Robert Sedgewick and Kevin Wayne.
*
*  This file is part of algs4.jar, which accompanies the textbook
*
*      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
*      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
*      http://algs4.cs.princeton.edu
*
*
*  algs4.jar is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  algs4.jar is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
******************************************************************************/
