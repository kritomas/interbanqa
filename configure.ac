AC_INIT([interbanqa], [1.0], [kritomas])
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_PROG_CC

AC_CANONICAL_HOST
if test "$host_os" = "mingw32" || test "$host_os" = "msys"; then
  WINDOWS=true
else
  WINDOWS=false
fi
AM_CONDITIONAL([WINDOWS], [test x$WINDOWS = xtrue])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT