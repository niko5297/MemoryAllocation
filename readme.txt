I have implemented Next-Fit memory allocation strategy using the listed material.

Next-fit algorithm is like first-fit algortihm, it allocates the first element in the list,
that has the required space and haven't already been allocated.
One of the difference between first-fit and next-fit, is that next-fit continues from the last allocated or freed element.
When next-fit algorithm reaches end of list, the algorithm wraps around to the start again.

To run the program simply open terminal or command prompt and find this working directory.

Then type: "make"
And follow it up with: "./mem -try next"

This command runs try_mem method. To run the tests, simply type: "./mem -test -f0 all next" or "./mem -test all next"