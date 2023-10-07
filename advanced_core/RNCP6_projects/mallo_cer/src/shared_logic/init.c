#include <mallo_cer.h>

pthread_mutex_t		mallo_cer_mutex = PTHREAD_MUTEX_INITIALIZER;

t_pile*		g_pile_anchor = NULL;
