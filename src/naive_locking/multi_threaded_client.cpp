#include"fullcsbtree2.h"
#include<pthread.h>
#include<unistd.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
FullCSBTree *root;

void* writer(void *arg)
{
    usleep(10);
    root->insert(10);
    root->insert(5);
    root->insert(2);
    root->insert(1);
    root->insert(30);
    root->insert(20);
    root->insert(25);
}

void* reader(void *arg)
{
    cout<<root->search(10)<<endl;
    cout<<root->search(5)<<endl;
    cout<<root->search(30)<<endl;
    cout<<root->search(25)<<endl;
    usleep(500);
    cout<<root->search(25)<<endl;
}

int main(int argc, char **argv)
{
    root = new FullCSBTree(1);
    pthread_t tid[2];
    int error;
    error = pthread_create(&(tid[0]), NULL, &writer, NULL);
    if(error != 0) {
        cout<<"Thread 1 can't be created : %s"<<strerror(error)<<endl;
        return 1;
    }
    error = pthread_create(&(tid[1]), NULL, &reader, NULL);
    if(error != 0) {
        cout<<"Thread 2 can't be created : %s"<<strerror(error)<<endl;
        return 1;
    }
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL); 
    return 0;
}
