#pragma once
#include "../../objects/includes/GameObject.hpp"
#include "../../guis/includes/GuiElement.hpp"

template<class T>
class LinkedList {
public:
	LinkedList();
	virtual ~LinkedList();
	T *head;
	T *tail;
	unsigned int size;
	void add(T *object);
	void remove(T *object, const bool &be_free);
	void remove(T *object);

	class Iterator {
	public:
		T *node;
		Iterator(T *node);
		Iterator& operator++();
		T* operator*();
		bool operator!=(const Iterator &other) const;
	};

	Iterator begin();
 	Iterator end();
};

template class LinkedList<GameObject>;
template class LinkedList<GuiElement>;
