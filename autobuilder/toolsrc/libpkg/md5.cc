// This file is part of LibPkg.
// Copyright © 2004 Graham Shaw.            
// Distribution and use are subject to the GNU Lesser General Public License,
// a copy of which may be found in the file !LibPkg.Copyright.

#include <algorithm>
#include <istream>

#include "libpkg/md5.h"

using namespace std;

namespace {

typedef pkg::md5::uint32 uint32;

/** Rotate left circular.
 * @param x the 32-bit value to be rotated
 * @param n the number of bits by which to rotate
 * @return the rotated value
 */
inline uint32 rlc(uint32 x,int n)
{
	return (x<<n)|(x>>(32-n));
}

/** The MD5 auxiliary function F.
 * For each bit position return y if x is true, z if x is false.
 * @return bitwise x?y:z
 */
inline uint32 F(uint32 x,uint32 y,uint32 z)
{
	return (x&y)|(z&~x);
}

/** The MD5 auxiliary function G.
 * For each bit position return x if z is true, y if z is false.
 * @return bitwise z?x:y
 */
inline uint32 G(uint32 x,uint32 y,uint32 z)
{
	return (x&z)|(y&~z);
}

/** The MD5 auxiliary function H.
 * For each bit position return the parity of x, y and z.
 * @return bitwise x^y^z
 */
inline uint32 H(uint32 x,uint32 y,uint32 z)
{
	return x^y^z;
}

/** The MD5 auxiliary function I.
 * For each bit position return !y if x is true, !y^z if x is false.
 * @return bitwise !(x?y:y^z)
 */
inline uint32 I(uint32 x,uint32 y,uint32 z)
{
	return y^(x|~z);
}

/** Perform one of the sixteen steps in the first round of MD5.
 * @param a register a (modified)
 * @param b register b
 * @param c register c
 * @param d register d
 * @param X a byte from the message
 * @param s the rotation distance
 * @param T a value from the sine table
 */
inline void r1step(uint32& a,uint32 b,uint32 c,uint32 d,int X,int s,int T)
{
	a=b+rlc(a+F(b,c,d)+X+T,s);
}

/** Perform one of the sixteen steps in the second round of MD5.
 * @param a register a (modified)
 * @param b register b
 * @param c register c
 * @param d register d
 * @param X a byte from the message
 * @param s the rotation distance
 * @param T a value from the sine table
 */
inline void r2step(uint32& a,uint32 b,uint32 c,uint32 d,int X,int s,int T)
{
	a=b+rlc(a+G(b,c,d)+X+T,s);
}

/** Perform one of the sixteen steps in the third round of MD5.
 * @param a register a (modified)
 * @param b register b
 * @param c register c
 * @param d register d
 * @param X a byte from the message
 * @param s the rotation distance
 * @param T a value from the sine table
 */
inline void r3step(uint32& a,uint32 b,uint32 c,uint32 d,int X,int s,int T)
{
	a=b+rlc(a+H(b,c,d)+X+T,s);
}

/** Perform one of the sixteen steps in the fourth round of MD5.
 * @param a register a (modified)
 * @param b register b
 * @param c register c
 * @param d register d
 * @param X a byte from the message
 * @param s the rotation distance
 * @param T a value from the sine table
 */
inline void r4step(uint32& a,uint32 b,uint32 c,uint32 d,int X,int s,int T)
{
	a=b+rlc(a+I(b,c,d)+X+T,s);
}

}; /* anonymous namespace */

namespace pkg {

md5::md5():
	count(0)
{
	// Initialise state.
	state[0]=0x67452301;
	state[1]=0xefcdab89;
	state[2]=0x98badcfe;
	state[3]=0x10325476;

	// Initialise buffer.
	// (This is necessary in case a uint32 is longer than 32 bits.)
	fill(buffer,buffer+buffer_size,0);
}

void md5::process_buffer()
{
	// Make copy of state.
	uint32 A=state[0];
	uint32 B=state[1];
	uint32 C=state[2];
	uint32 D=state[3];

	// Round 1.
	r1step(A,B,C,D,buffer[ 0], 7,0xd76aa478);
	r1step(D,A,B,C,buffer[ 1],12,0xe8c7b756);
	r1step(C,D,A,B,buffer[ 2],17,0x242070db);
	r1step(B,C,D,A,buffer[ 3],22,0xc1bdceee);
	r1step(A,B,C,D,buffer[ 4], 7,0xf57c0faf);
	r1step(D,A,B,C,buffer[ 5],12,0x4787c62a);
	r1step(C,D,A,B,buffer[ 6],17,0xa8304613);
	r1step(B,C,D,A,buffer[ 7],22,0xfd469501);
	r1step(A,B,C,D,buffer[ 8], 7,0x698098d8);
	r1step(D,A,B,C,buffer[ 9],12,0x8b44f7af);
	r1step(C,D,A,B,buffer[10],17,0xffff5bb1);
	r1step(B,C,D,A,buffer[11],22,0x895cd7be);
	r1step(A,B,C,D,buffer[12], 7,0x6b901122);
	r1step(D,A,B,C,buffer[13],12,0xfd987193);
	r1step(C,D,A,B,buffer[14],17,0xa679438e);
	r1step(B,C,D,A,buffer[15],22,0x49b40821);

	// Round 2.
	r2step(A,B,C,D,buffer[ 1], 5,0xf61e2562);
	r2step(D,A,B,C,buffer[ 6], 9,0xc040b340);
	r2step(C,D,A,B,buffer[11],14,0x265e5a51);
	r2step(B,C,D,A,buffer[ 0],20,0xe9b6c7aa);
	r2step(A,B,C,D,buffer[ 5], 5,0xd62f105d);
	r2step(D,A,B,C,buffer[10], 9,0x02441453);
	r2step(C,D,A,B,buffer[15],14,0xd8a1e681);
	r2step(B,C,D,A,buffer[ 4],20,0xe7d3fbc8);
	r2step(A,B,C,D,buffer[ 9], 5,0x21e1cde6);
	r2step(D,A,B,C,buffer[14], 9,0xc33707d6);
	r2step(C,D,A,B,buffer[ 3],14,0xf4d50d87);
	r2step(B,C,D,A,buffer[ 8],20,0x455a14ed);
	r2step(A,B,C,D,buffer[13], 5,0xa9e3e905);
	r2step(D,A,B,C,buffer[ 2], 9,0xfcefa3f8);
	r2step(C,D,A,B,buffer[ 7],14,0x676f02d9);
	r2step(B,C,D,A,buffer[12],20,0x8d2a4c8a);

	// Round 3.
	r3step(A,B,C,D,buffer[ 5], 4,0xfffa3942);
	r3step(D,A,B,C,buffer[ 8],11,0x8771f681);
	r3step(C,D,A,B,buffer[11],16,0x6d9d6122);
	r3step(B,C,D,A,buffer[14],23,0xfde5380c);
	r3step(A,B,C,D,buffer[ 1], 4,0xa4beea44);
	r3step(D,A,B,C,buffer[ 4],11,0x4bdecfa9);
	r3step(C,D,A,B,buffer[ 7],16,0xf6bb4b60);
	r3step(B,C,D,A,buffer[10],23,0xbebfbc70);
	r3step(A,B,C,D,buffer[13], 4,0x289b7ec6);
	r3step(D,A,B,C,buffer[ 0],11,0xeaa127fa);
	r3step(C,D,A,B,buffer[ 3],16,0xd4ef3085);
	r3step(B,C,D,A,buffer[ 6],23,0x04881d05);
	r3step(A,B,C,D,buffer[ 9], 4,0xd9d4d039);
	r3step(D,A,B,C,buffer[12],11,0xe6db99e5);
	r3step(C,D,A,B,buffer[15],16,0x1fa27cf8);
	r3step(B,C,D,A,buffer[ 2],23,0xc4ac5665);

	// Round 4.
	r4step(A,B,C,D,buffer[ 0], 6,0xf4292244);
	r4step(D,A,B,C,buffer[ 7],10,0x432aff97);
	r4step(C,D,A,B,buffer[14],15,0xab9423a7);
	r4step(B,C,D,A,buffer[ 5],21,0xfc93a039);
	r4step(A,B,C,D,buffer[12], 6,0x655b59c3);
	r4step(D,A,B,C,buffer[ 3],10,0x8f0ccc92);
	r4step(C,D,A,B,buffer[10],15,0xffeff47d);
	r4step(B,C,D,A,buffer[ 1],21,0x85845dd1);
	r4step(A,B,C,D,buffer[ 8], 6,0x6fa87e4f);
	r4step(D,A,B,C,buffer[15],10,0xfe2ce6e0);
	r4step(C,D,A,B,buffer[ 6],15,0xa3014314);
	r4step(B,C,D,A,buffer[13],21,0x4e0811a1);
	r4step(A,B,C,D,buffer[ 4], 6,0xf7537e82);
	r4step(D,A,B,C,buffer[11],10,0xbd3af235);
	r4step(C,D,A,B,buffer[ 2],15,0x2ad7d2bb);
	r4step(B,C,D,A,buffer[ 9],21,0xeb86d391);

	// Add result to state.
	state[0]+=A;
	state[1]+=B;
	state[2]+=C;
	state[3]+=D;
}

void md5::operator()(const void* data,size_t length)
{
	const unsigned char* p=static_cast<const unsigned char*>(data);
	while (length--)
	{
		// Select position in input buffer.
		uint32& x=buffer[((count++)>>2)&(buffer_size-1)];

		// Shift byte into input buffer word.
		x=(x>>8)|((*p++)<<24);

		// If buffer full then process it.
		if (!(count&0x3f)) process_buffer();
	}
}

void md5::operator()(istream& in)
{
	// Read data from stream in blocks of 4096 bytes.
	char data[4096];
	while (in)
	{
		in.read(data,sizeof(data));
		operator()(data,in.gcount());
	}
}

void md5::operator()()
{
	// Record length.
	// (This is the number of bytes supplied, not counting the
	// padding or the length field.)
	uint64 length=count;

	// Append padding.
	// (All but the first byte of the padding array will be zero.)
	static const unsigned char padding[64]={0x80};
	operator()(padding,64-((count+8)&63));

	// Append length field.
	// (The recorded length is in bytes.  It must be converted to
	// bits before processing.)
	unsigned char blength[8];
	blength[0]=(length<<3)&0xff;
	length>>=5;
	for (int i=1;i!=8;++i)
	{
		blength[i]=length&0xff;
		length>>=8;
	}
	operator()(blength,8);
}

md5::operator string()
{
	static const char hex[]="0123456789abcdef";
	string result;
	result.reserve(state_size<<3);
	for (int i=0;i!=state_size;++i)
		for (int j=0;j!=4;++j)
		{
			char b=state[i]>>(j<<3);
			result+=hex[(b>>4)&0xf];
			result+=hex[(b>>0)&0xf];
		}
	return result;
}

}; /* namespace pkg */
