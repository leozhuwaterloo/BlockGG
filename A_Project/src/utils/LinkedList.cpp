#include "./includes/LinkedList.hpp"

template<class T>
LinkedList<T>::LinkedList(): head(nullptr), tail(nullptr), size(0) {}

template<class T>
LinkedList<T>::~LinkedList() {}

template<class T>
void LinkedList<T>::add(T *object) {
  if (head == nullptr) {
    head = object;
    tail = object;
  } else {
    object->parent = tail;
    tail->next = object;
    tail = object;
  }
  ++size;
}

template<class T>
void LinkedList<T>::remove(T *object) {
  remove(object, true);
}

template<class T>
void LinkedList<T>::remove(T *object, const bool &be_free) {
  if (object == head) {
    head = head->next;
    if (head != nullptr) head->parent = nullptr;
    else tail = nullptr;
  } else if (object == tail) {
    tail = tail->parent;
    tail->next = nullptr;
  } else {
  	object->parent->next = object->next;
    object->next->parent = object->parent;
    object->parent = nullptr;
    object->next = nullptr;
  }
  if(be_free) {
    delete object;
    object = nullptr;
  }
  --size;
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() {
  return Iterator(head);
}

template<class T>
typename LinkedList<T>::Iterator LinkedList<T>::end() {
  return Iterator(nullptr);
}

// ---------------------------------------------------------------
template<class T>
LinkedList<T>::Iterator::Iterator(T *node): node(node) {}

template<class T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
  node = node->next;
  return *this;
}

template<class T>
T* LinkedList<T>::Iterator::operator*() {
  return node;
}

template<class T>
bool LinkedList<T>::Iterator::operator!=(const Iterator &other) const {
  return this->node != other.node;
}
