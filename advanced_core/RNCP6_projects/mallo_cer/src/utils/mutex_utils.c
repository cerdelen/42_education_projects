#include <mallo_cer.h>
#include <stdio.h>

int			lock_mutex_wrapper()
{
	int	err = pthread_mutex_lock(&mallo_cer_mutex);
	if (err)
		printf("Error: Lock Mutex error\n");
	return (err);
}

int			unlock_mutex_wrapper()
{
	int	err = pthread_mutex_unlock(&mallo_cer_mutex);
	if (err)
		printf("Error: Unlock Mutex error\n");
	return (err);
}
