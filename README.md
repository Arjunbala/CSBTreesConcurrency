# CSBTreesConcurrency
This project explores concurrency control mechanisms in CSB+-trees.

# Code Details

There are 3 implementations in this repo under the src/ folder
1. Vanilla Implementation - This implements single threaded CSB+-Trees without any locking
2. Naive Locking - This implements concurrency control in CSB+-Trees using coarse-grained locking
3. Efficient Locking - This implements concurrency control in CSB+-Trees using fine-grained locking and optimistic concurrency control

We provide a header file for each of the implementations. Please have a look at the respective folders in the src/ folder for instructions on how to run sample code and for sample API usage. 
