/* vi: set sw=4 ts=4: */
/*
 * repeat implementation for busybox
 *
 * Copyright (C) 2003  Manuel Novoa III  <mjn3@codepoet.org>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

/* BB_AUDIT SUSv3 compliant */
/* http://www.opengroup.org/onlinepubs/007904975/utilities/cat.html */

//kbuild:lib-$(CONFIG_REPEAT)     += repeat.o
//applet:IF_REPEAT(APPLET(repeat, BB_DIR_USR_BIN, BB_SUID_DROP))

//config:config REPEAT
//config:	bool "repeat"
//config:	default y
//config:	help
//config:	  repeat is used to read the contens of a file to stdout as a loop

//usage:#define repeat_trivial_usage
//usage:       "[FILE]"
//usage:#define repeat_full_usage "\n\n"
//usage:       "open FILE and print to stdout as a loop"
//usage:
//usage:#define repeat_example_usage
//usage:       "$ repeat /proc/uptime\n"
//usage:       "110716.72 17.67"
//usage:       "110716.72 17.67"

#include "libbb.h"

/* This is a NOFORK applet. Be very careful! */


int bb_repeat(char **argv)
{
	int fd;
	int retval = EXIT_SUCCESS;

	if (!*argv)
		argv = (char**) &bb_argv_dash;

	fd = open_or_warn_stdin(*argv);
	if (fd >= 0) {
		/* This is not a xfunc - never exits */
		off_t r;
		do {
			r = bb_copyfd_eof(fd, STDOUT_FILENO);
			lseek(fd, 0, SEEK_SET);
		} while(r >= 0);
	}
	retval = EXIT_FAILURE;

	return retval;
}

int repeat_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int repeat_main(int argc UNUSED_PARAM, char **argv)
{
	getopt32(argv, "u");
	argv += optind;
	return bb_repeat(argv);
}
