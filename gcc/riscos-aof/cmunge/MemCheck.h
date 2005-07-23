/*

	#   #              ###  #                 #
	## ##             #   # #                 #
	# # #  ###  ## #  #     ####   ###   ###  ####
	#   # #   # # # # #     #   # #   # #   # #   #
	#   # ##### # # # #     #   # ##### #     ####
	#   # #     #   # #   # #   # #     #   # #  #
	#   #  ###  #   #  ###  #   #  ###   ###  #   #
	_______________________________________________

Author:		Copyright © 1995, 1996 Julian Smith, Warm
		Silence Software.

Purpose:	Checking of memory accesses.
*/

/*
$Id: MemCheck,v 1.1 2000/12/05 00:49:01 justin Exp $
*/

#ifndef __MemCheck_MemCheck_h
#define __MemCheck_MemCheck_h


#ifdef __cplusplus
	extern "C"	{
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>



typedef int	MemCheck_checking;
/*
Used to store old checking status. Has to be always defined so that user
code which stores old checking will compile in non-MemCheck builds.
 */


typedef int (*MemCheck_printfn)( void* reference, const char* format, va_list args);
/*
Defines type of function to be passed to MemCheck_RedirectToFunction().
 */


#if defined( MemCheck_MEMCHECK)
	
	#if !defined( __CC_NORCROFT) || !defined( __arm) || !defined( __riscos) || !defined( __acorn)
		#error MemCheck will only work with the Acorn C compiler.
	#endif
	
	
	#pragma	check_memory_accesses
/*
Makes Acorn's compiler generate special instructions before each read or
write of memory.
 */
	
	
	void	MemCheck_Init( void);
/*
This (or MemCheck_InitNoDebug) should be called at the start of main().

MemCheck_Init will try to load debug data if possible.

If your main uses argc and argv, call MemCheck_RegisterArgs also.
 */
	
	
	void	MemCheck_InitNoDebug( void);
/*
This should be used instead of MemCheck_Init if you don't want MemCheck
to try to load the debug data in the program image.
 */
	
	
	void	MemCheck_Finalise( void);
/*
Call this in modules' finalise code. Does nothing in non-module version
of MemCheck. In module version, this closes MemChecks's output file and
also frees all memory claimed from the the RMA by MemCheck.
 */
	
	
	void	MemCheck_RegisterArgs( int argc, char* argv[]);
/*
Call this function to register the parameters given to main() with
MemCheck. Each argument string is registered as a read-only.
 */
	
	
void	MemCheck_InterceptSCLStringFunctions( void);
/*
Intercepts calls to various ANSI str/mem functions in the shared C
library, and checks the array parameters for read/write access.
 */
	
	
	void	MemCheck_CheckMallocHeap( void);
/*
Checks whether the malloc heap is consistant. This is called whenever
malloc, realloc and free is called, so you won't usually need to call it
yourself.
 */
	
	
	void	MemCheck_SetMallocHeapChecking( int check);
/*
If 'check' is 0, then MemCheck_CheckMallocHeap is not called
automatically when malloc, realloc and free are called.

If 'check' is 1, MemCheck_CheckMallocHeap is called automatically when
malloc, realloc and free are called.

The default is as though MemCheck_SetMallocHeapChecking( 1) was called.
 */
	
	
	MemCheck_checking	MemCheck_SetReadChecking( int checkreads);
/*
checkreads=1 - turns on checking of memory-reads.

checkreads=0 - turns off checking of memory-reads.

Also see MemCheck_SetWriteChecking, MemCheck_SetChecking.
 */
	
	
	MemCheck_checking	MemCheck_SetWriteChecking( int checkwrites);
/*
checkwrites=1 - turns on checking of memory-writes.

checkwrites=0 - turns off checking of memory-writes.

Also see MemCheck_SetReadChecking, MemCheck_SetChecking.
 */
	
	
	MemCheck_checking	MemCheck_SetChecking( int checkreads, int checkwrites);
/*
Sets whether to check reads or writes. These are initially both set to 1
- ie reads and writes are both checked.

This simply calls MemCheck_SetReadChecking and
MemCheck_SetWriteChecking.
 */
	
	
	void	MemCheck_RestoreChecking( MemCheck_checking checking);
/*
Restores checking status returned by a previous call to
MemCheck_SetReadChecking, MemCheck_SetWriteChecking or
MemCheck_SetChecking.
 */
	
	
	void	MemCheck_SetReadQuitting( int quitread);
/*
quitread=1 - quit after an illegal read of memory.

Also see MemCheck_SetWriteQuitting, MemCheck_SetQuitting.
 */
	
	
	void	MemCheck_SetWriteQuitting( int quitwrite);
/*
quitwrite=1 - quit program after an illegal write of memory.

Also see MemCheck_SetReadQuitting, MemCheck_SetQuitting.
 */
	
	
	void	MemCheck_SetQuitting( int quitread, int quitwrite);
/*
Sets whether to terminate program after a read or write error has been
detected. Initially, quitwrite is 1, quitread is 0 (ie program is
quitted only after a write error).

This simply calls MemCheck_SetReadQuitting and
MemCheck_SetWriteQuitting.
 */
	
	
	void	MemCheck_SetDefaultMallocReadAccess( int readaccess);
/*
readaccess=1	- Subsequent malloc blocks are readable.

readaccess=0	- Subsequent malloc blocks are not readable.

Default read-access is initially 1 (ie reads from malloc blocks are
allowed).

Also see MemCheck_SetDefaultMallocWriteAccess,
MemCheck_SetDefaultMallocAccess.
 */
	
	
	void	MemCheck_SetDefaultMallocWriteAccess( int writeaccess);
/*
writeaccess=1	- Subsequent malloc blocks are writable.
writeaccess=0	- Subsequent malloc blocks are not writable.

Default write-access is initially 1 (ie writes to malloc blocks are
allowed).

Also see MemCheck_SetDefaultMallocReadAccess,
MemCheck_SetDefaultMallocAccess.
 */
	
	
	void	MemCheck_SetDefaultMallocAccess( int readaccess, int writeaccess);
/*
Simply calls MemCheck_SetDefaultMallocReadAccess and
MemCheck_SetDefaultMallocWriteAccess.
 */
	
	
	void	MemCheck_SetDefaultMiscReadAccess( int readaccess);
/*
readaccess=1	- Subsequent misc blocks are readable.

readaccess=0	- Subsequent misc blocks are not readable.

Read-access is initially 1 (ie reads from misc blocks are allowed).

Also see MemCheck_SetDefaultMiscWriteAccess,
MemCheck_SetDefaultMiscAccess.
 */
	
	
	void	MemCheck_SetDefaultMiscWriteAccess( int writeaccess);
/*
writeaccess=1	- Subsequent misc blocks are writable.

writeaccess=0	- Subsequent misc blocks are not writable.

Write-access is initially 1 (ie writes to misc blocks are allowed).

Also see MemCheck_SetDefaultMiscReadAccess,
MemCheck_SetDefaultMiscAccess.
 */
	
	
	void	MemCheck_SetDefaultMiscAccess( int readaccess, int writeaccess);
/*
Simply calls MemCheck_SetDefaultMiscReadAccess and
MemCheck_SetDefaultMiscWriteAccess.
 */
	
	
	void	MemCheck_SetDefaultFlexReadAccess( int readaccess);
/*
readaccess=1	- Subsequent flex blocks are readable.

readaccess=0	- Subsequent flex blocks are not readable.

Read-access is initially 1 (ie reads from flex blocks are allowed).

Also see MemCheck_SetDefaultFlexWriteAccess,
MemCheck_SetDefaultFlexAccess.
 */
	
	
	void	MemCheck_SetDefaultFlexWriteAccess( int writeaccess);
/*
writeaccess=1	- Subsequent flex blocks are writable.

writeaccess=0	- Subsequent flex blocks are not writable.

Write-access is initially 1 (ie writes to flex blocks are allowed).

Also see MemCheck_SetDefaultFlexReadAccess,
MemCheck_SetDefaultFlexAccess.
 */
	
	
	void	MemCheck_SetDefaultFlexAccess( int readaccess, int writeaccess);
/*
Simply calls MemCheck_SetDefaultFlexReadAccess and
MemCheck_SetDefaultFlexWriteAccess.
 */
	
	
	void	MemCheck_SetReadAccessQuitting( int quitreadaccesserror);
/*
quitreadaccesserror=1 - quit program after read of memory set to be
unreadable.

quitreadaccesserror=0 - don't quit program after read of memory set to
be unreadable.

Also see MemCheck_SetWriteAccessQuitting, MemCheck_SetAccessQuitting.
 */
	
	
	void	MemCheck_SetWriteAccessQuitting( int quitwriteaccesserror);
/*
quitwriteaccesserror=1 - quit after write to memory set to be
unwriteable.

quitwriteaccesserror=0 - don't quit program after write of memory set to
be unwriteable.

Also see MemCheck_SetReadAccessQuitting, MemCheck_SetAccessQuitting.
 */
	
	
	void	MemCheck_SetAccessQuitting( int quitreadaccesserror, int quitwriteaccesserror);
/*
Simply calls MemCheck_SetReadAccessQuitting and
MemCheck_SetWriteAccessQuitting.
 */
	
	
	void	MemCheck_SetStoreMallocFunctions( int yes);
/*
Sets whether function nesting is stored for each subsequent malloc
block.

yesno=1 - store function nesting.

yesno=0 - don't store functio nesting.

Initially, function-nesting is /not/ stored.
 */
	
	
	void	MemCheck_SetReportFrees( int yes);
/*
'yes'=1 - Makes MemCheck report info on every malloc block passed to free().

'yes'=0 - Turns off report of calls to free().
 */
 	
 	
	void	MemCheck_RegisterMiscBlock( const void* ptr, size_t size);
/*
This will add the specified block to MemCheck's list of read and
writeable blocks of memory.

Useful if (for eg) your program uses RMA memory.

Also see MemCheck_ResizeMiscBlock, MemCheck_UnRegisterMiscBlock,
MemCheck_RegisterFlexBlock.
 */
	
	
	void	MemCheck_RegisterMiscBlock_String( const char* string);
/*
Registers the specified string as a misc block, including the '\0'
terminator.

This function is provided because the obvious way of registering a
string 'MemCheck_RegisterMiscBlock( string, 1 + strlen( string));'
causes MemCheck to report a read error in the call to strlen(). Thus one
has to turn off checking around the call to strlen(), which is clumsy.
 */
	
	
	void	MemCheck_RegisterMiscBlock_StringCR( const char* stringcr);
/*
As MemCheck_RegisterMiscBlock_String, except that the string is
terminated by any control character.
 */
	
	
	#define	MemCheck_RegisterMiscBlock_Ptr( ptr)	MemCheck_RegisterMiscBlock( ptr, sizeof( *(ptr)))
/*
Registers block of memory pointed to by ptr, with the size being the
size of the deferenced pointer.
 */
	
	
	void	MemCheck_ResizeMiscBlock( const void* ptr, size_t newsize);
/*
Informs MemCheck of new size of specified block. 'ptr' should be
identical to the 'ptr' in a previous call to MemCheck_RegisterMiscBlock.

Also see MemCheck_RegisterMiscBlock, MemCheck_UnRegisterMiscBlock,
MemCheck_RegisterFlexBlock.
 */
	
	
	void	MemCheck_UnRegisterMiscBlock( const void* ptr);
/*
Call when a block of memory is freed. 'ptr' should be identical to the
'ptr' in a previous call to MemCheck_RegisterMiscBlock.

Also see MemCheck_RegisterMiscBlock, MemCheck_ResizeMiscBlock,
MemCheck_UnRegisterFlexBlock.
 */
	
	
	void	MemCheck_RegisterFlexBlock( void** anchor, size_t size);
/*
This will add the specified shifting-heap block to MemCheck's list of
read and writeable blocks of memory.

'*anchor' is assumed to point to the block of memory, and can change
during program execution.

This is for use with shifting-heap memory allocators such as Acorn's
flex library.

Example usage:

{
void*	anchor;

flex_alloc( &anchor, 320);
MemCheck_RegisterFlexBlock( &anchor, 320);
...
flex_extend( &anchor, 400);
MemCheck_ResizeFlexBlock( &anchor, 400);
...
flex_midextend( &anchor, 20, 8);
MemCheck_ResizeFlexBlock( &anchor, 408);
...
flex_free( &anchor);
MemCheck_UnRegisterFlexBlock( &anchor);
}

It is recommended that you use a central set of wrapper-functions for
flex_* functions which call the MemCheck flex-block functions. This will
ensure that MemCheck is always informed of changes to flex blocks.

Also see MemCheck_UnRegisterFlexBlock, MemCheck_RegisterMiscBlock.
 */
	
	
	void	MemCheck_UnRegisterFlexBlock( void** anchor);
/*
Call when a block of memory is freed. 'anchor' should be identical to
the 'anchor' in a previous call to MemCheck_RegisterFlexBlock.

Also see MemCheck_RegisterFlexBlock, MemCheck_ResizeFlexBlock.
 */
	
	
	void	MemCheck_ResizeFlexBlock( void** anchor, size_t newsize);
/*
Call this function when a flex block is resized.

Also see MemCheck_RegisterFlexBlock, MemCheck_UnRegisterFlexBlock.
 */
	
	
	void	MemCheck_SetBlockReadable( const void* ptr, int readable);
/*
'ptr' should be pointer to a malloc, misc or flex block known to
MemCheck.

readable=0:	Block is made unreadable (ie attempts to read
		within block are faulted).
		
readable=1:	Makes block readable.

The default read-access for blocks can be set with
MemCheck_SetDefaultMallocReadAccess, MemCheck_SetDefaultMiscReadAccess
and MemCheck_SetDefaultFlexReadAccess.

Also see MemCheck_SetBlockAccess.
 */
	
	
	void	MemCheck_SetBlockWritable( const void* ptr, int readable);
/*
'ptr' should be pointer to a malloc, misc or flex block known to
MemCheck.

writable=0:	Block is made unwritable (ie attempts to write
		within block are faulted).

writable=1:	Makes block writableable.

The default write-access for blocks can be set with
MemCheck_SetDefaultMallocWriteAccess, MemCheck_SetDefaultMiscWriteAccess
and MemCheck_SetDefaultFlexWriteAccess.

Also see MemCheck_SetBlockReadable.
 */
 	
 	
 	
 	void	MemCheck_SetBlockAccess( const void* ptr, int readaccess, int writeaccess);
/*
'ptr' should be pointer to a malloc, misc or flex block known to
MemCheck.

readable=0:	Block is made unreadable (ie attempts to read
		from within the block are faulted).

readable=1:	Makes block readable.

writable=0:	Block is made unwritable (ie attempts to write
		within block are faulted).

writable=1:	Makes block writable.

The default access permissions for blocks can be set with
MemCheck_SetDefaultMallocAccess, MemCheck_SetDefaultMiscAccess
and MemCheck_SetDefaultFlexAccess.

Also see MemCheck_SetBlockReadable, MemCheck_SetBlockWritable.
 */

	
	
	void	MemCheck_OutputBlocksInfo( void);
/*
Sends information about all known blocks to MemCheck output.
 */
	
	
	void	MemCheck_SetAutoOutputBlocksInfo( int yes);
/*
yes = 1:	Info on all known blocks are output after each MemCheck
		diagnostic, using MemCheck_OutputBlocksInfo.
yes = 0:	Block info not output.

MemCheck initially behaves as though 
MemCheck_SetAutoOutputBlocksInfo( 1) was called - information on all
known blocks /is/ shown after each diagnostic.

This function is provided because the blocks information can make
MemCheck's output very large.
 */
	
	
	void	MemCheck_SetOSBuffersLifetime( int lifetime);
/*
This effects the lifetime that MemCheck assigns to the static buffers
returned by OS functions such as getenv, _kernel_last_oserror,
_kernel_swi etc.

Most of the time, you shouldn't have to worry about using this function.

Consider the following code, run with the system variables <Foo> set,
and <Bar> unset:
	{
	char*	f = getenv( "Foo");	|* non-NULL	*|
	char*	b = getenv( "Bar");	|* NULL		*|
	if ( f[0])	{...}
	}

Initially, the access of the buffer pointed to by f[0] will be allowed,
because MemCheck only unregisters an old static buffer when a non-NULL
buffer is returned.

However, after MemCheck_SetOSBuffersLifetime( 0) is called, MemCheck is
stricter. It will unregister buffers immediately, even if the
replacement buffer is NULL. The documentation for getenv et al is vague
about the precise nature of the lifetime of the returned buffers, so it
might be useful to ensure code works after
MemCheck_SetOSBuffersLifetime( 0).

life = 0:	Old OS buffers are unregistered even if the new 
		buffer is NULL.

life = 1:	Old OS buffers are only unregistered when a 
		non-NULL replacement is returned.

Note that the wrappers for getenv, _kernel_last_oserror etc are
completely independent - calling _kernel_last_oserror will never
unregister a buffer previously returned by getenv, for example.
 */
	
	
	int	MemCheck_Printf( const char *fmt, ...);
/*
Behaves exactly like printf, except that sends output to wherever
MemCheck diagnostics are going.

Also see MemCheck_RedirectToStream, MemCheck_RedirectToFilename and
MemCheck_RedirectToFilenamef.
 */
	
	
	void	MemCheck_RedirectToStream( FILE* stream);
/*
Makes subsequent MemCheck diagnostics go to 'stream'.

Also see MemCheck_RedirectToFilename, MemCheck_RedirectToFilenamef.
 */
	
	
	void	MemCheck_RedirectToFilename( const char* filename);
/*
Makes subsequent MemCheck diagnostics go to 'filename'.

Also see MemCheck_RedirectToFilenamef, MemCheck_RedirectToStream.
 */
	
	
	void	MemCheck_RedirectToFilenamef( const char* fmt, ...);
/*
Makes subsequent MemCheck diagnostics go to 'fmt, ...', using
printf-style specification of the filename.

Also see MemCheck_RedirectToFilename, MemCheck_RedirectToStream.
 */
	
	
	void	MemCheck_RedirectToFunction( MemCheck_printfn fn, void* reference);
/*
Sends subsequent MemCheck diagnostics to the specified function.
 */
 	
 	
	void	MemCheck_RegisterMiscStack( void* bottom, void* top);
/*
Registers an area of memory which MemCheck should treat as a single
non-extensible stack.

This is for situations where there is no valid _kernel_stack_chunk
structure below the stack.

Note that MemCheck already knows about the SVC stack.

MemCheck only stores the most recent stack given to
MemCheck_RegisterMiscStack.

When examining the stack, MemCheck first checks for the sl being from
the SVC stack (8k stack at 0xC00000), then for sl being within the last
stack passed to MemCheck_RegisterMiscStack.

If neither of these is true, the stack is assumed to be as described in
PRMs 4-239, ie each stack chunk has a valid _kernel_stackchunk at
sl-560, with possibly multiple stack chunks.

Thus a stack passed to MemCheck_RegisterMiscStack is used in preference
to the normal multiple-stackchunks stack.

Also see MemCheck_UnRegisterMiscStack.
 */
	
	
	void	MemCheck_UnRegisterMiscStack( void* bottom);
/*
Guess...

Also see MemCheck_RegisterMiscStack.
 */
	
	
	extern int	MemCheck_threaddepth;
/*
This is non-zero if MemCheck is currently threaded. This is for use by
preemtive threads packages, so that they can refuse to switch threads
when MemCheck is threaded - MemCheck uses a lot of internal static data,
so isn't reenterant.
 */

#else

/*
when compilation is without MemCheck_MEMCHECK defined, all MemCheck
calls are removed using the following macros:
 */

	#define	MemCheck_Init()
	#define	MemCheck_InitNoDebug()
	#define	MemCheck_Finalise()
	
	#define	MemCheck_RegisterArgs( argc, argv)
	#define	MemCheck_InterceptSCLStringFunctions()
	
	#define	MemCheck_CheckMallocHeap()
	#define	MemCheck_SetMallocHeapChecking( check)
	
	#define	MemCheck_SetReadChecking( checkreads)	0
	#define	MemCheck_SetWriteChecking( checkwrites)	0
	#define	MemCheck_SetChecking( checkreads, checkwrites)	0
	#define	MemCheck_RestoreChecking( checking)	(checking)=(checking)
	
	#define	MemCheck_SetReadQuitting( quitread)
	#define	MemCheck_SetWriteQuitting( quitwrite)
	#define	MemCheck_SetQuitting( quitread, quitwrite)
	
	#define	MemCheck_SetDefaultMallocReadAccess( readaccess)
	#define	MemCheck_SetDefaultMallocWriteAccess( writeaccess)
	#define	MemCheck_SetDefaultMallocAccess( readaccess, writeaccess)
	#define	MemCheck_SetDefaultMiscReadAccess( readaccess)
	#define	MemCheck_SetDefaultMiscWriteAccess( writeaccess)
	#define	MemCheck_SetDefaultMiscAccess( readaccess, writeaccess)
	#define	MemCheck_SetDefaultFlexReadAccess( readaccess)
	#define	MemCheck_SetDefaultFlexWriteAccess( writeaccess)
	#define	MemCheck_SetDefaultFlexAccess( readaccess, writeaccess)
	
	#define	MemCheck_SetReadAccessQuitting( quitreadaccesserror)
	#define	MemCheck_SetWriteAccessQuitting( quitwriteaccesserror)
	#define	MemCheck_SetAccessQuitting( quitreadaccesserror, quitwriteaccesserror)
	
	#define	MemCheck_SetStoreMallocFunctions( yesno)
	#define	MemCheck_SetReportFrees( yes)
	
	#define	MemCheck_RegisterMiscBlock( ptr, size)
	#define	MemCheck_RegisterMiscBlock_String( string)
	#define	MemCheck_RegisterMiscBlock_StringCR( stringcr)
	#define	MemCheck_RegisterMiscBlock_Ptr( ptr)
	#define	MemCheck_ResizeMiscBlock( ptr, newsize)
	#define	MemCheck_UnRegisterMiscBlock( ptr)
	
	#define	MemCheck_RegisterFlexBlock( anchor, size)
	#define	MemCheck_UnRegisterFlexBlock( anchor)
	#define	MemCheck_ResizeFlexBlock( anchor, newsize)
	
	#define	MemCheck_SetBlockReadable( ptr, readable)
	#define	MemCheck_SetBlockWritable( ptr, writable)
	#define	MemCheck_SetBlockAccess( ptr, readable, writable)
	
	#define	MemCheck_OutputBlocksInfo()
	#define	MemCheck_SetAutoOutputBlocksInfo( yes)
	
	#define	MemCheck_SetOSBuffersLifetime(lifetime)
	
	#define	MemCheck_Printf (1) ? 0 : printf
	
	#define	MemCheck_RedirectToStream( stream)
	#define	MemCheck_RedirectToFilename( filename)
	#define	MemCheck_RedirectToFilenamef (1) ? 0 : printf
	#define	MemCheck_RedirectToFunction( fn, reference)
	
	#define	MemCheck_RegisterMiscStack( bottom, top)
	#define	MemCheck_UnRegisterMiscStack( bottom)
	
	#define	MemCheck_threaddepth	0

#endif



#ifdef __cplusplus
	}
#endif

#endif
