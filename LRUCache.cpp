#include <unordered_map>
#include <utility>
#include <cstdio>

struct entry{
    entry() {}
    entry(int key, int value) : key_(key), value_(value) {}
    entry* prev;
    entry* next;
    int key_;
    int value_;
};

class LRUCache{
public:
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
    int Get(int key) {
        auto it = entryMap.find(key);
        if (it != entryMap.end()) {
            entry* e = RemoveEntry(it->second);
            AppendEntry(e);
            return it->second->value_;
        }
        return -1;
    }
    void Set(int key, int value) {
        auto it = entryMap.find(key);
        if (it != entryMap.end()) {
            printf("key:%d val:%d found in entryMap!\n", key, value);
            it->second->value_ = value;
            RemoveEntry(it->second);
            AppendEntry(it->second);
        } else {
            printf("we didn't find key:%d val:%d in map\n", key, value);
            if (size_ == capacity_) {
                printf("we are full filled\n");
                entry* old = RemoveEntry(head_.prev);
                size_--;
                entryMap.erase(old->key_);
                delete old;
            }
            entry* e = new entry(key, value);
            AppendEntry(e);
            size_++;
            entryMap.insert(std::pair<int, entry*>(key, e));
        }
    }
private:
    //not allowed to copy
    LRUCache(LRUCache& c) {}
    void operator=(LRUCache& c) {}
    
    //list operations
    entry* RemoveEntry(entry* e){
        printf("removing entry which key:%d val:%d\n", e->key_, e->value_);
        e->next->prev = e->prev;
        e->prev->next = e->next;
        return e;
    }

    void AppendEntry(entry* e){
        printf("appending entry which key:%d val:%d\n", e->key_, e->value_);
        e->prev = &head_;
        e->next = head_.next;
        head_.next->prev = e;
        head_.next = e;
    }

    //data members
    std::unordered_map<int, entry*> entryMap;
    entry head_;
    int capacity_;
    int size_;
};

int main() {
    LRUCache c(5);
    c.Set(2, 1);
    c.Set(3, 5);
    c.Set(3, 6);
    printf("stop\n");
    printf("try to find a key 3 val is %d:", c.Get(3));
    printf("here we created a lrucache\n");
    return 0;
}
