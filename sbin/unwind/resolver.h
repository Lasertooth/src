/*	$OpenBSD: resolver.h,v 1.2 2019/01/27 12:40:54 florian Exp $	*/

/*
 * Copyright (c) 2018 Florian Obser <florian@openbsd.org>
 * Copyright (c) 2004, 2005 Esben Norby <norby@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

enum unwind_resolver_state {
	DEAD,
	UNKNOWN,
	RESOLVING,
	VALIDATING
};

static const char * const	unwind_resolver_state_str[] = {
	"dead",
	"unknown",
	"resolving",
	"validating"
};

enum unwind_resolver_type {
	RECURSOR,
	FORWARDER,
	STATIC_FORWARDER,
	STATIC_DOT_FORWARDER,
	RESOLVER_NONE = -1
};

static const char * const	unwind_resolver_type_str[] = {
	"recursor",
	"dhcp forwarder",
	"static forwarder",
	"DoT forwarder"
};

static const int64_t		histogram_limits[] = {
	-1,
	10,
	20,
	40,
	60,
	80,
	100,
	200,
	400,
	600,
	800,
	1000,
	INT64_MAX,
};

struct ctl_resolver_info {
	enum unwind_resolver_state	 state;
	enum unwind_resolver_type	 type;
	int				 selected;
};

void		 resolver(int, int);
int		 resolver_imsg_compose_frontend(int, pid_t, void *, uint16_t);
