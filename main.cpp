#include <iostream>
#include "cache.hpp"
#include <pthread.h>
#include<iostream>
#include <unistd.h> 
#include<vector>
using namespace std;

void* thread_func_put(void* arg)
{
    auto* params = static_cast<std::pair<fixed_sized_cache<int, std::string>*, std::pair<int, std::string>>*>(arg);
    params->first->Put(params->second.first, params->second.second);
    // cout<<params->second.first<<endl;
    // std::cout << "Put: (" << params->second.first << ", " << params->second.second << ")" << std::endl;
    return nullptr;
}

void* thread_func_get(void* arg)
{
    auto* params = static_cast<std::pair<fixed_sized_cache<int, std::string>*, int>*>(arg);
    try
    {
        auto value = params->first->Get(params->second);
        // std::cout << "Get: (" << params->second << ", " << *value << ")" << std::endl;
    }
    catch (const std::range_error& e)
    {
        // std::cout << "Get: (" << params->second << ") - " << e.what() << std::endl;
    }
    return nullptr;
}

int main(){
  constexpr std::size_t CACHE_SIZE = 3;
  std::cout<<"Enter choice: "<<std::endl;
  int ch;
  cin>>ch;
  if(ch <= 0 or ch > 4)
    ch=1;
  ch--;
  vector<string> choices = {"LRU", "LFU" , "FIFO", "LIFO"};
  fixed_sized_cache<int, std::string> cache(CACHE_SIZE, choices[ch]);

    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9;

    auto params1 = std::make_pair(&cache, std::make_pair(1, string("one")));
    auto params2 = std::make_pair(&cache, std::make_pair(2, string("two")));
    auto params3 = std::make_pair(&cache, std::make_pair(3, string("three")));
    auto params4 = std::make_pair(&cache, std::make_pair(1, string("one")));
    auto params5 = std::make_pair(&cache, 1);
    auto params6 = std::make_pair(&cache, 2);
    auto params7 = std::make_pair(&cache, 3);
    auto params8 = std::make_pair(&cache, 4);
    auto params9 = std::make_pair(&cache, std::make_pair(4, string("four")));


    // cout<<params1.second.first<<endl;
    pthread_create(&t1, nullptr, thread_func_put, &params1);
    pthread_create(&t2, nullptr, thread_func_put, &params2);
    pthread_create(&t3, nullptr, thread_func_put, &params3);
    pthread_create(&t4, nullptr, thread_func_put, &params4);
    pthread_create(&t9, nullptr, thread_func_put, &params9);


    std::cout << "Sleeping for 3 seconds..." << std::endl;
    sleep(3);

    pthread_create(&t5, nullptr, thread_func_get, &params5);
    pthread_create(&t6, nullptr, thread_func_get, &params6);
    pthread_create(&t7, nullptr, thread_func_get, &params7);
    pthread_create(&t8, nullptr, thread_func_get, &params8);

    

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);
    pthread_join(t5, nullptr);
    pthread_join(t6, nullptr);
    pthread_join(t7, nullptr);
    pthread_join(t8, nullptr);

    // std::cout<<"Hi theere"<<endl;

    return 0;
}