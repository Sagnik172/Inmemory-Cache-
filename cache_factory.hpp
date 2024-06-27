#ifndef CACHE_FACTORY_HPP
#define CACHE_FACTORY_HPP
#include "lru_cache_policy.hpp"
#include "cache_policy.hpp"
#include "lfu_cache_policy.hpp"
#include "fifo_cache_policy.hpp"
template<typename Key>
class CacheFactory{

    public:
    CacheFactory() = default;
    ~CacheFactory() = default;
    static ICachePolicy<Key> * getPolicyInstance(std::string policyname)
    {
        if(policyname=="LRU")
            return new LRUCachePolicy<Key>();
        else if(policyname=="LFU")
            return new LRUCachePolicy<Key>();
        else if(policyname=="FIFO")
            return new FIFOCachePolicy<Key>();
        return new LRUCachePolicy<Key>();
    }

};

#endif