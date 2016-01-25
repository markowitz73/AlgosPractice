//This header is about a simple LRUCache impl, which stores key-value pairs.
//It uses an entry as the cache unit to store data, which at the same time
//work as a node in a circular doubly linked list.When we first create a cache.
//a dummy node containing no data named LRUCache::head_ would act as the 
//start point of this list so we can easily remove or insert node behind
//it. Also, it is much cheaper compared to std::list because we don't need
//that many functions. 
//Besides, We use an unordered_map for quick hashing to get the entry pointer
//thus we no longer have to iterate the list to find the entry we need.

#include <unordered_map>
#include <utility>
#include <cstdio>


//TODO
//Wrap entries with simple idiom RAII aka: refs counting & auto release
template <typename K, typename V>
struct _entry{
    _entry() {}
    _entry(K key, V value) : key_(key), value_(value) {}
    _entry<K, V>* prev;
    _entry<K, V>* next;
    K key_;
    V value_;
};
template <typename V>
struct _result{
    _result() : valid_(false), value_(nullptr){}
    _result(V* v) : valid_(true), value_(v){}
    inline V Get(){
        return *value_;
    }
    inline bool Valid(){
        return valid_;
    }
    V* value_;
    bool valid_;
};

template<typename K, typename V>
class LRUCache{
public:
    using Result = _result<V>;
    using entry = _entry<K, V>;
    LRUCache(int capacity) : capacity_(capacity), size_(0) {
        head_ = entry();
        head_.prev = &head_;
        head_.next = &head_;
    }
    ~LRUCache() {
        printf("Brief Summary\ntheir are %d entries in list, %d nodes in map\n", size_, entryMap.size());

        entry* e = head_.next;
        while(e != &head_) {
            printf("deleting node key:%d val:%d \n", e->key_, e->value_);
            entry* ref = e;
            e = e->next;
            delete ref;
        }
    } //clean up entries

    Result Get(const K& key) {
        auto it = entryMap.find(key);
        if (it != entryMap.end()) {
            entry* e = it->second;
            RemoveEntry(e);
            AppendEntry(e);
            return Result(&(e->value_));
        }
        return Result();
    }

    void Set(K key, V value) {
        auto it = entryMap.find(key);
        if (it != entryMap.end()) {
            it->second->value_ = value;
            RemoveEntry(it->second);
            AppendEntry(it->second);
        } else {
            printf("we didn't find key in the map\n", key, value);
            if (size_ == capacity_) {
                printf("we are full filled\n");
                entry* old = head_.prev;
                RemoveEntry(old);
                size_--;
                entryMap.erase(old->key_);
                delete old;
            }
            entry* e = new entry(key, value);
            AppendEntry(e);
            size_++;
            entryMap.insert(std::pair<K, entry*>(key, e));
        }
    }

private:
    //not allowed to copy
    LRUCache(LRUCache& c) {}
    void operator=(LRUCache& c) {}
    
    //list operations
    //there's no reason why i returned an entry* here
    void RemoveEntry(entry* e){
        e->next->prev = e->prev;
        e->prev->next = e->next;
    }

    void AppendEntry(entry* e){
        e->prev = &head_;
        e->next = head_.next;
        head_.next->prev = e;
        head_.next = e;
    }

    //data members
    std::unordered_map<K, entry*> entryMap;
    entry head_;
    K capacity_;
    V size_;
};

