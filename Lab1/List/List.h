// List.h
#pragma once
#ifndef _INC_LIST_
#define _INC_LIST_
// Подключаемые заголовочные файлы
#include <iostream>
#include <string>


//************************************************************ Класс исключений *******************************************************************//
class List_exception
{

	using string = std::string;

	string error;

public:

	List_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


//************************************************************ Класс АТД "Список" *****************************************************************//
template <class T>
class List
{

	size_t size;			// Количество элементов в массиве
	size_t capacity;		// Ёмкость
	T* data;				// Данные
	mutable size_t last_op;	// Количество переборов на последней операции
	
	// Тело конструктора копирования
	void _copy(const List& list);
	// Изменение ёмкости списка
	void _change_capacity(size_t capacity);	
	// Получить индекс элемента, если его нет в списке, возвращается -1
	size_t _getIndex(const T& item) const;

public:

	class Iterator;
	class RIterator;

	// Конструктор по-умолчанию
	List() : size{ 0 }, capacity{ 0 }, data{ nullptr }, last_op{ 0 } {}
	// Конструктор с параметрами: size - размер списка, val - элемент по-умолчанию
	List(size_t size, const T& val);
	// Конструктор копирования
	List(const List& list) { _copy(list); }
	// Деструктор
	~List() { if (this->data != nullptr) delete[] this->data; }
	
	// Оператор доступа к элементу по индексу
	T& operator[](size_t index);
	const T& operator[](size_t index) const { return const_cast<List&>(*this)[index]; }
	// Оператор присваивания списков
	const List& operator=(const List& list);

	// Получить размер списка
	size_t getSize() const { return this->size; }
	// Получить ёмкость
	size_t getCapacity() const { return this->capacity; }
	// Получить количество переборов на последней операции
	size_t getLastOp() const { return this->last_op; }
	// Получить индекс заданного элемента, если элемента нет вызывается исключение
	size_t index(const T& item) const;
	// Проверка списка на пустоту
	bool isEmpty() const { return this->size == 0; }
	// Есть ли элемент в списке
	bool inList(const T& item) const { return _getIndex(item) != -1; }
	
	// Чтение значения с заданным номером в списке
	const T& getItemByIndex(size_t index) const { return const_cast<List&>(*this)[index]; }
	// Запись значения по заданному индексу
	void setItemByIndex(size_t index, const T& item) { (*this)[index] = item; }
	
	// Очистка списка
	void clear() { _change_capacity(0); this->size = 0; this->last_op = 0; }
	// Добавить в конец списка 
	void append(const T& item) { insert(item, this->size); }
	// Добавить элемент в заданную позицию списка
	void insert(const T& item, size_t index);
	// Удалить первый элемент списка, равный item
	void deleteItem(const T& item) { deleteItemByIndex(index(item)); }
	// Удалить элемент под индексом i
	void deleteItemByIndex(size_t index);
	
	// Вывести список
	void print() const;
	
	// Установка итератора на начальный элемент списка
	Iterator begin() const { return Iterator(*this, 0); }
	// Установка итератора на элемент после конечного элемента
	Iterator end() const { return Iterator(*this, this->size); }
	// Установка обратного итератор в конец списка
	RIterator rbegin() const { return RIterator(*this, 0); }
	// Установка обратного итератора перед первым элементом списка
	RIterator rend() const { return RIterator(*this, this->size); }

	// Класс прямого итератора
	class Iterator
	{

		const List* p_list;	// Ссылка на список
		size_t cur;			// Номер текущего элемент списка

	public:

		// Конструктор прямого итератора по умолчанию
		Iterator() : p_list(nullptr), cur(-1) {}
		// Конструктор прямого итератора с параметром list - ссылкой на лист
		Iterator(const List& list) : p_list(&list), cur(-1) {}
		// Конструктор прямого итератора с параметром cur - номером текущего элемента списка
		Iterator(const List& list, size_t cur);
		// Конструктор копирования прямого итератора
		Iterator(const Iterator& iter) : p_list(iter.p_list), cur(iter.cur) {}

		// Оператор "разыменовывания" - получение самого элемента списка по ссылке
		T& operator*() const;

		// Оператор префиксного инкрементирования прямого итератора
		Iterator& operator++() { *this += 1; return *this; }
		// Оператор префиксного декрементирования прямого итератора
		Iterator& operator--() { *this -= 1; return *this; }
		// Оператор постфиксного инкрементирования прямого итератора
		Iterator operator++(int);
		// Оператор постфиксного декрементирования прямого итератора
		Iterator operator--(int);
		// Оператор перемещения прямого итератора на n позиций вправо
		Iterator operator+(size_t n) { return Iterator(*this) += n; }
		// Оператор перемещения прямого итератора на n позиций влево
		Iterator operator-(size_t n) { return Iterator(*this) -= n; }

		// Оператор равенства прямых итераторов
		bool operator==(const Iterator& iter) const { return this->p_list == iter.p_list && this->cur == iter.cur; }
		// Оператор неравенства прямых итераторов
		bool operator!=(const Iterator& iter) const { return !(this->p_list == iter.p_list && this->cur == iter.cur); }

		// Оператор присваивания прямого итератора
		const Iterator& operator=(const Iterator& iter);
		// Оператор "+=" прямого итератора - делает тоже, самое, что и оператор "+", но возвращает ссылку на сам объект
		const Iterator& operator+=(size_t n);
		// Оператор "-=" прямого итератора - делает тоже, самое, что и оператор "-", но возвращает ссылку на сам объект
		const Iterator& operator-=(size_t n);

	};

	// Класс обратного итератора
	class RIterator
	{

		const List* p_list;	// Ссылка на список
		size_t cur;			// Номер текущего элемент списка

	public:

		// Конструктор обратного итератора по умолчанию
		RIterator() : p_list(nullptr), cur(-1) {}
		// Конструктор обратного итератора с параметром list - ссылкой на лист
		RIterator(const List& list) : p_list(&list), cur(-1) {}
		// Конструктор обратного итератора с параметром cur - номером текущего элемента списка
		RIterator(const List& list, size_t cur);
		// Конструктор копирования обратного итератора
		RIterator(const RIterator& riter) : p_list(riter.p_list), cur(riter.cur) {}

		// Оператор "разыменовывания" - получение самого элемента списка по ссылке
		T& operator*() const;

		// Оператор префиксного инкрементирования обратного итератора
		RIterator& operator++() { *this += 1; return *this; }
		// Оператор префиксного декрементирования обратного итератора
		RIterator& operator--() { *this -= 1; return *this; }
		// Оператор постфиксного инкрементирования обратного итератора
		RIterator operator++(int);
		// Оператор постфиксного декрементирования обратного итератора
		RIterator operator--(int);
		// Оператор перемещения обратного итератора на n позиций вправо
		RIterator operator+(size_t n) { return RIterator(*this) += n; }
		// Оператор перемещения обратного итератора на n позиций влево
		RIterator operator-(size_t n) { return RIterator(*this) -= n; }

		// Оператор равенства обратных итераторов
		bool operator==(const RIterator& riter) const { return this->p_list == riter.p_list && this->cur == riter.cur; }
		// Оператор неравенства обратных итераторов
		bool operator!=(const RIterator& riter) const { return !(this->p_list == riter.p_list && this->cur == riter.cur); }

		// Оператор присваивания обратного итератора
		const RIterator& operator=(const RIterator& riter);
		// Оператор "+=" обратного итератора - делает тоже, самое, что и оператор "+", но возвращает ссылку на сам объект
		const RIterator& operator+=(size_t n);
		// Оператор "-=" обратного итератора - делает тоже, самое, что и оператор "-", но возвращает ссылку на сам объект
		const RIterator& operator-=(size_t n);

	};

};
//*************************************************************************************************************************************************//


//************************************************************** Реализация методов List **********************************************************//
// private-методы:
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
	if (capacity > this->capacity) // Если ёмкость больше данной
	{
		T* temp = new T[capacity]; // Выделяем память под новый массив элементов
		for (size_t i = 0; i < this->capacity; i++) // Копируем содержимое старого массива в новый
			temp[i] = this->data[i];

		delete[] this->data;
		this->data = temp;
		this->capacity = capacity;
		this->last_op = this->capacity;
	}
	else if (capacity <= this->capacity / 2) // Если список уменьшился в 2 или более раза, то сокращаем выделенную память
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

// public-методы:
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
		throw List_exception("индекс вышел за пределы списка");
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

	// Если элемента нет в списке
	if (index == -1)
		throw List_exception("данного элемента нет в списке");
	else
		return index;
}

template<class T> typename void List<T>::insert(const T& item, size_t index)
{
	// Корректируем индекс, если он вышел за пределы
	if (index > this->size)
		index = this->size;

	_change_capacity(this->size + 1);
	this->size++;

	// Сдвигаем элемента вправо, освобождая место для нового
	for (size_t i = this->size - 1; i > index; i--)
		this->data[i] = this->data[i - 1];

	this->data[index] = item;
	this->last_op += this->size - index;
}

template<class T> typename void List<T>::deleteItemByIndex(size_t index)
{
	if (index >= this->size)
		throw List_exception("индекс вышел за пределы списка");

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


//************************************************************* Реализация методов Iterator *******************************************************//
template <class T> List<T>::Iterator::Iterator(const List& list, size_t cur)
{
	this->p_list = &list;

	if (cur >= list.size)
		cur = -1;

	this->cur = cur;
}

template <class T> typename T& List<T>::Iterator::operator*() const
{
	// Если итератор не установлен
	if (this->cur == -1)
		throw List_exception("итератор не установлен");

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
		throw List_exception("итератор не установлен");
	
	if (this->cur + n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur += n;

	return *this;
}

template <class T> const typename List<T>::Iterator& List<T>::Iterator::operator-=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("итератор не установлен");
	
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


//********************************************************** Реализация методов RIterator *********************************************************//
template <class T> List<T>::RIterator::RIterator(const List& list, size_t cur)
{
	this->p_list = &list;

	if (cur >= list.size)
		cur = -1;

	this->cur = cur;
}

template <class T> typename T& List<T>::RIterator::operator*() const
{
	if (this->cur == -1) // Если итератор не установлен
		throw List_exception("итератор не установлен");

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
		throw List_exception("итератор не установлен");
	
	if (this->cur - n >= this->p_list->size)
		this->cur = -1;
	else
		this->cur -= n;

	return *this;
}

template <class T> const typename List<T>::RIterator& List<T>::RIterator::operator-=(size_t n)
{
	if (this->cur == -1)
		throw List_exception("итератор не установлен");
	
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
