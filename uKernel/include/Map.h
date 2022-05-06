#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <stdlib.h>

#include "schedassert.h"

#define MAP_INIT_SIZE 2

/**
 * This isn't exactly what you expected when you see "Map": it isn't implemented
 * using a set. It is implemented using a list. Operations that should be O(1)
 * are most time O(n). This is ok, because the expected number of elements for
 * our application is around 1 or 2 (maybe 3 in special cases).
 * @tparam KEY      The type of the map keys
 * @tparam VALUE    The type of the map values
 */
template <typename KEY, typename VALUE> class Map {
public:
  /** A map element. Stores a key and its corresponding value. */
  typedef struct {
    KEY key;
    VALUE value;
  } MapElement;

private:
  /** Array of map elements. */
  MapElement **data;
  /** The number of elements currently in the map. */
  size_t size;
  /** Current maximum size of the map. Used internally to calculate when to grow
   * the @data container */
  size_t maxSize;

  /** Duplicates the size of the @data container. Used internally. */
  void increaseSize() {
    this->maxSize *= 2;
    this->data = (MapElement **)realloc(this->data,
                                        this->maxSize * sizeof(MapElement *));
    assertCond(this->data != nullptr, F("Failed to re-allocate map memory"));
  }

  /** Appends a new element to the @data container. Used internally. */
  void add(KEY k, VALUE v) {
    if (this->size >= this->maxSize)
      increaseSize();
    this->data[this->size++] = new MapElement{k, v};
  }

public:
  Map() {
    this->maxSize = MAP_INIT_SIZE;
    this->size = 0;
    this->data = (MapElement **)malloc(MAP_INIT_SIZE * sizeof(MapElement *));
    assertCond(this->data != nullptr, F("Failed to allocate map memory"));
  }

  ~Map() {
    for (size_t i = 0; i < this->size; ++i) {
      free(this->data[i]);
    }
    free(this->data);
  }

  /** Returns the number of elements in the Map. */
  size_t getSize() const { return this->size; }

  /** Get an element by index. Used for iterating the map. */
  MapElement *at(size_t i) const {
    if (i >= this->size)
      return nullptr;
    return this->data[i];
  }

  /** Get an element by key. */
  MapElement *get(KEY k) const {
    for (size_t i = 0; i < this->size; ++i) {
      MapElement *elem = this->data[i];
      if (elem->key == k)
        return elem;
    }

    return nullptr;
  }

  /** Set the value for a key. */
  void set(KEY k, VALUE v) {
    MapElement *elem = this->get(k);
    if (elem == nullptr) {
      this->add(k, v);
    } else {
      elem->value = v;
    }
  }

  /** Remove the given key from the container. */
  void remove(KEY k) {
    size_t i;
    for (i = 0; i < this->size; ++i) {
      MapElement *elem = this->data[i];
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
