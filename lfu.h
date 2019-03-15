#include<unordered_map>
#include<list>
#include<utility>
using namespace std;
class LFUCache
{
    class LRU
    {
        using pr = pair<int, int>;
        using itr = list<pr>::iterator;
        using hash = unordered_map < int, itr >;
        friend class LFUCache;
        hash cachemap;
        list<pr> cache;
    public:
        LRU()
        {
        }

        int get(int key)
        {
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
                if (cachemap.count(key))//exist:MTF
                {
                    auto itr = cachemap[key];
                    cache.erase(itr);
                }
                cache.push_front(make_pair(key, value));
                cachemap[key] = cache.begin();
        }
    };
    using pr = pair<int, int>;
    using itr = list<pr>::iterator;
    using valhash= unordered_map < int,int >;
    using hash = unordered_map < int, LRU >;
    // fre -> LRU
    hash fremap;
    // val -> fre
    valhash valmap;
    int cap;
    //least frequency
    int lf = 1;
public:
    LFUCache(int capacity) :cap(capacity){}
    int get(int key)
    {
        if (cap==0||!valmap.count(key))return -1;
        else//update fremap and lf
        {
            int fre = valmap[key];
            valmap[key]++;
            auto ptr = fremap[fre].cachemap[key];
            int value = ptr->second;
            fremap[fre].cache.erase(ptr);
            fremap[fre].cachemap.erase(key);
            fre++;
            fremap[fre].put(key, value);
            if (fre == lf + 1 && fremap[lf].cache.size() == 0)lf++;
            return value;
        }
    }

    void put(int key, int value)
    {
        static int size = 0;
        if (cap == 0)return;
        if (size < cap || valmap.count(key))
        {
            if (!valmap.count(key))//new
            {
                valmap[key] = 1;
                fremap[1].put(key, value);
                lf = 1;
                size++;
            }
            else//edit
            {
                int fre = valmap[key];
                valmap[key]++;
                auto ptr = fremap[fre].cachemap[key];
                fremap[fre].cache.erase(ptr);
                fremap[fre].cachemap.erase(key);
                fre++;
                fremap[fre].put(key, value);
                if (fre == lf + 1 && fremap[lf].cache.size() == 0)lf++;
            }
        }
        else {//full(new)
            valmap.erase(fremap[lf].cache.back().first);
            fremap[lf].cachemap.erase(fremap[lf].cache.back().first);
            fremap[lf].cache.pop_back();
            size--;
            put(key, value);
        }
    }

};