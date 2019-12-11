#include "Vector.hpp"

vector* vector_create() {
    vector* out = new vector;
    out->already_used_ = 0;
    out->storage_size_ = 0;
    out->storage_ = nullptr;
    return out;
}

vector* vector_create(int size, const value_type& default_value)
{
    vector* out = vector_create();
    assert(size >= 0);

    if (size == 0) {
        return out;
    }

    out->already_used_ = size;
    out->storage_size_ = size;
    out->storage_ = new value_type[size];

    std::fill(out->storage_, out->storage_ + out->already_used_, default_value);
    return out;
}

int vector_size(vector* in)
{
    return in->already_used_;
}

bool vector_is_empty(vector* in)
{
    return vector_size(in) == 0;
}

iterator vector_begin(vector* in)
{
    return in->storage_;
}

iterator vector_end(vector* in)
{
    if (in->storage_) {
        return in->storage_ + in->already_used_;
    }

    return nullptr;
}

void vector_swap(vector* lhs, vector* rhs)
{
    using std::swap;

    swap(lhs->already_used_, rhs->already_used_);
    swap(lhs->storage_size_, rhs->storage_size_);
    swap(lhs->storage_, rhs->storage_);
}



// TVector& operator=(TVector other)
// {
//     Swap(*this, other);
//     return *this;
// }

// TVector(const TVector& other):
//     TVector()
// {
//     TVector next(other.storage_size_);
//     next.already_used_ = other.already_used_;

//     if (other.storage_ ) {
//         std::copy(other.storage_, other.storage_ + other.storage_size_,
//                 next.storage_);
//     }

//     swap(*this, next);
// }

void vector_destroy(vector** in)
{
    delete[] (*in)->storage_;

    (*in)->storage_size_ = 0;
    (*in)->already_used_ = 0;
    (*in)->storage_ = nullptr;
    delete (*in);
    *in = nullptr;
}

void vector_push_back(vector* in, const value_type& value)
{
    if (in->already_used_ < in->storage_size_) {
        in->storage_[in->already_used_] = value;
        ++(in->already_used_);
        return;
    }

    int next_size = 1;
    if (in->storage_size_) {
        next_size = in->storage_size_ * 2;
    }

    vector* next = vector_create(next_size);

    next->already_used_ = in->already_used_;

    if (in->storage_ ) {
        std::copy(in->storage_, in->storage_ + in->storage_size_, next->storage_);
    }

    vector_push_back(next, value);
    vector_swap(in, next);
    delete next;
}

value_type& vector_at(vector* in, int index)
{
    if (index < 0 || index > in->already_used_) {
        std::cout << "Out of range\n";
        exit(-1);
    }

    return in->storage_[index];
}
