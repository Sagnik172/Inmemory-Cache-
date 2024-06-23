

#ifndef CACHE_FACTORY_HPP
#define CACHE_FACTORY_HPP
#include "lru_cache_policy.hpp"
#include "cache_policy.hpp"

template<typename Key>
class CacheFactory{

    public:
    CacheFactory() = default;
    ~CacheFactory() = default;
    static ICachePolicy<Key> * getPolicyInstance(std::string policyname)
    {
        if(policyname=="LRUCache")
            return new LRUCachePolicy<Key>();
        return nullptr;
    }

};

#endif