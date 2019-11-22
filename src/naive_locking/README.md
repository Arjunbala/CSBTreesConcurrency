To compile code -

g++ single_client.cpp -o single_client

To run -

./single_client

Multi-threaded client -

Without locking - g++ multi_threaded_client.cpp -o multi_threaded_client -lpthread

With locking - g++ multi_threaded_client.cpp -o multi_threaded_client -lpthread -DNAIVE_LOCKING

./multi_threaded_client
