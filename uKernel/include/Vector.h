#ifndef UKERNEL_VECTOR_H
#define UKERNEL_VECTOR_H

#include <stddef.h>
#include <stdlib.h>

#include "schedassert.h"

#define VECTOR_INIT_SIZE 5

template <typename T> class Vector {
private:
  T *data;
  size_t size;
  size_t maxSize;

  void increaseSize() {
    this->maxSize *= 2;
    this->data = (T *)realloc(this->data, this->maxSize * sizeof(T *));
    assertCond(this->data != nullptr, F("Failed to re-allocate map memory"));
  }

public:
  Vector() {
    this->maxSize = VECTOR_INIT_SIZE;
    this->size = 0;
    this->data = (T *)malloc(VECTOR_INIT_SIZE * sizeof(T *));
    assertCond(this->data != nullptr, F("Failed to allocate map memory"));
  }

  ~Vector() { free(this->data); }

  size_t getSize() const { return this->size; }

  T at(size_t i) const {
    assertCond(i < this->size, F("Address out of bounds for vector"));
    return this->data[i];
  }

  void push(T elem) {
    if (this->size >= this->maxSize)
      this->increaseSize();
    this->data[this->size++] = elem;
  }

  size_t indexOf(T toFind) const {
    for (size_t i = 0; i < this->size; ++i) {
      if (this->data[i] == toFind)
        return i;
    }
    return this->size;
  }

  void remove(T toFind) {
    size_t i;
    for (i = 0; i < this->size; ++i) {
      if (this->data[i] == toFind)
        break;
    }
    if (i == this->size) // not found
      return;

    --this->size;
    for (; i < this->size; ++i) {
      this->data[i] = this->data[i + 1];
    }
  }

  void clear() { this->size = 0; }

  void sort(int (*cmpFunc)(const void *a, const void *b)) {
    qsort(this->data, this->size, sizeof(T), cmpFunc);
  }

  T operator[](size_t i) const { return this->at(i); }
};

#endif // UKERNEL_VECTOR_H
