#include "BException.h"

int BException::m_nestLevel=0;
jmp_buf BException::m_lastBuffer;
int BException::m_code=0;

