#include "fullcsbtree2.h"
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define FRAC_INSERT_THREADS 1

int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

float get_time_elapsed_sec(struct timeval tv1, struct timeval tv2) {
    struct timeval tvdiff = { tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec };
    if (tvdiff.tv_usec < 0) { tvdiff.tv_usec += 1000000; tvdiff.tv_sec -= 1; }
    return tvdiff.tv_sec + tvdiff.tv_usec/(1000.0*1000.0);
}

void *searchThread(void *arg)
{
    FullCSBTree *root = (FullCSBTree*) arg;
    int i;
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    for(i=0;i<100;i++) {
        root->search(i*random_number(0,10));
    }
    gettimeofday(&tv2, NULL);
    printf("Search throughput = %f keys/sec\n", (100.0)/get_time_elapsed_sec(tv1,tv2));
}

int main(int argc, char **argv)
{
    FullCSBTree *root = new FullCSBTree(1);
    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    int i, rc;

    root->insert(10);
    root->insert(5);
    root->insert(2);
    root->insert(1);
    root->insert(30);
    root->insert(20);
    root->insert(25);
    // Create thread
    for(i=0;i<num_threads;i++) {
	// Decide whether to create search thread or insert thread
	
        rc = pthread_create(&threads[i], NULL, searchThread, (void*)root);
	if(rc) {
	    cout<<"Unable to create thread, error:"<<rc<<endl;
	    exit(-1);
        }
    }

    // Wait for all threads to complete
    for(i=0;i<num_threads;i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
