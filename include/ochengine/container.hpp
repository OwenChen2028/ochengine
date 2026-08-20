#pragma once

template <typename T> struct Container {
  T *values;

  int size;
  int capacity;

  Container() {
    size = 0;
    capacity = 10;

    values = new T[capacity];
  }

  ~Container() { delete[] values; }

  Container(const Container &other) : size(other.size), capacity(other.capacity) {
    values = new T[capacity];

    for (int i = 0; i < size; i++) {
      values[i] = other.values[i];
    }
  }

  Container &operator=(const Container &other) {
    if (this != &other) {
      T *newValues = new T[other.capacity];

      for (int i = 0; i < other.size; ++i) {
        newValues[i] = other.values[i];
      }

      delete[] values;

      values = newValues;
      size = other.size;
      capacity = other.capacity;
    }

    return *this;
  }

  void addValue(T value) {
    if (size == capacity) {
      capacity *= 2;
      T *newValues = new T[capacity];

      for (int i = 0; i < size; i++) {
        newValues[i] = values[i];
      }

      delete[] values;
      values = newValues;
    }

    values[size] = value;
    size++;
  }

  void setValue(T value, int index) {
    if (index >= 0 && index < size) {
      values[index] = value;
    } else if (index == size) {
      addValue(value);
    }
  }

  T getValue(int index) {
    if (index >= 0 && index < size) {
      return values[index];
    } else {
      return T(); // default value
    }
  }

  int getSize() { return size; }

  int getCapacity() { return capacity; }
};
