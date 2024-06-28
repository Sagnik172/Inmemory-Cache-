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


#endif