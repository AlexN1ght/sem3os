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


vector* vector_create();
vector* vector_create(int size, const value_type& default_value = value_type());
int vector_size(vector* in);
bool vector_is_empty(vector* in);
iterator vector_begin(vector* in);
iterator vector_end(vector* in);
void vector_swap(vector* lhs, vector* rhs);
void vector_destroy(vector** in);
void vector_push_back(vector* in, const value_type& value);
value_type& vector_at(vector* in, int index);
