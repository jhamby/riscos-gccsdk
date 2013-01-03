// This file does not contain any source code. It's purpose is to add pages
// to the doxygen manual that are not specific to any other file.

// This will be a place to jot down any thoughts or explanations on the code in RISCOS.net.

/*! \page notes Notes

\section toolbox Toolbox

\subsection objectcreation Creating Objects
All Toolbox objects derive from the \b Toolbox.Object class and are created using
one of the CreateInstance methods. These are static methods, so do not require an
object instance to use them although you will have to use the fully qualified
name. They use generics whereby the type argument is the type of the Toolbox object
that you wish to create. For example, to create a Window:

\code
var window = Toolbox.Object.CreateInstance<Window> ("MainWindow");
\endcode

In this case, MainWindow is the name of the template in the Resource file.

\subsection sharedobjects Shared Toolbox Objects
A shared Toolbox object is only created once by the Toolbox. Further attempts to
create such an object cause the Toolbox to return the same ID and increase a
reference count.<br>
For C#, this means that there is only one managed object for this shared Toolbox
object. If, for example, in a multi document editor, there is a shared Scale
dialogue object, and each document viewer registers an interest in the AppyScale
event, then if one document is scaled all documents viewers will receive the event.
This is unlikely to be the intended effect and infact can be disasterous for certain
events, for example, those involved with the RAM transfer protocol in the SaveAs
dialogue. There are two ways to overcome this; either make all objects non-shared,
or add a check in the event handler to ensure that it is the intended target of the
event. If the document viewer is set to be an ancestor object, then this can be done
by testing the scale dialogue's ancestor against the document viewer.

\subsection autoobjects Automatically created objects
If an object is flagged in !ResEd to be automatically created then the toolbox will
create the object as soon as required and send a Toolbox event to the task, containing
the ID of the object plus the name of the template that it was created from. The task
can then create a C# object as a result of this event.<br>
If, however, the task has already created the object thereby placing it in the list of
known objects, then this will not work as the library will attempt to dispatch the event
to the object.

\section wimp Wimp

\subsection windows Creating windows

A window can be created by first defining its attributes:

\code
var attributes = new Wimp.WindowAttributes ("A Wimp window");
\endcode

In the above case, only a title string is supplied, but we can also supply the
maximum length of the title, if we may wish to change the text in the future, and also
a validation string.<br>
The attributes of the window are filled in with sensible values by default, but all can
be changed if required. For example, both vertical and horizontal scroll bars are initially
enabled, to disable the horizontal scroll bar, use:

\code
attributes.HorizontalScrollBar = false;
\endcode

Finally, the attributes are used to actually create the window:

\code
var window = new Wimp.Window (attributes);
\endcode

\section sprites Sprites

\subsection spritearea Creating a sprite area

Care must be taken as to where a sprite area is placed. Although it should be technically possible
to place in managed memory, the garbage collector is likely to move it invalidating any sprite
pointers. This means that any sprite operations must be done by sprite name, or the area locked
down before taking the address of a sprite and performing the required operations.<br>
A better option is to use unmanaged memory such as a heap in a dynamic area:

\code
// Create a heap in a dynamic area named "Sprites" with an initial size of 4KB and
// a maximum size of 10MB.
var heap = new OS.DynamicAreaHeap ("Sprites", 10 * 1024 * 1024, 4 * 1024 * 1024);

// Allocate a block of memory from the heap for the sprite area.
var sprite_area_memory = heap.Alloc (1 * 1024 * 1024);

// Create a sprite area in the memory block.
var sprite_area = new OSSpriteOp.SpriteArea (sprite_area_memory, 1 * 1024 * 1024);
\endcode

\subsection newsprite Creating a sprite

There are two types of sprite object - SpriteByName and SpriteByPointer.

\code
var sprite1 = new OSSpriteOp.SpriteByName (sprite_area, "sprite1", false, 50, 50, 28);
\endcode

Calling methods on sprite1 will perform operations on the sprite via its name.

\code
var sprite2 = new OSSpriteOp.SpriteByPointer (sprite_area, "sprite2", false, 50, 50, 28);
\endcode

Calling methods on sprite2 will perform operations on the sprite via its pointer.

*/

/*
FIXME: Need to verify this:
----
 As sprite2 is derived from SpriteByName, it's still possible to access sprite operations
by sprite name by calling base methods.
----
*/
