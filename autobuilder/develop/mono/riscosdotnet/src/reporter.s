#include "macros.h"
#include "swis.h"

	.text

	@ void rdn_Report_Text0
	NAME	"rdn_Report_Text0"
	.global rdn_Report_Text0
rdn_Report_Text0:
	SWI	SWI_Report_Text0
	MOV	pc, lr

	.end
