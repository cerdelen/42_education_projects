#ifndef MALLO_CER_H
#define MALLO_CER_H

#include <stddef.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGESIZE ((size_t) getpagesize())
#define ALIGNMENT ((size_t) 16)
#define SMALL_CHUNK ((size_t) PAGESIZE / 128)
#define BIG_CHUNK ((size_t) SMALL_CHUNK * 8)
#define SMALL_MMAP_SPACE ((size_t) SMALL_CHUNK * 128)
#define BIG_MMAP_SPACE ((size_t) BIG_CHUNK * 128)
#define ALIGNED_SIZEOF_PILE ((size_t) ((sizeof(t_pile) + 15) & ~15))
#define ALIGNED_SIZEOF_CHUNK ((size_t) ((sizeof(t_chunk) + 15) & ~15))


struct pile;
struct chunk;
struct t_pile;

// pool == All memory of this program
// pile == A zone preallocated of which there might be more
// chunks == returns of malloc ... multiple chunks make up one pile
// 

// pile size == multiple of group sizes
// chunk sizes as per subject
// SMALL <= 128 (because 128 * 128 = pagesize and i need to at least have 100 and i need to at least pagesize) 
// BIG <= 1024 (because SMALL * 8)
// HUMUNGOUS >= 1204
//
// resulting into pile sizes of 
// SMALL = 16 kb (128 * 128)
// BIG = 131 kb (128 * 1024)
// HUMUNGOUS = size of chunk
//
//
//somehow i have to actually put meta data infront of a malloced ptr so free knows how long it is
//if smth is freed set this metadata of size to 0 so subsequent calls to this dont allocate smth
//
//
//if free is called on ptr not allocated by malloc undefined behaviour (might resolve in freeing of space not wanted to be freed)
//
// metadata of piles will be prepended to pile so pile is actually pilesize + sizeof(metadata)
// which type of pile
// next pile
// prev pile
//
// not sure how this will work yet somehow have to store where is first free space to use and how much
// used space (so first free space is )
//

/* 
 *  I have a global pointed to a 
 *  ILL have a global ptr to an anchor pile and all else will be a list
 *  (IDEA have 2 anchors, one for SMALL requests and one for BIG requests)
 *
 * Have multiple 
 *
 * */

typedef enum e_chunk_type {
	SMALL,
	BIG,
	HUMUNGOUS,
}	t_chunk_type;


typedef struct s_pile
{
	struct s_pile*		next;
	struct s_pile*		prev;
	struct s_chunk*		first_empty;
	size_t				empty_space;
	t_chunk_type		chunk_type;
}	t_pile;

typedef struct s_chunk
{
	// next two ptrs are NULL if malloced (or if inside freed space)
	// if random ptr is trying to get it it could be freeing stuff which it shouldnt but thats undefined behaviour
	// when freed i set them to next/prev freed "block of chunks"
	struct s_chunk*		next;
	struct s_chunk*		prev;
	size_t				size;	// i use this for free chunks to say how long this free space is and for allocated chunks to tell me how lonkg this allocated space is
	bool				free;
}	t_chunk;


extern pthread_mutex_t			mallo_cer_mutex;
extern t_pile*					g_pile_anchor;

void			free(void *ptr);
void*			malloc(size_t size);
void*			realloc(void *ptr, size_t size);
void			show_alloc_mem();

// PILES
void*			create_new_pile(size_t size, t_chunk_type chunk_type);
void*			create_new_pile_s_or_b(t_chunk_type chunk_type);
void*			create_humungous_new_pile(size_t size);

// CHUNKS
void*			get_chunk_from_specific_pile(size_t size, t_pile *pile);
void*			get_chunk_from_piles(size_t size, t_chunk_type chunk_type);
t_chunk_type	get_chunk_type(size_t size);

// UTILS
int				lock_mutex_wrapper();
int				unlock_mutex_wrapper();
void			ptr_swap(void* ptr1, void* ptr2);

#endif // !MALLO_CER_H
