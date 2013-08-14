AM_CFLAGS =
AM_CXXFLAGS = 
AM_LDFLAGS = 

lib_LTLIBRARIES = libthreadpooltimer.la

libthreadpooltimer_la_SOURCES = \
	     timer.cpp \
	     timer_impl.cpp \
	     timer_run_loop.cpp \
	     thread_pool_timer_exception.cpp 

libthreadpooltimer_la_CXXFLAGS = -fPIC -std=c++11
libthreadpooltimer_la_CFLAGS = -fPIC

# Test program
noinst_PROGRAMS = threadpooltimertest
threadpooltimertest_SOURCES = threadpooltimertest.cpp 
threadpooltimertest_CXXFLAGS = -std=c++11 
threadpooltimertest_LDADD = libthreadpooltimer.la

include_HEADERS = tp_timer.hpp thread_pool_timer_exception.hpp
