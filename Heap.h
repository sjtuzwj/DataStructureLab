/*
 * File Heap.h
 * -----------
 * You should finish your homework in this file
 */

#ifndef _heap_h
#define _heap_h

#include <iostream>
#include <vector>
#include <map>
#include<algorithm>
#include<cmath>
using namespace std;

//assumption:
//all paras are legal!!!!!!!!!!!



/*
 * Class Heap
 * ----------
 * This is the class you should implement.
 * It is a template class, Compare can be greater<int>, less<int> and so on.
 * When you define a Heap like Heap<greater<int>>, it should be a maxHeap (who's top this the max element)
 * When you define a Heap like Heap<less<int>>, it should be a minHeap (who's top this the min element)
 * So, when you want to compare two element while implement the member function, you shoud use the member variable cmp, rather than <,<=,>,>=
 */ 
template<class Compare>
class Heap
{
    private:
      // use elements to store the element
      map<int,vector<int>> elements;
      int hp=0;

      // cmp is a comparer that can help you to compare two element's priority
      // Usage:
      // cmp(element_1, element_2)
      // if element_1's priority is higher than element_2, it will return true, otherwise, return false
      // Example:
      // when you define a Heap like Heap<greater<int>>, it means Compare is greater<int>, and cmp is a instance of greater<int>
      // So, cmp(1,2) and cmp(1,1) will return false and cmp(2,1) will return true
      // when you define a Heap like Heap<less<int>>, it means Compare is less<int>, and cmp is a instance of less<int>
      // So, cmp(2,1) and cmp(1,1) will return false and cmp(1,2) will return true
      // By this way, you can implement minHeap and maxHeap using one class
      Compare cmp;

      // the Top element's index, can make the code more readable
      // It's up to you whether to use it
       pair<int,int> topIndex = make_pair(0,0);
       pair<int,int> nilIndex = make_pair(0,0);


      // Some help function, can improve the code structure
      // It's up to you whether to use them
      int& get(pair<int,int>index)
      {
          if (!elements.count(index.first))elements[index.first] = vector<int>(pow(2, index.first));//complete
          return elements[index.first][index.second];
      }
       pair<int,int> getParentIndex(pair<int,int> index)
       {
           return make_pair(index.first-1,index.second/2);
       }
       pair<int,int> getLeftIndex(pair<int,int> index)
       {
            return make_pair(index.first+1,index.second*2);   
       }
       pair<int,int> getRightIndex(pair<int,int> index)
       {
           return make_pair(index.first+1,index.second*2+1);
       }
       bool isInRange(pair<int,int> index)
       {
           return index.first == nilIndex.first && index.second <= nilIndex.second || nilIndex.first>index.first;
       }
       bool isTop(pair<int,int> index)
       {
           return index==topIndex;
       }
       void swap(pair<int,int> index_1, pair<int,int> index_2)
       {
           std::swap(get(index_1),get(index_2));
       }
       pair<int,int> findIndex(int element)
       {
           for (auto& x : elements)//暴力搜索
           {
               auto i = find(x.second.begin(), x.second.end(), element);
               if(i!=x.second.end())
                   return make_pair(x.first, i- x.second.begin());
           }
           return make_pair(-1, -1);
       }
       void shift_up(pair<int,int> index)
       {
           if(isTop(index))return;
           auto prt=getParentIndex(index);
           if(cmp(get(index),get(prt))) //下克上 (●ˇ∀ˇ●)
           {
               swap(index, prt);
               shift_up(prt);
           }
       }
       void shift_down(pair<int,int> index)
       {
           auto l=getLeftIndex(index);
           auto r=getRightIndex(index);
           if(isInRange(l) && isInRange(r))//二胎
           {   
           auto root= cmp(get(l),get(r))? l:r;//二子夺嫡 ヾ(•ω•`)o
           if(cmp(get(root),get(index)))//传位嫡长子 
           {
               swap(index, root);
               shift_down(root);
           }
           }
           else if(isInRange(l))//独生子女
           {
           if(cmp(get(l),get(index))) 
           {
               swap(index, l);
               shift_down(l);
           }
           }
           else
           return;
       }

    public:
        void show()
        {
            for (auto &x : elements)
            {
                for (auto & y : x.second)
                    cout << y << '\t';
                cout << endl;
            }

        }
      /*
       * Constructor: Heap()
       * Usage: Heap<greater<int>> maxHeap
       */
      Heap()=default;

      /*
       * Constructor: Heap(vector<int> init_elements)
       * Usage: Heap<greater<int>> maxHeap(init_elements)
       * ------------------------------------------------
       * You should implement this Method with O(mlog(n)) time complexity (m = init_elements.size(), n = elements.size())
       */
      Heap(vector<int> init_elements);

      /*
       * Destructor: ~Heap()
       * Usage: usually implicit
       */
      ~Heap()=default;

      /*
       * Method: insert(int element)
       * Usage: maxHeap.insert(1)
       * ------------------------
       * Add the new element in to the end of elements and shift_up this element
       * You should implement this Method with O(log(n)) time complexity (n = elements.size())
       */
      void insert(int element);

      /*
       * Method: insert(vector<int> new_elements)
       * Usage: maxHeap.insert(new_elements)
       * ------------------------
       * Add the new element in to the end of elements and shift_up this element one by one
       * You should implement this Method with O(mlog(n)) time complexity (m = new_elements.size(), n = elements.size())
       */
      void insert(const vector<int>& new_elements);

      /*
       * Method: erase(int element)
       * Usage: maxHeap.erase(1)
       * ------------------------
       * Swap this element with the elements's last element, delete the last element, and shift_down the swapped element
       * You should implement this Method with O(log(n)) time complexity (n = elements.size())
       */
      bool erase(int element);

      /*
       * Method: pop()
       * Usage: maxHeap.pop()
       * ------------------------
       * Swap the top element with the elements's last element, delete and return the last element, and shift_down the swapped element
       * You should implement this Method with O(log(n)) time complexity (n = elements.size())
       */
      int pop();

      /*
       * Method: top()
       * Usage: maxHeap.top()
       * ------------------------
       * return the top element
       * You should implement this Method with O(log(1)) time complexity
       */
      int top();

      /*
       * Method: size()
       * Usage: maxHeap.size()
       * ------------------------
       * return the number of element in the Heap
       * You should implement this Method with O(log(1)) time complexity
       */
      int size();
};
template<class Compare>
Heap<Compare>::Heap(vector<int> init_elements)//mlgn
{
    insert(init_elements);
}

template<class Compare>
void Heap<Compare>::insert(int element)//only one operation lgn -> lgn
{
    hp++;
    if (hp == 1)nilIndex=make_pair(0,0);
    else if(nilIndex.second==pow(2,nilIndex.first)-1)
       {
            nilIndex.first++;
            nilIndex.second=0;
       }
    else nilIndex.second++;
    get(nilIndex) = element;
    shift_up(nilIndex);//lgn
}

template<class Compare>
void Heap<Compare>::insert(const vector<int>& new_elements)//mlgn
{
    for(auto x:new_elements)
    insert(x);
}

template<class Compare>
bool Heap<Compare>::erase(int element)//n+ lgn
{
    hp--;
    auto index=findIndex(element);//On
    if (index.first == -1)return false;
    swap(index,nilIndex);
    if(nilIndex.second==0)
       {
            elements.erase(nilIndex.first);//O1
            nilIndex.first--;
            nilIndex.second=pow(2,nilIndex.first)-1;
       }
    else
    {
    nilIndex.second--;
    }
    shift_down(index);
    return true;
}

template<class Compare>
int Heap<Compare>::pop()//olgn
{
    int t= top();
    erase(top());//erase: findIndex always find top first, so O(lgn)
    return t;
}

template<class Compare>
int Heap<Compare>::top()//o1
{
    return get(topIndex);
}

template<class Compare>
int Heap<Compare>::size()//o1
{
    return hp;
}

#endif
