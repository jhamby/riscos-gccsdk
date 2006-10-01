#   Copyright (C) 2005 Free Software Foundation, Inc.
#   Written By Nick Burrett <nick@sqrt.co.uk>
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

# The last file on the linker command line simply zero terminates the
# list of C++ constructors/destructors to help the __do_global_{c,d}tors()
# function.

	.file	"crtn.asm"
	.section ".ctors","aw",%progbits
	.word	0

	.section ".dtors","aw",%progbits
	.word	0

	@ unwind-dw2-fde requires that the FDE data be terminated by
	@ a structure with length 0, which we can achieve by just ensuring
	@ that the last value in the .eh_frame section is 0.
	.section ".eh_frame","aw",%progbits
	.word	0
	
# end of crtn-riscos.asm
