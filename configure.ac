dnl Process this file with autoconf to produce a configure script.
AC_INIT([passgen],[1.2.1])
AC_CONFIG_SRCDIR([src/passgen.c])
AM_INIT_AUTOMAKE

dnl Set of available languages.
ALL_LINGUAS="en"

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

AC_SUBST(PKG_CPPFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(PKG_CFLAGS)
AC_SUBST(PKG_FFLAGS)

dnl Where to put passgen.conf
if test [ \"NONE\" == \"$prefix\" ]; then
AC_SUBST(CONFDIR, "/etc")
AC_ARG_WITH(conf,
	AS_HELP_STRING([--with-conf=FILE],[where config file lives [[/etc]]]),
	CONF="${withval}")
else
	AC_SUBST(CONFDIR, "$prefix/etc")
	AC_ARG_WITH(conf,
        AS_HELP_STRING([--with-confdir=DIR],[where config file lives [[/etc]]]),
	CONFDIR="${withval}")
fi
AC_MSG_NOTICE([where to store config... $CONFDIR])
AC_DEFINE_UNQUOTED(CONFDIR, "$CONFDIR", [Where to store config.])
AC_DEFINE_UNQUOTED(CONF, "$CONFDIR/passgen.conf", [Where to find passgen.conf.])

dnl Force autotools to put the exectuables where the user wants them
if test [ \"NONE\" == \"$prefix\" ]; then
	AC_SUBST(BINDIR, "/usr/bin")
else
	AC_SUBST(BINDIR, "$prefix/bin")
fi

dnl Force autotools to put the man files where the user wants them
if test [ \"NONE\" == \"$prefix\" ]; then
	AC_SUBST(mandir, "/usr/share/man")
else
	AC_SUBST(mandir, "$prefix/share/man")
fi

dnl Files to process
AC_CONFIG_FILES([Makefile etc/passgen.conf etc/Makefile src/Makefile doc/Makefile])
AC_OUTPUT
