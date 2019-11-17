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
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;


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
        int sizeBeforeAllocation = head->size;
        next = (struct memoryList *) malloc(requested);
        next->size = requested;
        /*
        next->last = next;
        next->next = next->next;

        if (next->next!=NULL){
            //Explain
            next->next->last = next;
            next->next = next;
        }

        next->size = head->size-requested;
        next->ptr = head->ptr+requested;
        next->alloc = 1;
        head->alloc = 0;
    return head->ptr;
         */



        if (head==NULL){
            head = next;
            return next;
        }
        head->last = next;
        next->next = head;
        next->alloc = 1;

        //Moves head forward;
        head = next;

        //FIXME: Hvorfor fungere det ikke når denne kører?
        //head->size = sizeBeforeAllocation-next->size;



    return next->ptr;


}

int isForwardBlockFree(void* block){
    struct memoryList* temp = next;
    temp->ptr = block;
    if (block!=NULL) {
        if (temp->next->alloc == 0) {
            return 1;
        } else return 0;
    }
}

int isBackwardBlockFree(void* block){
    struct memoryList* temp = next;
    temp->ptr = block;
    if (block!=NULL) {
        if (temp->last->alloc == 0) {
            return 1;
        } else return 0;
    }
}

void* mergeForward(struct memoryList* temp){

    if (temp==NULL){
        return NULL;
    }

    /*
     * block.next = next.next;
     * block.size += next.size;
     * free(next);
     */
    temp->next = next->next;
    temp->size += next->size;
    free(next);

}

void* mergeBackward(struct memoryList* temp){

    if (temp==NULL){
        return NULL;
    }

    /*
     * block.last = last.last;
     * block.size += last.size;
     * block.ptr = last.ptr;
     * free(last);
     */

    temp->last = next->last;
    temp->size += next->last;
    temp->ptr = next->last->ptr;
    free(next);
}

void freeMem(){

    //TODO: Hvis initmem køres igen, så skal alt memory wipes. Dvs. du skal køre igennem alt memory og benytte free
    while (head!=NULL) {
        free(head->ptr);
        head = head->next;
        if (head->next==NULL){
            free(head->ptr);
            head = head->last;
        }
    }

}

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	if (head!=NULL) freeMem();

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
	            if (requested<=head->size /*&& head->alloc==0*/) {
                    return nextFit(requested);
                } else {
                    return "ERROR: Not enough memory";
	            }
	  }
	return NULL;
}



/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{

    /*
     * Find element
     * Set Alloc
     * Merge block
     */

    /*
     * Mergeblock(block)
     * Check Alloc
     *  if nextFree(block)
     *      newBlock = merge()
     *      Mergeblock(newBlock)
     *  else if behindFree(block)
     *      newBlock = merge()
     *      Mergeblock(newBlock)
     *  return;
     */

    struct memoryList* temp = head;
    while (temp->ptr!=block){
        temp = temp->next;
    }


    temp->alloc = 0;

    //TODO: Comment what following line is doing

    if ((temp->last != NULL) && temp->last->alloc==0){
        //mergeBackward(temp);
        temp->last->size += temp->size;
        struct memoryList* temp2 = temp;
        if (temp2->last !=NULL) {
            temp2->last->ptr = temp2->next;
            temp2->next->ptr = temp2->last;
        }

        printf("Freeing %d\n",temp2->size);
        free(temp2);

    }


    //TODO: Comment what following line is doing
    if ((temp->next != NULL) && temp->next->alloc==0){
        //mergeForward(temp);
        temp->next->size += temp->size;
        struct memoryList* temp2 = temp->next;
       //struct memoryList* temp2 = temp;

        if (temp2->next!= NULL){

            temp2->next->ptr = temp2->last;
            temp2->last->ptr = temp2->next;

        }
        printf("Freeing %d\n",temp2->size);
        printf("Headsize before %d\n",head->size);
        free(temp2);
        printf("Headsize after %d\n",head->size);


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
    int tempFree = mySize;
    while(temp != NULL) {
        if (temp->alloc==0){



        }
        temp = temp->next;
    }
    return sizeFree;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
    //TODO: Same logic as above just instead of max compare with size
	return 0;
}       

char mem_is_alloc(void *ptr)
{


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
    while(temp != NULL) {
        printf("Size: %d\n",temp->size);
        temp = temp->next;
    }
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
	
	initmem(strat,500);
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	
	print_memory();
	print_memory_status();
	
}
