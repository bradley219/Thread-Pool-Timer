# Flag defaults
CXXFLAGS='-O2 '
CFLAGS='-O2 '

AC_PREREQ([2.67])
AC_INIT(ThreadPoolTimer, 1.0.0, snyder.bradleyj@gmail.com)
AM_INIT_AUTOMAKE(ThreadPoolTimer, 1.0.0)
AM_PATH_CPPUNIT(1.9.6)

LT_INIT([disable-shared])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([libpthread not found])])
AC_CHECK_LIB([rt], [clock_gettime], [], [])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_OUTPUT(Makefile src/Makefile)
