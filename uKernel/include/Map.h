#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <stdlib.h>

#include "assert.h"

#define MAP_INIT_SIZE 2

template<typename KEY, typename VALUE>
class Map {
public:
  typedef struct {
    KEY key;
    VALUE value;
  } MapElement;

private:
  MapElement** data;
  size_t size;
  size_t maxSize;

  void increaseSize() {
    this->maxSize *= 2;
    this->data = (MapElement**) realloc(this->data,
                                        this->maxSize * sizeof(MapElement*));
    assertCond(this->data != nullptr, F("Failed to re-allocate map memory"));
  }

  void add(KEY k, VALUE v) {
    if (this->size >= this->maxSize)
      increaseSize();
    this->data[this->size++] = new MapElement{k, v};
  }

public:
  Map() {
    this->maxSize = MAP_INIT_SIZE;
    this->size = 0;
    this->data = (MapElement**) malloc(MAP_INIT_SIZE * sizeof(MapElement*));
    assertCond(this->data != nullptr, F("Failed to allocate map memory"));
  }

  ~Map() {
    for (size_t i = 0; i < this->size; ++i) {
      free(this->data[i]);
    }
    free(this->data);
  }

  size_t getSize() const { return this->size; }

  MapElement* at(size_t i) const {
    if (i >= this->size) return nullptr;
    return this->data[i];
  }

  MapElement* get(KEY k) const {
    for (size_t i = 0; i < this->size; ++i) {
      MapElement* elem = this->data[i];
      if (elem->key == k)
        return elem;
    }

    return nullptr;
  }

  void set(KEY k, VALUE v) {
    MapElement* elem = this->get(k);
    if (elem == nullptr) {
      this->add(k, v);
    } else {
      elem->value = v;
    }
  }

  void remove(KEY k) {
    size_t i;
    for (i = 0; i < this->size; ++i) {
      MapElement* elem = this->data[i];
      if (elem->key == k) {
        free(elem);
        break;
      }
    }
    if (i == this->size) // not found
      return;
    --this->size;
    for (; i < this->size; ++i) {
      this->data[i] = this->data[i + 1];
    }
  }
};

#endif // MAP_H
