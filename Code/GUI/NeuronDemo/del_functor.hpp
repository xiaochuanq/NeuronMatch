/*
 * del_functor.h
 *
 *  Created on: Mar 8, 2011
 *      Author: xiaochuanq
 */
#ifndef DEL_FUNCTOR_HPP
#define DEL_FUNCTOR_HPP
#include <utility>
template<class T>
struct del_fun
{
   void operator()(T p) {
     delete p;
   }
};

template<typename T>
struct del_array_fun
{
	void operator()(T p){
		delete[] p;
	}
};

template<class A, class B>
struct DeleteMapFntor
{
    // Overloaded () operator.
    // This will be called by for_each() function.
  void operator()(std::pair<A,B> x) const
    {
        // Assuming the second item of map is to be
        // deleted. Change as you wish.
        delete x.second;
    }
};

#endif
