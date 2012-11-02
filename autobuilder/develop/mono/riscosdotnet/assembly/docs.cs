// This file does not contain any source code. It's purpose is to add pages
// to the doxygen manual that are not specific to any other file.

// This will be a place to jot down any thoughts or explanations on the code in RISCOS.net.

/*! \page notes Notes

\section toolbox Toolbox

\subsection lookuporwrap Toolbox.Object.LookupOrWrap
Used to either find an object if it's been seen before or wrap it if not. This
isn't an ideal solution as new objects have to be manually added to it plus it
will not work for derived objects. Perhaps requires some kind of factory pattern?

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

*/
