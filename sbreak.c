/*
 * AT&T Unix 7th Edition memory allocation routines.
 *
 * Modified for ex by Gunnar Ritter, Freiburg i. Br., Germany,
 * July 2000.
 *
 * Copyright(C) Caldera International Inc. 2001-2002. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   Redistributions of source code and documentation must retain the
 *    above copyright notice, this list of conditions and the following
 *    disclaimer.
 *   Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *   All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed or owned by Caldera
 *      International, Inc.
 *   Neither the name of Caldera International, Inc. nor the names of
 *    other contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * USE OF THE SOFTWARE PROVIDED FOR UNDER THIS LICENSE BY CALDERA
 * INTERNATIONAL, INC. AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL CALDERA INTERNATIONAL, INC. BE
 * LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef	VMUNIX

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"
#include "ex.h"

#ifdef	LANGMSG
#include <nl_types.h>
extern	nl_catd	catd;
#else
#define	catgets(a, b, c, d)	(d)
#endif

#define	POOL	32768

char *pool = NULL;
intptr_t ps = 0;

char *
poolsbrk(intptr_t unused)
{
	intptr_t os, ns;

	if (pool == NULL) {
		if ((pool = malloc(POOL)) == NULL)
			error(catgets(catd, 1, 241,
				"No memory pool"));
		ps = POOL;
	}
	return NULL;
}

void
shiftptr(size_t incr)
{
	fendcore = (line *) ((char *)fendcore + incr);
	endcore = (line *) ((char *)endcore + incr);
	addr1 = (line *) ((char *)addr1 + incr);
	addr2 = (line *) ((char *)addr2 + incr);
	dol = (line *) ((char *)dol + incr);
	dot = (line *) ((char *)dot + incr);
	one = (line *) ((char *)one + incr);
	truedol = (line *) ((char *)truedol + incr);
	unddol = (line *) ((char *)unddol + incr);
	zero = (line *) ((char *)zero + incr);
}

void *
sbreak(size_t incr)
{
	char *oldpool = pool;

	if (incr == 0)
		return pool + ps;
	if ((pool = realloc(pool, ps + incr)) == NULL)
		return (void *)-1;
	ps += incr;
	if (pool != oldpool)
		shiftptr(pool - oldpool);
	return pool;
}

#endif /* VMUNIX */
