/*
 *
 * 		Atollic TrueSTUDIO Minimal System calls file
 *
 * 		For more information about which c-functions
 * 		need which of these lowlevel functions
 * 		please consult the Newlib libc-manual
 *
 * */
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#undef errno
extern int errno;

#define asm __asm

register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;

void initialise_monitor_handles(void)
{
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}		/* Make sure we hang here */
}



caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;
	caddr_t ret = (caddr_t)-1;

	if (heap_end == 0)
		heap_end = &end;

	if (heap_end + incr > stack_ptr)
	{
//		write(1, "Heap and stack collision\n", 25);
//		abort();
		errno = ENOMEM;
	}
	else
	{
		prev_heap_end = heap_end;
		ret = (caddr_t)prev_heap_end;
		heap_end += incr;
	}

	return ret;
}

int _close(int file)
{
	return -1;
}


int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	//from the serial port.
	*ptr=(char)fgetc((FILE*)file); //define in spp layer
	return (1);
}

int _write(int file, char *ptr, int len)
{

	return len;
}

int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}
