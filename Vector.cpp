// ============================================================================
// CS32 P01 Part 1: Vector implementation
//
// This is the ONLY file you edit and submit.
//
// Every function below is a stub that throws "not implemented". Replace each
// stub body with your implementation. See Vector.h for what each function
// must do.
//
// Note: this file is #included at the bottom of Vector.h rather than compiled
// on its own (templates must be visible wherever they are used). Do not add
// it to a g++ command line; `make` already does the right thing.
// You may add your own non-member helper functions here.
// ============================================================================

#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

// ---- Constructors and the Big 3 ----

// Default constructor. Creates an empty vector: size 0, capacity 0, no
// allocation.
template <typename T>
Vector<T>::Vector() {
    begin_ = end_ = end_cap_ = nullptr;
}

// Creates a vector of n elements, each value-initialized (0 for int, "" for
// std::string). Size and capacity are both n. Note that new T[n] does NOT
// zero ints. If n is 0, allocates nothing.
//
// Hint: you don't need a special case for each type. Assigning T() works for
// every type: it gives 0 for int, 0.0 for double, nullptr for pointers, ""
// for std::string, and calls the default constructor for a class type.
template <typename T>
Vector<T>::Vector(std::size_t n) {
    begin_ = end_ = end_cap_ = nullptr;
    if (n) { begin_ = new T[n](); end_ = end_cap_ = begin_ + n; }
}

// Creates a vector of n copies of value. Size and capacity are both n.
// If n is 0, allocates nothing.
template <typename T>
Vector<T>::Vector(std::size_t n, const T& value) {
    begin_ = end_ = end_cap_ = nullptr;
    if (n) {
        T* buffer = new T[n];
        try { for (std::size_t i = 0; i < n; ++i) buffer[i] = value; }
        catch (...) { delete[] buffer; throw; }
        begin_ = buffer; end_ = end_cap_ = begin_ + n;
    }
}

// Creates a vector holding the elements of il, in order: Vector<int> v{1, 2, 3}.
// Size and capacity are both il.size(). If il is empty, allocates nothing.
//
// Working with std::initializer_list<T>: when you write Vector<int> v{1, 2, 3},
// the compiler packs {1, 2, 3} into il. It is a read-only view of the values:
//   - il.size() is the number of values (3 here)
//   - you can loop over it:  for (const T& x : il) { ... }
//   - il.begin() and il.end() are pointers to const T, like your own begin()/end()
//   - it has NO operator[], so il[i] does not compile
// Reference: https://en.cppreference.com/w/cpp/utility/initializer_list
template <typename T>
Vector<T>::Vector(std::initializer_list<T> il) {
    begin_ = end_ = end_cap_ = nullptr;
    if (il.size()) {
        T* buffer = new T[il.size()];
        std::size_t i = 0;
        try { for (const T& item : il) buffer[i++] = item; }
        catch (...) { delete[] buffer; throw; }
        begin_ = buffer; end_ = end_cap_ = begin_ + il.size();
    }
}

// Copy constructor. Makes a deep copy of other. The new capacity is other.size() 
// (not other.capacity()).
// If other is empty, allocates nothing.
template <typename T>
Vector<T>::Vector(const Vector& other) {
    begin_ = end_ = end_cap_ = nullptr;
    std::size_t n = other.size();
    if (n) {
        T* buffer = new T[n];
        try { for (std::size_t i = 0; i < n; ++i) buffer[i] = other[i]; }
        catch (...) { delete[] buffer; throw; }
        begin_ = buffer; end_ = end_cap_ = begin_ + n;
    }
}

// Copy assignment operator. Replaces this vector's contents with a deep copy
// of other's elements and frees the old buffer. The new capacity is
// other.size() (if 0, no buffer). Self-assignment (v = v) must leave v
// unchanged. Returns *this so that a = b = c works.
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this == &other) return *this;
    Vector copy(other);
    T* old_begin = begin_; T* old_end = end_; T* old_cap = end_cap_;
    begin_ = copy.begin_; end_ = copy.end_; end_cap_ = copy.end_cap_;
    copy.begin_ = old_begin; copy.end_ = old_end; copy.end_cap_ = old_cap;
    return *this;
}

// Destructor. 
template <typename T>
Vector<T>::~Vector() {
    delete[] begin_;
}

// ---- Private helper ----

// Private helper: changes the capacity to exactly new_cap. Afterwards,
// capacity() == new_cap, size() and the elements are unchanged, and no memory
// is leaked. Any function that changes the capacity can be built on this one.
// Precondition: new_cap >= size(). If new_cap is 0, the vector ends up with no
// buffer at all.
template <typename T>
void Vector<T>::reallocate(std::size_t new_cap) {
    std::size_t n = size();
    if (new_cap == 0) {
        delete[] begin_; begin_ = end_ = end_cap_ = nullptr; return;
    }
    T* buffer = new T[new_cap];
    try { for (std::size_t i = 0; i < n; ++i) buffer[i] = begin_[i]; }
    catch (...) { delete[] buffer; throw; }
    delete[] begin_;
    begin_ = buffer; end_ = buffer + n; end_cap_ = buffer + new_cap;
}

// ---- Element access ----

template <typename T>
const T& Vector<T>::at(std::size_t i) const {
    if (i >= size()) throw std::out_of_range("at");
    return begin_[i];
}

// Returns a reference to element i. Do NOT check bounds: making sure
// i < size() is the caller's job, and an out-of-range i is undefined behavior
// (just like std::vector). at() is the version that checks.
template <typename T>
T& Vector<T>::operator[](std::size_t i) {
    return begin_[i];
}

// Same as above, for const vectors. operator== needs this: its parameters are
// const Vector&, so it can only read elements through a const member function.
template <typename T>
const T& Vector<T>::operator[](std::size_t i) const {
    return begin_[i];
}

// Returns a reference to element i.
// Throws std::out_of_range if i >= size().
template <typename T>
T& Vector<T>::at(std::size_t i) {
    if (i >= size()) throw std::out_of_range("at");
    return begin_[i];
}

// Returns a reference to the first element.
// Throws std::out_of_range if the vector is empty.
// Returns a reference to the last element.
// Throws std::out_of_range if the vector is empty.
// Returns a pointer to the first element of the buffer, or nullptr if there
// is no buffer.
template <typename T>
T* Vector<T>::data() {
    return begin_;
}

// ---- Iterators ----

// Returns a pointer to the first element. Together with end(), this lets
// range-for loops and std::sort work on your Vector.
template <typename T>
T* Vector<T>::begin() {
    return begin_;
}

// Same as above, for const vectors. Together with end() const, this lets a
// range-for loop run over a const Vector&.
template <typename T>
const T* Vector<T>::begin() const {
    return begin_;
}

// Returns a pointer one past the last element (not past the end of the
// buffer). For an empty vector, end() == begin().
template <typename T>
T* Vector<T>::end() {
    return end_;
}

// Same as above, for const vectors.
template <typename T>
const T* Vector<T>::end() const {
    return end_;
}

// ---- Capacity ----

// Returns the number of elements currently stored.
template <typename T>
std::size_t Vector<T>::size() const {
    return begin_ == nullptr ? 0 : end_ - begin_;
}

// Returns how many elements the buffer can hold before it must grow.
template <typename T>
std::size_t Vector<T>::capacity() const {
    return begin_ == nullptr ? 0 : end_cap_ - begin_;
}

// Returns true if the vector has no elements (size() == 0).
// Makes room for n elements up front, so that the vector can later grow to
// n elements without reallocating. Afterwards, capacity() is at least n (and
// exactly n if the buffer had to grow). The elements and size() are unchanged,
// and capacity never goes down.
template <typename T>
void Vector<T>::reserve(std::size_t n) {
    if (n > capacity()) reallocate(n);
}

// Shrinks the buffer so capacity() == size(), keeping the elements.
// Does nothing if they are already equal. If size() is 0, frees the buffer
// and all three pointers become nullptr.
template <typename T>
void Vector<T>::shrink_to_fit() {
    if (size() != capacity()) reallocate(size());
}

// ---- Modifiers ----

// Appends a copy of value to the end. If the buffer is full, first grows it to
//     max(capacity() + 1, (size_t)(capacity() * growth_factor))
// Read growth_factor each time (the tests change it); do not hard-code 2.
// Careful: value may refer to an element of this vector (v.push_back(v[0])),
// which lives in the old buffer you are about to free.
template <typename T>
void Vector<T>::push_back(const T& value) {
    T saved(value);
    if (size() == capacity()) {
        std::size_t n = static_cast<std::size_t>(capacity() * growth_factor);
        if (n < capacity() + 1) n = capacity() + 1;
        reallocate(n);
    }
    *end_ = saved; ++end_;
}

// Removes the last element. Capacity is unchanged and nothing is freed.
// Throws std::out_of_range if the vector is empty.
// Removes all elements: size becomes 0. Capacity is unchanged and nothing is
// freed.
template <typename T>
void Vector<T>::clear() {
    end_ = begin_;
}

// Changes the size to n.
//   - n < size(): removes elements from the end. Capacity is unchanged.
//   - n > size(): appends T() elements (0 for int). If n > capacity(), first
//     grows the buffer to exactly n. Slots reused from earlier elements must be
//     overwritten with T(), not left holding old values.
template <typename T>
void Vector<T>::resize(std::size_t n) {
    std::size_t old_size = size();
    if (n > capacity()) reallocate(n);
    for (std::size_t i = old_size; i < n; ++i) begin_[i] = T();
    end_ = begin_ == nullptr ? nullptr : begin_ + n;
}

// Inserts a copy of value at position index, so that afterwards v[index] is
// the new value. The elements that were at index and after each move one
// position right. index == size() appends. If the buffer
// is full, first grows it with the same formula as push_back.
// Throws std::out_of_range if index > size() (and changes nothing).
// Like push_back, value may refer to an element of this vector.
template <typename T>
void Vector<T>::insert(std::size_t index, const T& value) {
    if (index > size()) throw std::out_of_range("insert");
    T saved(value);
    if (size() == capacity()) {
        std::size_t n = static_cast<std::size_t>(capacity() * growth_factor);
        if (n < capacity() + 1) n = capacity() + 1;
        reallocate(n);
    }
    for (std::size_t i = size(); i > index; --i) begin_[i] = begin_[i - 1];
    begin_[index] = saved; ++end_;
}

// Removes the element at index, shifting the elements after it one position
// left. Capacity is unchanged.
// Throws std::out_of_range if index >= size() (and changes nothing).
template <typename T>
void Vector<T>::erase(std::size_t index) {
    if (index >= size()) throw std::out_of_range("erase");
    for (std::size_t i = index; i + 1 < size(); ++i) begin_[i] = begin_[i + 1];
    --end_;
}

// ---- Non-member operators ----

// Returns true if a and b have the same size and every pair of elements
// compares equal with ==. Capacity does not matter.
// Returns true if a and b are not equal (hint: use operator==).
#endif
