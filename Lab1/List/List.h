// List.h
#pragma once
#ifndef _INC_LIST_
#define _INC_LIST_
// ������������ ������������ �����
#include <iostream>
#include <string>


//************************************************************ ����� ���������� *******************************************************************//
class List_exception
{

	using string = std::string;

	string error;

public:

	List_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


//************************************************************ ����� ��� "������" *****************************************************************//
template <class T>
class List
{

	size_t size;			// ���������� ��������� � �������
	size_t capacity;		// �������
	T* data;				// ������
	mutable size_t last_op;	// ���������� ��������� �� ��������� ��������
	
	// ���� ������������ �����������
	void _copy(const List& list);
	// ��������� ������� ������
	void _change_capacity(size_t capacity);	
	// �������� ������ ��������, ���� ��� ��� � ������, ������������ -1
	size_t _getIndex(const T& item) const;

public:

	class Iterator;
	class RIterator;

	// ����������� ��-���������
	List() : size{ 0 }, capacity{ 0 }, data{ nullptr }, last_op{ 0 } {}
	// ����������� � �����������: size - ������ ������, val - ������� ��-���������
	List(size_t size, const T& val);
	// ����������� �����������
	List(const List& list) { _copy(list); }
	// ����������
	~List() { if (this->data != nullptr) delete[] this->data; }
	
	// �������� ������� � �������� �� �������
	T& operator[](size_t index);
	const T& operator[](size_t index) const { return const_cast<List&>(*this)[index]; }
	// �������� ������������ �������
	const List& operator=(const List& list);

	// �������� ������ ������
	size_t getSize() const { return this->size; }
	// �������� �������
	size_t getCapacity() const { return this->capacity; }
	// �������� ���������� ��������� �� ��������� ��������
	size_t getLastOp() const { return this->last_op; }
	// �������� ������ ��������� ��������, ���� �������� ��� ���������� ����������
	size_t index(const T& item) const;
	// �������� ������ �� �������
	bool isEmpty() const { return this->size == 0; }
	// ���� �� ������� � ������
	bool inList(const T& item) const { return _getIndex(item) != -1; }
	
	// ������ �������� � �������� ������� � ������
	const T& getItemByIndex(size_t index) const { return const_cast<List&>(*this)[index]; }
	// ������ �������� �� ��������� �������
	void setItemByIndex(size_t index, const T& item) { (*this)[index] = item; }
	
	// ������� ������
	void clear() { _change_capacity(0); this->size = 0; this->last_op = 0; }
	// �������� � ����� ������ 
	void append(const T& item) { insert(item, this->size); }
	// �������� ������� � �������� ������� ������
	void insert(const T& item, size_t index);
	// ������� ������ ������� ������, ������ item
	void deleteItem(const T& item) { deleteItemByIndex(index(item)); }
	// ������� ������� ��� �������� i
	void deleteItemByIndex(size_t index);
	
	// ������� ������
	void print() const;
	
	// ��������� ��������� �� ��������� ������� ������
	Iterator begin() const { return Iterator(*this, 0); }
	// ��������� ��������� �� ������� ����� ��������� ��������
	Iterator end() const { return Iterator(*this, this->size); }
	// ��������� ��������� �������� � ����� ������
	RIterator rbegin() const { return RIterator(*this, 0); }
	// ��������� ��������� ��������� ����� ������ ��������� ������
	RIterator rend() const { return RIterator(*this, this->size); }

	// ����� ������� ���������
	class Iterator
	{

		const List* p_list;	// ������ �� ������
		size_t cur;			// ����� �������� ������� ������

	public:

		// ����������� ������� ��������� �� ���������
		Iterator() : p_list(nullptr), cur(-1) {}
		// ����������� ������� ��������� � ���������� list - ������� �� ����
		Iterator(const List& list) : p_list(&list), cur(-1) {}
		// ����������� ������� ��������� � ���������� cur - ������� �������� �������� ������
		Iterator(const List& list, size_t cur);
		// ����������� ����������� ������� ���������
		Iterator(const Iterator& iter) : p_list(iter.p_list), cur(iter.cur) {}

		// �������� "���������������" - ��������� ������ �������� ������ �� ������
		T& operator*() const;

		// �������� ����������� ����������������� ������� ���������
		Iterator& operator++() { *this += 1; return *this; }
		// �������� ����������� ����������������� ������� ���������
		Iterator& operator--() { *this -= 1; return *this; }
		// �������� ������������ ����������������� ������� ���������
		Iterator operator++(int);
		// �������� ������������ ����������������� ������� ���������
		Iterator operator--(int);
		// �������� ����������� ������� ��������� �� n ������� ������
		Iterator operator+(size_t n) { return Iterator(*this) += n; }
		// �������� ����������� ������� ��������� �� n ������� �����
		Iterator operator-(size_t n) { return Iterator(*this) -= n; }

		// �������� ��������� ������ ����������
		bool operator==(const Iterator& iter) const { return this->p_list == iter.p_list && this->cur == iter.cur; }
		// �������� ����������� ������ ����������
		bool operator!=(const Iterator& iter) const { return !(this->p_list == iter.p_list && this->cur == iter.cur); }

		// �������� ������������ ������� ���������
		const Iterator& operator=(const Iterator& iter);
		// �������� "+=" ������� ��������� - ������ ����, �����, ��� � �������� "+", �� ���������� ������ �� ��� ������
		const Iterator& operator+=(size_t n);
		// �������� "-=" ������� ��������� - ������ ����, �����, ��� � �������� "-", �� ���������� ������ �� ��� ������
		const Iterator& operator-=(size_t n);

	};

	// ����� ��������� ���������
	class RIterator
	{

		const List* p_list;	// ������ �� ������
		size_t cur;			// ����� �������� ������� ������

	public:

		// ����������� ��������� ��������� �� ���������
		RIterator() : p_list(nullptr), cur(-1) {}
		// ����������� ��������� ��������� � ���������� list - ������� �� ����
		RIterator(const List& list) : p_list(&list), cur(-1) {}
		// ����������� ��������� ��������� � ���������� cur - ������� �������� �������� ������
		RIterator(const List& list, size_t cur);
		// ����������� ����������� ��������� ���������
		RIterator(const RIterator& riter) : p_list(riter.p_list), cur(riter.cur) {}

		// �������� "���������������" - ��������� ������ �������� ������ �� ������
		T& operator*() const;

		// �������� ����������� ����������������� ��������� ���������
		RIterator& operator++() { *this += 1; return *this; }
		// �������� ����������� ����������������� ��������� ���������
		RIterator& operator--() { *this -= 1; return *this; }
		// �������� ������������ ����������������� ��������� ���������
		RIterator operator++(int);
		// �������� ������������ ����������������� ��������� ���������
		RIterator operator--(int);
		// �������� ����������� ��������� ��������� �� n ������� ������
		RIterator operator+(size_t n) { return RIterator(*this) += n; }
		// �������� ����������� ��������� ��������� �� n ������� �����
		RIterator operator-(size_t n) { return RIterator(*this) -= n; }

		// �������� ��������� �������� ����������
		bool operator==(const RIterator& riter) const { return this->p_list == riter.p_list && this->cur == riter.cur; }
		// �������� ����������� �������� ����������
		bool operator!=(const RIterator& riter) const { return !(this->p_list == riter.p_list && this->cur == riter.cur); }

		// �������� ������������ ��������� ���������
		const RIterator& operator=(const RIterator& riter);
		// �������� "+=" ��������� ��������� - ������ ����, �����, ��� � �������� "+", �� ���������� ������ �� ��� ������
		const RIterator& operator+=(size_t n);
		// �������� "-=" ��������� ��������� - ������ ����, �����, ��� � �������� "-", �� ���������� ������ �� ��� ������
		const RIterator& operator-=(size_t n);

	};

};
//*************************************************************************************************************************************************//


//************************************************************** ���������� ������� List **********************************************************//
// private-������:
template <class T> typename void List<T>::_copy(const List& list)
{
	_change_capacity(list.size);
	this->size = list.size;

	for (size_t i = 0; i < this->size; i++)
		this->data[i] = list.data[i];

	this->last_op = 0;
}

template <class T> typename void List<T>::_change_capacity(size_t capacity)
{
	if (capacity > this->capacity) // ���� ������� ������ ������
	{
		T* temp = new T[capacity]; // �������� ������ ��� ����� ������ ���������
		for (size_t i = 0; i < this->capacity; i++) // �������� ���������� ������� ������� � �����
			temp[i] = this->data[i];

		delete[] this->data;
		this->data = temp;
		this->capacity = capacity;
		this->last_op = this->capacity;
	}
	else if (capacity <= this->capacity / 2) // ���� ������ ���������� � 2 ��� ����� ����, �� ��������� ���������� ������
	{
		T* temp = new T[capacity];
		for (size_t i = 0; i < capacity; i++)
			temp[i] = this->data[i];

		delete[] this->data;
		this->data = temp;
		this->capacity = capacity;
		this->last_op = capacity;
	}
	else
		this->last_op = 0;
}

template<class T> typename size_t List<T>::_getIndex(const T& item) const
{
	for (size_t i = 0; i < this->size; i++)
		if (this->data[i] == item)
			return i;

	return -1;
}

// public-������:
template <class T> List<T>::List(size_t size, const T& val):
	size(size), capacity(0), data(nullptr)
{
	_change_capacity(size);
	this->size = size;

	for (size_t i = 0; i < this->size; i++)
		this->data[i] = val;
	
	this->last_op = 0;
}

template<class T> typename T& List<T>::operator[](size_t index)
{
	this->last_op = 1;

	if (index < this->size)
		return this->data[index];
	else
		throw List_exception("������ ����� �� ������� ������");
}

template<class T> const typename List<T>& List<T>::operator=(const List& list)
{
	if (this == &list)
		return *this;

	_copy(list);

	return *this;
}

template<class T> typename size_t List<T>::index(const T& item) const
{
	size_t index = _getIndex(item);

	// ���� �������� ��� � ������
	if (index == -1)
		throw List_exception("������� �������� ��� � ������");
	else
		return index;
}

template<class T> typename void List<T>::insert(const T& item, size_t index)
{
	// ������������ ������, ���� �� ����� �� �������
	if (index > this->size)
		index = this->size;

	_change_capacity(this->size + 1);
	this->size++;

	// �������� �������� ������, ���������� ����� ��� ������
	for (size_t i = this->size - 1; i > index; i--)
		this->data[i] = this->data[i - 1];

	this->data[index] = item;
	this->last_op += this->size - index;
}

template<class T> typename void List<T>::deleteItemByIndex(size_t index)
{
	if (index >= this->size)
		throw List_exception("������ ����� �� ������� ������");

	for (size_t i = index; i < this->size - 1; i++)
		this->data[i] = this->data[i + 1];

	_change_capacity(this->size - 1);
	
	this->size--;
	this->last_op += this->size - index;
}

template<class T> typename void List<T>::print() const
{
	using std::cout;
	using std::endl;

	cout << "[";
	if (this->size == 0)
	{
		cout << "]" << endl;

		return;
	}
	for (size_t i = 0; i < this->size - 1; i++)
		cout << this->data[i] << ", ";

	cout << this->data[this->size - 1] << "]" << endl;
}
//*************************************************************************************************************************************************//


//************************************************************* ���������� ������� Iterator *******************************************************//
template <class T> List<T>::Iterator::Iterator(const List& list, size_t cur)
{
	this->p_list = &list;

	if (cur >= list.size)
		cur = -1;

	this->cur = cur;
}

template <class T> typename T& List<T>::Iterator::operator*() const
{
	// ���� �������� �� ����������
	if (this->cur == -1)
		throw List_exception("�������� �� ����������");

	return this->p_list->data[this->cur];
}

template <class T> typename List<T>::Iterator List<T>::Iterator::operator++(int)
{
	Iterator tmp(*this);

	*this += 1;

	return tmp;
}

template <class T> typename List<T>::Iterator List<T>::Iterator::operator--(int)
{
	Iterator tmp(*this);

	*this -= 1;

	return tmp;
}

template <class T> const typename List<T>::Iterator& List<T>::Iterator::operator+=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("�������� �� ����������");
	
	if (this->cur + n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur += n;

	return *this;
}

template <class T> const typename List<T>::Iterator& List<T>::Iterator::operator-=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("�������� �� ����������");
	
	if (this->cur - n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur -= n;

	return *this;
}

template <class T> const typename List<T>::Iterator& List<T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	this->p_list = iter.p_list;
	this->cur = iter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


//********************************************************** ���������� ������� RIterator *********************************************************//
template <class T> List<T>::RIterator::RIterator(const List& list, size_t cur)
{
	this->p_list = &list;

	if (cur >= list.size)
		cur = -1;

	this->cur = cur;
}

template <class T> typename T& List<T>::RIterator::operator*() const
{
	if (this->cur == -1) // ���� �������� �� ����������
		throw List_exception("�������� �� ����������");

	return this->p_list->data[this->cur];
}

template <class T> typename List<T>::RIterator List<T>::RIterator::operator++(int)
{
	RIterator tmp(*this);

	*this += 1;

	return tmp;
}

template <class T> typename List<T>::RIterator List<T>::RIterator::operator--(int)
{
	RIterator tmp(*this);

	*this -= 1;

	return tmp;
}

template <class T> const typename List<T>::RIterator& List<T>::RIterator::operator+=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("�������� �� ����������");
	
	if (this->cur - n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur -= n;

	return *this;
}

template <class T> const typename List<T>::RIterator& List<T>::RIterator::operator-=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("�������� �� ����������");
	
	if (this->cur + n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur += n;

	return *this;
}

template <class T> const typename List<T>::RIterator& List<T>::RIterator::operator=(const RIterator& riter)
{
	if (this == &riter)
		return *this;

	this->p_list = riter.p_list;
	this->cur = riter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
