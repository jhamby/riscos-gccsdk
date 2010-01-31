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
	LDR	a2, =crt1_data
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

#ifndef __TARGET_SCL__
	.data

crt1_data:
	.word	_init			@ #0
	.word	_fini			@ #4
	.word	__executable_start	@ #8
	.word	0			@ #12 - Free memory pointer
	.word	__data_start		@ #16
	.word	main			@ #20
	.word	0			@ #24 - Always 0 indicating that profiling should be disabled.
	.word	0			@ #28 - Reserved for dynamic linker use
	.word	0			@ #32 - Reserved for dynamic linker use
#endif

# end of crt1-riscos.asm
