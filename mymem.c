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
int lastTimeFree = 0;
void* wrapPointer = NULL;
int wrapAround = 0;

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

    // If myFree() ran before
    lastTimeFree = 0;

    //If no element yet allocated, then set current element to head
        if (current == NULL){
            current = head;
        }
        //Go through the list
        while (current != NULL) {
            sizeLeft = current->size - requested;

            //If current element does not make the requirements to be allocated
            if (current->size < requested || current->alloc == 1) {
                //If end of list have been reached, set the element to the first
                if (current->next == NULL){

                    printf("Current before %d\n",current->ptr);
                    wrapPointer = current->ptr;
                    current = head;
                    printf("next->ptr: %d\n",next->ptr);

                    printf("Current %d head-> next %d\n",current->ptr, head->next->ptr);
                } else current = current->next;
                //If there is size left, start allocation that block
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

	// If head is not empty, clear it
	if(head!=NULL) free(head);

	myMemory = malloc(sz);

    /* TODO: Initialize memory management structure. */

    //Initialize head
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



    //Initalize temp memoryList for searching
    struct memoryList *temp = head;

    //If myFree() were just called, then continue from last element
    if (lastTimeFree == 1) {
        temp = current;
    }

    //While the memoryList is not equal to the block you wish to free, jump to next
    while ((temp != NULL && temp->ptr != block)) {
        temp = temp->next;
        /*if (temp->ptr == block){
            break;
        }
         */

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
        current = temp->next;
       /* if (current == NULL){
            current = temp;
            current->ptr = current->ptr + current->size-1;
        } else
        current->ptr = current->ptr + current->last->size-1;
*/


    }
    lastTimeFree = 1;
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
    struct memoryList* temp = head;

    while (temp!= NULL){
        if (temp->ptr == ptr){
            if (temp->alloc == 1){
                return 1;
            }
            else
                return 0;
        }
        temp = temp->next;
    }
    return 0;
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
     printf("%d(%d) ptr(%d) ",temp->size,temp->alloc, temp->ptr);
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
    if (argc > 1)
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
        int correct_holes = 0;
        int correct_alloc = 100;
        int correct_largest_free = 0;
        int i;

        void* lastPointer = NULL;
        initmem(strategy,100);
        for (i = 0; i < 100; i++)
        {
            void* pointer = mymalloc(1);
            if ( i > 0 && pointer != (lastPointer+1) )
            {
                printf("Allocation with %s was not sequential at %i; expected %p, actual %p\n", strategy_name(strategy), i,lastPointer+1,pointer);
            }
            lastPointer = pointer;
        }

        for (i = 1; i < 100; i+= 2)
        {
            myfree(mem_pool() + i);
        }

        for (i = 1; i < 100; i+=2)
        {
            void* pointer = mymalloc(1);
            if ( i > 1 && pointer != (lastPointer+2) )
            {
                printf("Second allocation with %s was not sequential at %i; expected %p, actual %p\n", strategy_name(strategy), i,lastPointer+1,pointer);
            }
            lastPointer = pointer;
        }

        if (mem_holes() != correct_holes)
        {
            printf("Holes not counted as %d with %s\n", correct_holes, strategy_name(strategy));
        }

        if (mem_allocated() != correct_alloc)
        {
            printf("Memory not reported as %d with %s\n", correct_alloc, strategy_name(strategy));
        }

        if (mem_largest_free() != correct_largest_free)
        {
            printf("Largest memory block free not reported as %d with %s\n", correct_largest_free, strategy_name(strategy));
        }

    }
    print_memory();
    print_memory_status();



}
