#include<unordered_map>
#include<list>
#include<utility>
using namespace std;
class LRUCache
{
    using pr = pair<int, int>;
    using itr = list<pr>::iterator;
    using hash=unordered_map < int, itr >;
    hash cachemap;
    list<pr> cache;
    int cap;
  public:
    LRUCache(int capacity):cap(capacity)
    {
    }

    int get(int key)
    {
        if (cap == 0)return -1;
        if (cachemap.count(key))//MTF
        {
            auto itr = cachemap[key];
            auto value = *itr;
            cache.erase(itr);
            cache.push_front(value);
            cachemap[key] = cache.begin();
            return value.second;
        }
        else return -1;
    }

    void put(int key, int value)
    {
        if (cap == 0)return;
        if (cache.size() < cap || cachemap.count(key))//not full or not new
        {
            if (cachemap.count(key))//exist:MTF
            {
                auto itr = cachemap[key];
                cache.erase(itr);
            }
            cache.push_front(make_pair(key,value));
            cachemap[key] = cache.begin();
        }
        else
        {
            //lru
            cachemap.erase(cache.back().first);
            cache.pop_back();
            put(key, value);
        }
    }
};
