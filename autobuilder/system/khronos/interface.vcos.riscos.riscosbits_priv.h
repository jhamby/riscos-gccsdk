/*
 * Copyright (c) 2012, RISC OS Open Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of RISC OS Open Ltd nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RISCOSBITS_PRIV_H
#define RISCOSBITS_PRIV_H

/* Macros and other stuff that we don't need to make public in the headers.  */

#include <stdint.h>
#include "riscosbits.h"

#define INIT_LIST_HEAD(HEAD) do { (HEAD)->next = (HEAD)->prev = HEAD; } while(0)
#define list_for_each_safe(POS,NEXT,HEAD) for((POS)=(HEAD)->next,(NEXT)=(POS)->next;(POS) != (HEAD);(POS)=(NEXT),(NEXT)=(POS)->next)
#define list_for_each(POS,HEAD) for((POS)=(HEAD)->next;(POS) != (HEAD);(POS)=(POS)->next)
#define list_entry(ITEM,TYPE,MEMBER) ((TYPE *) (((int)(ITEM)) - offsetof(TYPE,MEMBER)))
#define list_del(POS) do { (POS)->next->prev = (POS)->prev; (POS)->prev->next = (POS)->next; (POS)->next = (POS)->prev = (POS); } while(0)
#define list_add(ITEM,HEAD) do { (ITEM)->next = (HEAD)->next; (ITEM)->prev = (HEAD); (HEAD)->next = (ITEM); (ITEM)->next->prev = (ITEM); } while(0)

struct key {
    struct __vcos_list_head link;
    uint32_t key;
    uint32_t value;
};

#endif
