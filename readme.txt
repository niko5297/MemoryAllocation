I have implemented Next-Fit memory allocation strategy








Answer the following questions as part of your report
=====================================================

1) Why is it so important that adjacent free blocks not be left as such?  What
would happen if they were permitted?

2) Which function(s) need to be concerned about adjacent free blocks?

3) Name one advantage of each strategy.

4) Run the stress test on all strategies, and look at the results (tests.out).
What is the significance of "Average largest free block"?  Which strategy
generally has the best performance in this metric?  Why do you think this is?

5) In the stress test results (see Question 4), what is the significance of
"Average number of small blocks"?  Which strategy generally has the best
performance in this metric?  Why do you think this is?

6) Eventually, the many mallocs and frees produces many small blocks scattered
across the memory pool.  There may be enough space to allocate a new block, but
not in one place.  It is possible to compact the memory, so all the free blocks
are moved to one large free block.  How would you implement this in the system
you have built?

7) If you did implement memory compaction, what changes would you need to make
in how such a system is invoked (i.e. from a user's perspective)?

8) How would you use the system you have built to implement realloc?  (Brief
explanation; no code)

9) Which function(s) need to know which strategy is being used?  Briefly explain
why this/these and not others.

10) Give one advantage of implementing memory management using a linked list
over a bit array, where every bit tells whether its corresponding byte is
allocated.