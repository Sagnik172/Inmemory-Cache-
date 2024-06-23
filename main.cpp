#include <iostream>
#include "cache.hpp"
#include "lru_cache_policy.hpp"
#include<iostream>
using namespace std;
int main(){
  constexpr std::size_t CACHE_SIZE = 256;
  fixed_sized_cache<std::string,int> cache(CACHE_SIZE);

  cache.Put("Hello", 1);
  cache.Put("world", 2);
  
  const auto hello_value = cache.Get("Hello");
  const auto world_value = cache.Get("world");

  std::cout << *hello_value << *world_value << '\n';
  return 0;
}