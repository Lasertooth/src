/* $OpenBSD: groupaccess.c,v 1.9 2006/07/02 22:45:59 stevesk Exp $ */
/*
 * Copyright (c) 2001 Kevin Steves.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "includes.h"

#include <sys/types.h>

#include <grp.h>

#include "groupaccess.h"
#include "xmalloc.h"
#include "match.h"
#include "log.h"

static int ngroups;
static char *groups_byname[NGROUPS_MAX + 1];	/* +1 for base/primary group */

/*
 * Initialize group access list for user with primary (base) and
 * supplementary groups.  Return the number of groups in the list.
 */
int
ga_init(const char *user, gid_t base)
{
	gid_t groups_bygid[NGROUPS_MAX + 1];
	int i, j;
	struct group *gr;

	if (ngroups > 0)
		ga_free();

	ngroups = sizeof(groups_bygid) / sizeof(gid_t);
	if (getgrouplist(user, base, groups_bygid, &ngroups) == -1)
		logit("getgrouplist: groups list too small");
	for (i = 0, j = 0; i < ngroups; i++)
		if ((gr = getgrgid(groups_bygid[i])) != NULL)
			groups_byname[j++] = xstrdup(gr->gr_name);
	return (ngroups = j);
}

/*
 * Return 1 if one of user's groups is contained in groups.
 * Return 0 otherwise.  Use match_pattern() for string comparison.
 */
int
ga_match(char * const *groups, int n)
{
	int i, j;

	for (i = 0; i < ngroups; i++)
		for (j = 0; j < n; j++)
			if (match_pattern(groups_byname[i], groups[j]))
				return 1;
	return 0;
}

/*
 * Free memory allocated for group access list.
 */
void
ga_free(void)
{
	int i;

	if (ngroups > 0) {
		for (i = 0; i < ngroups; i++)
			xfree(groups_byname[i]);
		ngroups = 0;
	}
}
