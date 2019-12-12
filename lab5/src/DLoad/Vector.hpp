#ifndef __vector_h__
#define __vector_h__

#include <iostream>
#include <algorithm>
#include <cassert>


using value_type = int;
using iterator = value_type*;
        
typedef struct vector {
        int already_used_;
        int storage_size_;
        value_type* storage_;
} vector;


extern "C" vector* vector_create();
vector* vector_create_n(int size, const value_type& default_value = value_type());
int vector_size(vector* in);
bool vector_is_empty(vector* in);
iterator vector_begin(vector* in);
iterator vector_end(vector* in);
void vector_swap(vector* lhs, vector* rhs);
extern "C" void vector_destroy(vector** in);
extern "C" void vector_push_back(vector* in, const value_type& value);
extern "C" value_type& vector_at(vector* in, int index);

#endif

