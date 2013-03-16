// This file is part of LibPkg.
// Copyright © 2004 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#ifndef _LIBPKG_MD5
#define _LIBPKG_MD5

#include <cstddef>
#include <string>
#include <iosfwd>
#include <stdint.h>

namespace pkg {

/** A class for producing MD5 message digests.
 * MD5 is a message digest algorithm published by Ron Rivest in 1992.
 * It takes a message of arbitrary length and produces a 128-bit digest.
 * A specification for the algorithm can be found in RFC 1321 and this
 * implementation follows that specification very closely.  It is not,
 * however, derived from the reference implementation given in appendix A
 */
class md5
{
public:
	/** A type to represent an unsigned 32-bit integer. */
	typedef uint32_t uint32;

	/** A type to represent an unsigned 64-bit integer. */
	typedef uint64_t uint64;
private:
	/** The size of the message digest state, in 32-bit words. */
	static const int state_size=4;

	/** The size of the input buffer, in 32-bit words. */
	static const int buffer_size=16;

	/** The message digest state.
	 * When the input data, padding and length word have been processed
	 * this array contains the message digest.
	 */
	uint32 state[state_size];

	/** The input buffer.
	 * Input must be processed in blocks of 16 32-bit words (512 bits).
	 * This array is used to buffer the input until that number of words
	 * have been accumulated.
	 */
	uint32 buffer[buffer_size];

	/** The byte count.
	 * This is the number of bytes which have been processed, or which
	 * are in the input buffer waiting to be processed.
	 */
	uint64 count;

	/** Process content of buffer.
	 * This function should be called when the input buffer is full.
	 * It incorporates the content of the buffer into the message digest
	 * state.  Once it has been called, the content of the buffer may
	 * be discarded.
	 */
	void process_buffer();
public:
	/** Construct MD5 object. */
	md5();

	/** Process data in memory.
	 * The specified number of bytes are transferred from memory to the
	 * input buffer.  Any complete blocks of 16 bytes are incorporated
	 * into the message digest state.
	 * @param data a pointer to the data
	 * @param length the length of the data in bytes
	 */
	void operator()(const void* data,size_t length);

	/** Process data from stream.
	 * Bytes are transferred from the stream to the input buffer until the
	 * end of file is reached.  Any complete blocks of 16 bytes are
	 * incorporated into the message digest state.
	 * @param in the input stream from which the data is to be read
	 */
	void operator()(std::istream& in);

	/** Flush buffer.
	 * The final value of the message digest is calculated by processing
	 * any bytes still remaining in the buffer, followed by between 1
	 * and 16 bytes of padding,
	 */
	void operator()();

	/** Read word from message digest.
	 * The digest is presented as an array of four 32-bit words.
	 * Although this function may be called at any time, the result is
	 * only meaningful after the buffer has been flushed.  Its effect is
	 * undefined if the index falls outside the range 0 to 3 inclusive.
	 * @param index the index of the word to be read
	 * @return a word from the message digest
	 */
	uint32 operator[](unsigned int index)
		{ return state[index]; }

	/** Convert message digest to hexadecimal string.
	 * The digest is presented low-order byte, high-order nybble first.
	 * Although this function may be called at any time, the result is
	 * only meaningful after the buffer has been flushed.
	 * @return the message digest, as a hexadecimal string
	 */
	operator std::string();
};

}; /* namespace pkg */

#endif
