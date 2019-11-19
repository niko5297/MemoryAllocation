#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
  // doubly-linked list
  struct memoryList *last;
  struct memoryList *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
};

strategies myStrategy = NotSet;    // Current strategy

size_t mySize;
int sizeLeft;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;
static struct memoryList *current;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void* nextFit (size_t requested){

    /**
     * Skriv lidt om på følgende løsning
     */

        if (current == NULL){
            current = head;
        }

        while (current != NULL) {
            sizeLeft = current->size - requested;

            //current = current->size;
            if (current->size < requested || current->alloc == 1) {
                current = current->next;
            } else if (sizeLeft > 0) {

                next = (struct memoryList *) malloc(sizeof(struct memoryList));
                next->last = current;
                next->next = current->next;
                next->size = sizeLeft;

                if (head->next == NULL){
                    head->next = next;
                }

                current->next = next;
                current->size = requested;
                current->alloc = 1;


             /*   if (current->last != NULL) {
                    current->ptr = current->last->ptr + requested;
                }
*/
                if (current->next!=NULL) {
                    next->ptr = (current->ptr) + requested;
                }

                return current->ptr;
            } else {
                current->alloc = 1;

                return current->ptr;
            }
        }
    return current->ptr;


}

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;
	sizeLeft = sz;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	if(head!=NULL) free(head);

	myMemory = malloc(sz);

    /* TODO: Initialize memory management structure. */

	head = (struct memoryList*) malloc(sizeof(struct memoryList));
	head->last=NULL;
	head->next=NULL;
	head->size=sz; // initially the first block size is equals to the memory pool size.
	head->alloc=0; // not allocated
	head->ptr=myMemory; // points to the same memory address as the memory pool

}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1
 */

void *mymalloc(size_t requested)
{
	assert((int)myStrategy > 0);

	switch (myStrategy)
	  {
	  case NotSet:
	            return NULL;
	  case First:
	            return NULL;
	  case Best:
	            return NULL;
	  case Worst:
	            return NULL;
	  case Next:
          return nextFit(requested);
      }
    return NULL;
}



/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block) {



    struct memoryList *temp = head;

    while (temp != NULL && temp->ptr != block) {
        temp = temp->next;
    }


    if (temp != NULL) {

        temp->alloc = 0;

        if (temp->alloc == 0 && temp->last != NULL && temp->last->alloc == 0) {
            temp->size += temp->last->size;

            struct memoryList *newTemp = temp->last;
            temp->last = newTemp->last;

            if (newTemp->last != NULL) {
                newTemp->next->last = temp;
                free(newTemp);
            }

        }

        //TODO: Comment what following line is doing
        if (temp->alloc == 0 && temp->next != NULL && temp->next->alloc == 0){
            temp->size += temp->next->size;

            struct memoryList *newTemp = temp->next;
            newTemp->alloc = 0;
            temp->next=newTemp->next;

            if (newTemp->next != NULL) {
                newTemp->next->last = temp;
                free(newTemp);

            }




        }

        printf("myMemory %d\n",myMemory);
        current = temp->next;
        //printf("next: %d",current->last->size);
        current->ptr = current->ptr + current->last->size-1;
        //current->ptr = current->last->ptr;


    }
}




/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the
 * memory pool this module manages via initmem/mymalloc/myfree.
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
    struct memoryList* temp = head;
    int nonAllocs = 0;
    while(temp != NULL) {
        if (temp->alloc==0){
            nonAllocs++;
        }
        temp = temp->next;
    }
	return nonAllocs;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
    struct memoryList* temp = head;
    int sizeAllocated = 0;
    while(temp != NULL) {
        if (temp->alloc==1){
            sizeAllocated += temp->size;
        }
        temp = temp->next;
    }
    return sizeAllocated;
}

/* Number of non-allocated bytes */
int mem_free()
{
    struct memoryList* temp = head;
    int sizeFree = mySize;
    while(temp != NULL) {
        if (temp->alloc==1){
            sizeFree -= temp->size;
        }
        temp = temp->next;
    }
    return sizeFree;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
    /**
     * In this function you should think about going through the memoryList and check for longest
     * sequence of unallocated bytes(alloc = 0). Keep track of the largest sequence and update in accordingly.
     */
    struct memoryList* temp = head;
    int sizeFree = 0;
    int tempFree = 0;
    while(temp != NULL) {
        if (temp->alloc==0){
            tempFree = temp->size;
            if (tempFree>sizeFree) sizeFree = tempFree;
        }
        temp = temp->next;
    }
    return sizeFree;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
    //TODO: Same logic as above just instead of max compare with size
    struct memoryList* temp = head;
    int freeCounter = 0;
    while(temp != NULL) {
        if (temp->alloc==0){
            if (temp->size<size) freeCounter++;
        }
        temp = temp->next;
    }
    return freeCounter;
}

char mem_is_alloc(void *ptr)
{
    //TODO: mem_is_alloc tager imod en ptr, hvorefter den tjekker alle dine blocke igennem efter denne ptr, og returnere alloc
    struct memoryList* temp = head;

    while (temp->ptr!=ptr){
        temp = temp->next;
    }
    return temp->alloc;
}

/*
 * Feel free to use these functions, but do not modify them.
 * The test code uses them, but you may find them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy.
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/*
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
    struct memoryList* temp = head;
    printf("NULL <- ");
    while(temp != NULL) {
     printf("%d(%d) ptr(%d)",temp->size,temp->alloc,temp->ptr);
     if (temp->next!=NULL){
         printf(" <-> ");
     }
        temp = temp->next;
    }
    printf(" -> NULL\n");
}

/* Use this function to track memory allocation performance.
 * This function does not depend on your implementation,
 * but on the functions you wrote above.
 */
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;


	/* A simple example.
	   Each algorithm should produce a different layout. */
    strategies strategy;
    int lbound = 1;
    int ubound = 4;

    if (strategyFromString(*(argv+1))>0)
        lbound=ubound=strategyFromString(*(argv+1));

    for (strategy = lbound; strategy <= ubound; strategy++)
    {
        int correct_holes;
        int correct_alloc;
        int correct_largest_free;
        int correct_small;
        void* first;
        void* second;
        void* third;
        int correctThird;

        initmem(strategy,100);

        first = mymalloc(10);
        second = mymalloc(1);
        myfree(first);
        third = mymalloc(1);

        printf("second + 1 : %d\n",second + 1);

        if (second != (first+10))
        {
            printf("Second allocation failed; allocated at incorrect offset with strategy %s\n", strategy_name(strategy));
        }

        correct_alloc = 2;
        correct_small = (strategy == First || strategy == Best);

        switch (strategy)
        {
            case Best:
                correctThird = (third == first);
                correct_holes = 2;
                correct_largest_free = 89;
                break;
            case Worst:
                correctThird = (third == second+1);
                correct_holes = 2;
                correct_largest_free = 88;
                break;
            case First:
                correctThird = (third == first);
                correct_holes = 2;
                correct_largest_free = 89;
                break;
            case Next:
                correctThird = (third == second+1);
                correct_holes = 2;
                correct_largest_free = 88;
                break;
            case NotSet:
                break;
        }

        if (!correctThird)
        {
            printf("Third allocation failed; allocated at incorrect offset with %s", strategy_name(strategy));

        }

        if (mem_holes() != correct_holes)
        {
            printf("Holes counted as %d, should be %d with %s\n", mem_holes(), correct_holes, strategy_name(strategy));

        }

        if (mem_small_free(9) != correct_small)
        {
            printf("Small holes counted as %d, should be %d with %s\n", mem_small_free(9), correct_small, strategy_name(strategy));

        }

        if (mem_allocated() != correct_alloc)
        {
            printf("Memory reported as %d, should be %d with %s\n", mem_allocated(0), correct_alloc, strategy_name(strategy));
        }

        if (mem_largest_free() != correct_largest_free)
        {
            printf("Largest memory block free reported as %d, should be %d with %s\n", mem_largest_free(), correct_largest_free, strategy_name(strategy));
        }

    }

	print_memory();
	print_memory_status();

}
