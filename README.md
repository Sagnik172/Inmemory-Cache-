# In-Memory Cache

This is a generic in-memory cache implementation in C++ that supports different cache policies such as LRU, FIFO, LIFO and LFU. It is thread-safe and can be used with various key and value types.

## Features

- **Generic Cache**: Supports any key and value types.
- **Thread-Safe**: Uses `pthread` for thread synchronization.
- **Flexible Policies**: Can be used with different cache eviction policies like LRU, FIFO, and LFU.


## Usage

### Include the Cache

Ensure you have the necessary header files in your project:

```cpp
#include "cache.hpp"
#include <pthread.h>
#include <unistd.h>
```

### Initialize the Cache

Suppose we want to initialize an in-memory cache consisting of [int,string] key-value pairs with a maximum capacity of 100, and using FIFO as its eviction policy.

This can be done using:
```cpp
constexpr std::size_t CACHE_SIZE = 100;
fixed_sized_cache<int, std::string> fifo_cache(CACHE_SIZE, "FIFO");
```
### Put and Get operations

```cpp
int main() {
    constexpr std::size_t CACHE_SIZE = 100;
    fixed_sized_cache<int, std::string> fifo_cache(CACHE_SIZE, "FIFO");
    //for writing entries
    fifo_cache.Put(1, "one");
    fifo_cache.Put(2, "two");
    fifo_cache.Put(3, "three");

    try {
    //for reading entries
        auto value = fifo_cache.Get(1);
        std::cout << "Value for key 1: " << *value << std::endl;
    } catch (const std::range_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
```
### Running the code
In order to run the code, use the following command:
```
g++ -o output_executable main.cpp -pthread
```
Then, execute the output_executable.exe file.

### Contact
For any questions or suggestions, feel free to contact me at sagnikdas23012002@gmail.com.
