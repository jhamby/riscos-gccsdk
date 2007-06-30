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
	
	.file		"crt1-riscos.asm"

#ifndef __TARGET_MODULE__
@	.section ".init","x"
	@ This function appears in the .init section to ensure it appears
	@ at the start of the ELF executable.  Though not strictly necessary
	@ it provides a common entry point at 0x8000 inline with expectations
	@ of the AIF format.

	@ XXX Futuremore, this code should appear in the libc crt0.S file.
	.global	_start
	.type	_start, %function
_start:
#ifndef __TARGET_SCL__
	@ On entry to _start, a1 is set by the dynamic loader to the start of
	@ free memory after it has claimed what it requires. This value is stored
	@ in the data block which is then passed to the runtime library.
	LDR	a2, =rt_data
	STR	a1, [a2, #12]
	MOV	a1, a2
#endif
	
	@ On RISC OS the main entry point to the run-time library is
	@ always called __main.
	B	__main

	@ This never gets executed, but it tells the linker to
	@ pull in a definition of 'main' from somewhere, so that if an
	@ application is linking when 'main' is in a static library,
	@ it ensures that 'main' actually gets pulled in.
	.type	main, %function
	B	main
	.size	_start, . - _start
#endif

#if !defined(__TARGET_MODULE__) && defined(__TARGET_SCL__)
	@ FIXME: currently disabled as we're no yet supporting C++ & module.
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
#endif

#ifdef __TARGET_SCL__
	.section ".eh_frame","aw",%progbits
	.global	__EH_FRAME_BEGIN__
	.type	__EH_FRAME_BEGIN__, %object
__EH_FRAME_BEGIN__:
#endif

#ifndef __TARGET_SCL__
	.data

rt_data:
	.word	_init
	.word	_fini
	.word	__executable_start
	.word	0
	.word	__data_start
	.word	main
#endif

# end of crt1-riscos.asm
