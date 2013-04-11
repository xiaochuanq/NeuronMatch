/*
 * quicksort.hpp
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#ifndef QUICKSORT_HPP_
#define QUICKSORT_HPP_

template<typename T>
int partition(vector<T>& vec, int left, int right, int pivot_idx)
{
	T key = vec[pivot_idx].key();
	swap( vec[right], vec[pivot_idx]);
	int store_idx = left;
	for( int i = left; i < right; ++i){
		if(vec[i].key() <= key){
			swap( vec[i], vec[store_idx]);
			++store_idx;
		}
	}
	swap( vec[store_idx], vec[right] );
	return store_idx;
}

template<typename T>
void quicksort(vector<T>& vec, int left, int right)
{
	if( right <= left)
		return;
	int pivot_idx = (left + right) / 2;
	int new_pivot_idx = partition(vec, left, right, pivot_idx);
	quicksort(vec, left, new_pivot_idx - 1);
	quicksort(vec, new_pivot_idx+1, right);
}

#endif /* QUICKSORT_HPP_ */
