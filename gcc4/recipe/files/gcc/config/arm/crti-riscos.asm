# Copyright (C) 2005 Free Software Foundation, Inc.
# Written By Nick Burrett <nick@sqrt.co.uk>
# 
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2, or (at your option) any
# later version.
# 
# In addition to the permissions in the GNU General Public License, the
# Free Software Foundation gives you unlimited permission to link the
# compiled version of this file with other programs, and to distribute
# those programs without any restriction coming from the use of this
# file.  (The General Public License restrictions do apply in other
# respects; for example, they cover modification of the file, and
# distribution when not linked into another program.)
# 
# This file is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
# 
#    As a special exception, if you link this library with files
#    compiled with GCC to produce an executable, this does not cause
#    the resulting executable to be covered by the GNU General Public License.
#    This exception does not however invalidate any other reasons why
#    the executable file might be covered by the GNU General Public License.
# 

# The first file to begin on the linker command line.  Ensure that the
# first word of the lists for C++ constructors/destructors is '-1' so
# that the __do_global_{c,d}tors() code will walk through the list and
# count the entries that it is supposed to be calling.
	
	.file		"crti-riscos.asm"

	.section ".init","x"
	@ This function appears in the .init section to ensure it appears
	@ at the start of the ELF executable.  Though not strictly necessary
	@ it provides a common entry point at 0x8000 inline with expectations
	@ of the AIF format.

	@ XXX Futuremore, this code should appear in the libc crt0.S file.
	.global	_start
	.type	_start, %function
_start:
	@ UnixLib must be initialised before any constructors can be called.
	@ a1 is set by the dynamic loader. In a static executable, a2 & a3, as
	@ set below, are ignored.
	LDR	a2, =__executable_start
	LDR	a3, =__data_start
	BL	__unixlib_main

	@ Run global object constructors in shared libraries
@	BL	__unixlib_call_ctors

	@ Run global object constructors in executable
@	BL	_init

	@; Run the users program.
	BL	_main
	@; C programs always terminate by calling exit.
	B	exit
	.type	_start, %function
	.size	_start, . - _start

	@ _main used to be in unix.c, however, as it calls main() in the executable,
	@ it can't remain in the shared library. This is the ARM code equivalent as
	@ produced by GCC 4. To make things easier, this applies to static executables
	@ as well. Note that if the positions of the argc or argv members of __u change
	@ then this code will have to be updated.
_main:
	LDR	a4, =__u
	LDR	ip, [a4, #0]
	LDR	a4, =environ
	LDR	a2, [ip, #4]
	LDR	a3, [a4, #0]
	LDR	a1, [ip, #0]
	B	main
	.type	_main, %function
	.size	_main, . - _main


	.section ".ctors","aw",%progbits
	.global	__CTOR_LIST__
	.type	__CTOR_LIST__, %object
__CTOR_LIST__:
	.word	-1

	.section ".dtors","aw",%progbits
	.global	__DTOR_LIST__
	.type	__DTOR_LIST__, %object
__DTOR_LIST__:
	.word	-1

	.section ".eh_frame","aw",%progbits
	.global	__EH_FRAME_BEGIN__
	.type	__EH_FRAME_BEGIN__, %object
__EH_FRAME_BEGIN__:

# end of crti-riscos.asm
