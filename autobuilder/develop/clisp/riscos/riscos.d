/*
 * Extensions for CLISP on RISC OS
 * Crispian Daniels 2006
 */

#include "lispbibl.c"

#include <swis.h>
#include <memory.h>

/* error: SWI number not valid */
nonreturning_function(local, fehler_swi_number, (object obj)) {
	pushSTACK(obj);
	pushSTACK(TheSubr(subr_self)->name);
	fehler(error, GETTEXT("~S: ~S is not a 24-bit integer."));
}

/* error: SWI parameter isn't a supported type */
nonreturning_function(local, fehler_input_value, (object obj)) {
	pushSTACK(obj);
	pushSTACK(TheSubr(subr_self)->name);
	fehler(error, GETTEXT("~S: input parameter ~S is not of type nil, integer, string, (simple-array (unsigned-byte 8)) or ( (simple-array (unsigned-byte 8)) . integer )."));
}

/* error: SWI returned an error */
nonreturning_function(local, fehler_swi_error, (_kernel_oserror *oserror)) {
	pushSTACK(asciz_to_string(oserror->errmess, Symbol_value(S(ascii))));
	pushSTACK(TheSubr(subr_self)->name);
	fehler(error, GETTEXT("~S: SWI returned an error: ~S."));
}


/* paramlist_to_asciizlist derived from stringlist_to_asciizlist found in pathname.d */
local maygc void paramlist_to_asciizlist (object paramlist)
{
	pushSTACK(NIL);/*result head*/
	pushSTACK(NIL);/*result tail*/
	pushSTACK(paramlist);/*paramlist head*/
	while (consp(STACK_0/*paramlist head*/)) {
		var object tmp = allocate_cons();
		if (nullp(STACK_2/*result*/)) STACK_1 = STACK_2 = tmp;
		else { Cdr(STACK_1/*result tail*/) = tmp; STACK_1 = tmp; }
		tmp = Car(STACK_0);
		if (stringp(tmp))
		{
			tmp = string_to_asciz(tmp,Symbol_value(S(ascii)));
		}
		else
		{
			tmp = NIL;
		}
		Car(STACK_1) = tmp;
		STACK_0 = Cdr(STACK_0);
	}
	skipSTACK(2); /* drop paramlist and result tail */
}


/*
 Interpretation of values in the input parameter list:
  nil: skip the register,
  integer: set the register to the integer's value,
  string: point the register to a temporary ASCII copy of the string (changes will be lost),
  (simple-array (unsigned-byte 8)): point the register to the start of the array in memory,
  ( (simple-array (unsigned-byte 8)) . integer ): 
  
*/


local maygc void set_register_array(_kernel_swi_regs *regs, object list)
{
  var object ascii_list;
  var int regno;

  paramlist_to_asciizlist(list);
  ascii_list = popSTACK();

  var object head = list;

  for (regno=0; regno<10; )
  {
    var object ascii;
    var object input;

    if (!consp(list))
	{
	  if (nullp(list))
      {
        break;
      }
	  fehler_list(head);
	}

    ascii = Car(ascii_list);
    input = Car(list);

    ascii_list = Cdr(ascii_list);
    list = Cdr(list);

    if (!nullp(ascii))
    {
      regs->r[regno] = (uintL)TheAsciz(ascii);
    }
    else if (nullp(input))
    {
      regs->r[regno] = 0;
    }
    else if (integerp(input))
    {
      regs->r[regno] = I_to_uint32(input);
    }
    else if (simple_bit_vector_p(Atype_8Bit,input))
    {
      regs->r[regno] = (uintL)&TheSbvector(input)->data[0];
    }
    else if (consp(input) && simple_bit_vector_p(Atype_8Bit,Car(input)) && integerp(Cdr(input)))
    {
      regs->r[regno] = (uintL)&TheSbvector(Car(input))->data[I_to_uint32(Cdr(input))];
    }
    else
    {
      fehler_input_value(input);
    }

    regno++;
  }
}


local maygc void list_from_register_array(_kernel_swi_regs *regs)
{
  var int regno;

  pushSTACK(NIL);/*list tail*/
  for (regno = 10; --regno >= 0; )
  {
    var object tmp = allocate_cons();
	Cdr(tmp) = STACK_0;
	STACK_0 = tmp;
	Car(tmp) = uint32_to_I(regs->r[regno]);
  }
}


/* (RISCOS:SWI no in)
 Returns a list of the first 10 register values.
 Doesn't return if an error occurs.
*/
LISPFUNN(riscos_swi,2)
{
  var uintL no = I_to_uint32(STACK_1);
  var _kernel_swi_regs in, out;
  var _kernel_oserror *oserror;

  if (no >= (1<<24))
  {
    fehler_swi_number(STACK_1);
  }
  no |= XOS_Bit;

  set_register_array(&in, STACK_0);
  oserror = _kernel_swi(no, &in, &out);

  if (oserror != NULL)
  {
    fehler_swi_error(oserror);
  }

  list_from_register_array(&out);
  VALUES1(STACK_0);
  skipSTACK(3);
}

/* (RISCOS:SWIX no in)
 First return value is a list of the first 10 register values, or a dotted cons containing the error number and message.
 Second return value is t if SWI succeeded without an error, nil otherwise.
*/
LISPFUNN(riscos_swix,2)
{
  var uintL no = I_to_uint32(STACK_1);
  var _kernel_swi_regs in, out;
  var _kernel_oserror *oserror;

  if (no >= (1<<24))
  {
    fehler_swi_number(STACK_1);
  }
  no |= XOS_Bit;

  set_register_array(&in, STACK_0);
  oserror = _kernel_swi(no, &in, &out);

  if (oserror != NULL)
  {
    STACK_0 = allocate_cons();
	Car(STACK_0) = uint32_to_I(oserror->errnum);
	Cdr(STACK_0) = asciz_to_string(oserror->errmess, Symbol_value(S(ascii)));
    VALUES2(STACK_0, NIL);
	skipSTACK(2);
  }
  else
  {
    list_from_register_array(&out);
    VALUES2(STACK_0, T);
    skipSTACK(3);
  }
}


/* (RISCOS:MALLOC size) */
LISPFUNN(riscos_malloc,1)
{
	var uintL size = I_to_uint32( popSTACK() );

	VALUES1(uint32_to_I(malloc(size)));
}


/* (RISCOS:FREE ptr) */
LISPFUNN(riscos_free,1)
{
	var uintL ptr = I_to_uint32( popSTACK() );

	free(ptr);
	VALUES1(NIL);
}


/* (RISCOS:MEMORYREF address) */
LISPFUNNR(riscos_memoryref,1)
{
	var volatile uint8 *address = (volatile uint8*)I_to_uint32( popSTACK() );

	VALUES1(uint8_to_I(*address));
}


/* (RISCOS:MEMORYSTORE address value)
 = (SETF (RISCOS:MEMORYREF address) value) */
LISPFUNN(riscos_memorystore,2)
{
	var object value = popSTACK();
	var volatile uint8 *address = (volatile uint8*)I_to_uint32( popSTACK() );

	*address = I_to_uint8(value);
	VALUES1(NIL);
}
