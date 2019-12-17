### To compile code for single-threaded client -

g++ single_client.cpp -o single_client

### To run code for single-threaded client -

./single_client


### To compile multi-threaded client without locking -

g++ multi_threaded_client.cpp -o multi_threaded_client -lpthread


### To compile multi-threaded client with locking -
g++ multi_threaded_client.cpp -o multi_threaded_client -lpthread -DNAIVE_LOCKING


### To run code for multi-threaded client -
./multi_threaded_client
