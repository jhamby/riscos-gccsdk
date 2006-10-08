@;* entry:
@;*  r0 = pointer to store file handle at
@;*  r1 = pointer to filename
@;*  r2 = buffer to hold header
@;* exit:
@;*  r0 = 0 if successful otherwise pointer to error block
@	IMPORT	open_ELF_file

@;* entry:
@;*  r0 = pointer to ELF header
@;*  r1 = pointer to 2 word block to store program header size and
@;*       number of program headers
@;* exit:
@;*  r0 = size of buffer required to hold program headers
@	IMPORT	read_program_headers

@;* entry:
@;*  r0 = ELF header
@;*  r1 = file handle
@;*  r2 = buffer to hold program headers
@;*  r3 = size of program headers
@;* exit:
@;*  r0 = 0 for success or pointer to error block
@	IMPORT	load_program_headers
