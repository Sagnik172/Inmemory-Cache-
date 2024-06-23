#ifndef CACHE_POLICY_HPP
#define CACHE_POLICY_HPP
template <typename Key>
class ICachePolicy
{
  public:
    virtual ~ICachePolicy() = default;
    virtual void Insert(const Key &key) = 0;
    virtual void Move(const Key &key) = 0;
    virtual void Erase(const Key &key) = 0;
    virtual const Key &ReplCandidate() const = 0;
};
// template <typename Key>
// class CacheFactory{
//   CacheFactory() = default;
//   ~CacheFactory() = default;
//   public:
//   ICachePolicy<Key>* getPolicyInstance(std::string s)
//   {
//     if(s=="LRUCachePolicy")
//     {
//       return new LRUCachePolicy<Key>();

//     }
//     return nullptr;
//   }


// };


#endif