// HashOA.h
#pragma once
#ifndef _INC_HASH_OA_
#define _INC_HASH_OA_
// Подключаемые заголовочные файлы
#include <iostream>
#include "AbstractHash.h"
#include "Hash_exception.h"


//**************************************************** Хеш-таблица с открытой адресацией **********************************************************//
template <class Key_T, class Data_T>
class HashOA : public AbstractHash<Key_T, Data_T>
{

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;

	// Класс-перечисление состояния ячейки хеш-таблицы
	enum class Status
	{
		BUSY,		// Занята
		DELETED,	// Свободна после удаления из неё элемента
		FREE		// Свободна
	};

	// Структура ячейки хеш-таблицы
	struct Cell
	{
		Key_T key {};		// Ключ
		Data_T data {};		// Данные
		Status state {};	// Состояние
	};

	Cell* cells; // Динамический массив ячеек хеш-таблицы

	// Линейное зондирование
	size_t _linearHash(const Key_T& key, size_t i) const { return (HashOA::_hash(key) + i) % this->size; }
	// Копирования хеш-таблиц
	void _copy(const HashOA& table);

public:

	class IteratorOA;

	// Конструктор хеш-таблицы с открытой адресацией с заданным размером
	HashOA(size_t size);
	// Конструктор копирования хеш-таблицы с открытой адресацией
	HashOA(const HashOA& table);
	// Деструктор хеш-таблицы с открытой адресацией
	virtual ~HashOA() { delete[] this->cells; }

	// Оператор доступа к элементу по ключу
	virtual Data_T& operator[](const Key_T& key) final;
	// Оператор присваивания хеш-таблиц с цепочками коллизий
	virtual const HashOA& operator=(const AbstractHash& table) final;

	// Получить тип хеш-таблицы
	virtual TableType getType() const final { return TableType::OPEN_ADDRESSING; }
	
	// Очистка хеш-таблицы c открытой адресацией
	virtual void clear() final;
	// Добавить элемент в хеш-таблицу с открытой адресацией по заданному ключу key и данными data
	virtual void append(const Key_T& key, const Data_T& data) final;
	// Удалить элемент из хеш-таблицы с открытой адресацией по ключу key
	virtual void remove(const Key_T& key) final;
	
	// Вывод хеш-таблицы с цепочками коллизий
	virtual void print() const final;
	
	// Установка итератора на начало хеш-таблицы
	IteratorOA begin() const;
	// Установка итератора в "неустановленное" состояние
	IteratorOA end() const { return IteratorOA(*this); }

	// Класс итератора хеш-таблицы с открытой адресацией
	class IteratorOA : public AbstractIterator
	{

		friend class HashOA;

		const HashOA* table;	// Указатель на хеш-таблицу
		size_t pos;				// Номер текущего элемента

		// private-конструктор итератора с параметрами: table - указатель на хеш-таблицу с цепочками коллизий,
		// pos - номером текущей цепочки и node - указателем на текущий элемент в цепочке
		IteratorOA(const HashOA* table, size_t pos) : table(table) {}

	public:

		// Конструктор итератора по умолчанию
		IteratorOA() : table(nullptr), pos(-1) {}
		// Конструктор с ссылкой на хеш-таблицу
		IteratorOA(const HashOA& table) : table(&table), pos(-1) {}
		// Конструктор с ссылкой на хеш-таблицу и ключом элемента
		IteratorOA(const HashOA& table, const Key_T& key);
		// Конструктор копирования итератора
		IteratorOA(const IteratorOA& iter) : table(iter.table), pos(iter.pos) {}

		// Оператор "разыменовывания" - получение текущего элемента хеш-таблицы по ссылке
		virtual const Key_T& operator*() const final;

		// Оператор префиксного инкрементирования
		virtual IteratorOA& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const typename AbstractIterator& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const typename AbstractIterator& iter) const final;
		
		// Оператор присваивания итераторов
		virtual const IteratorOA& operator=(const typename AbstractIterator& iter) final;
	};

};
//*************************************************************************************************************************************************//


//**************************************************** Реализация методов HashOA ******************************************************************//
// private-методы:
template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::_copy(const HashOA& table)
{
	for (size_t i = 0; i < table.size; i++)
		this->cells[i] = table.cells[i];

	this->size = table.size;
	this->count = table.count;
	this->last_op = 0;
}


// public-методы:
template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::HashOA(size_t size)
{
	this->size = HashOA::_clp2(size);
	
	this->cells = new Cell[this->size];
	for (size_t i = 0; i < this->size; i++)
		this->cells[i].state = Status::FREE;

	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::HashOA(const HashOA& table)
{
	this->cells = new Cell[table.size];

	_copy(table);
}

template <class Key_T, class Data_T> typename Data_T& HashOA<Key_T, Data_T>::operator[](const Key_T& key)
{
	// i - номер пробы, j - номер текущей ячейки
	size_t i = 0, j = 0;

	this->last_op = 0;

	do
	{
		// Хешируем ключ для i-ой пробы
		j = _linearHash(key, i);

		this->last_op++;

		// Если найдена занятая ячейка с тем же ключом, что и key
		if (this->cells[j].key == key && this->cells[j].state == Status::BUSY)
			return this->cells[j].data;

		i++;
		// Проходим по циклу, пока не встретим первую свободную ячейку или не пройдём всю таблицу
	} while (this->cells[j].state != Status::FREE && i < this->size);

	// Если так и не встретили элемента с ключом key, то выбрасываем исключение
	throw Hash_exception("элемента с таким ключом нет в хеш-таблице");
}

template <class Key_T, class Data_T> const typename HashOA<Key_T, Data_T>& HashOA<Key_T, Data_T>::operator=(const AbstractHash& table)
{
	const HashOA& table_oa = dynamic_cast<const HashOA&>(table);

	if (this == &table_oa)
		return *this;

	delete[] this->cells;

	this->cells = new Cell[table_oa.size];

	_copy(table_oa);

	return *this;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::clear()
{
	for (size_t i = 0; i < this->size; i++)
		this->cells[i].state = Status::FREE;

	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	// i - номер пробы, j - номер текущей ячейки
	size_t i = 0, j = 0;
	// Позиция первой удалённой ячейки
	long long pos = -1;
	
	this->last_op = 0;

	do 
	{
		j = _linearHash(key, i); // Хешируем ключ для i-ой пробы

		this->last_op++;

		// Если впервые встретили удалённую ячейку, то запоминаем её позицию в pos
		if (this->cells[j].state == Status::DELETED && pos == -1)
			pos = j;
		// Если встретили ячейку с тем же ключом, и при этом она занята, то выбрасываем исключение
		if (this->cells[j].state == Status::BUSY && this->cells[j].key == key)
			throw Hash_exception("элемент с таким ключом уже есть в хеш-таблице");

		i++;
	// Проходим по циклу, пока не встретим свободную ячейку или не пройдём всю таблицу
	} while (this->cells[j].state != Status::FREE && i < this->size);
	
	// Если в таблице все ячейки заняты, то выбрасываем исключение
	if (i == this->size && pos == -1)
		throw Hash_exception("хеш-таблице переполнена");

	// Иначе, если не встречали удалённую ячейку, то в pos записываем номер первой свободной ячейки
	if (pos == -1)
		pos = j;
	
	// В ином случае, т.н. если встретили удалённую ячейку, то добавляем элемент именно туда
	// а не в свободную ячейку

	this->cells[pos].key = key;
	this->cells[pos].data = data;
	this->cells[pos].state = Status::BUSY;

	this->count++;
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::remove(const Key_T& key)
{
	// i - номер пробы, j - номер текущей ячейки
	size_t i = 0, j = 0;

	this->last_op = 0;

	do
	{
		// Хешируем ключ для i-ой пробы
		j = _linearHash(key, i);

		this->last_op++;

		// Если встретили занятую ячейку с тем же ключом, то и key, то помечаем её, как удалённую
		if (this->cells[j].key == key && this->cells[j].state == Status::BUSY)
		{
			this->cells[j].state = Status::DELETED;
			this->count--;

			return;
		}

		i++;
	// Проходим по циклу, пока не встретим свободную ячейку или не пройдём всю таблицу
	} while (this->cells[j].state != Status::FREE && i < this->size);

	// Если так и не встретили элемента с ключом key, то выбрасываем исключение
	throw Hash_exception("элемента с таким ключом нет в хеш-таблице");
}

template <class Key_T, class Data_T> typename void HashOA<Key_T, Data_T>::print() const
{
	using std::cout;
	using std::endl;

	for (size_t i = 0; i < this->size; i++)
	{
		cout << "[" << i << "]: ";

		if (this->cells[i].state == Status::BUSY)
		{
			cout << "Busy" << ": " << this->cells[i].key;
		}
		else if (this->cells[i].state == Status::DELETED)
			cout << "Deleted";
		else
			cout << "Free";

		cout << endl;
	}
}

template <class Key_T, class Data_T> typename HashOA<Key_T, Data_T>::IteratorOA HashOA<Key_T, Data_T>::begin() const
{
	size_t i = 0;

	// Проходим по всей хеш-таблицы в поиске первой занятой ячейки
	for (; i < this->size; i++)
		if (this->cells[i].state == Status::BUSY)
			// Если нашли такую ячейку, то устанавливаем итератор на неё и возвращаем его
			return IteratorOA(this, i);

	// Если не нашли такую ячейку, то возвращаем неустановленный итератор
	return IteratorOA(*this);

}
//*************************************************************************************************************************************************//


//***************************************************** Реализация методов класса IteratorOA ******************************************************//
// public-методы:
template <class Key_T, class Data_T> HashOA<Key_T, Data_T>::IteratorOA::IteratorOA(const HashOA& table, const Key_T& key)
{
	this->table = &table;

	// Ищем в хеш-таблице table элемент с ключом key

	// i - номер пробы, j - номер текущей ячейки
	size_t i = 0, j = 0;

	do
	{
		// Хешируем ключ для i-ой пробы
		j = table._linearHash(key, i);

		// Если найдена занятая ячейка с тем же ключом, что и key
		if (table.cells[j].key == key && table.cells[j].state == Status::BUSY)
		{
			this->pos = j;

			return;
		}

		i++;
		// Проходим по циклу, пока не встретим первую свободную ячейку или не пройдём всю таблицу
	} while (table.cells[j].state != Status::FREE && i < table.size);

	// Если так и не встретили элемента с ключом key
	this->pos = -1;
}

template <class Key_T, class Data_T> const typename Key_T& HashOA<Key_T, Data_T>::IteratorOA::operator*() const
{
	if (this->pos == -1)
		throw Hash_exception("итератор не установлен");
	else
		return this->table->cells[this->pos].key;
}

template <class Key_T, class Data_T> typename HashOA<Key_T, Data_T>::IteratorOA& HashOA<Key_T, Data_T>::IteratorOA::operator++()
{
	if (this->pos == -1)
		throw Hash_exception("итератор не установлен");

	for (this->pos++; this->pos < this->table->size; this->pos++)
		if (this->table->cells[this->pos].state == Status::BUSY)
			return *this;

	this->pos = -1;

	return *this;
}

template <class Key_T, class Data_T> typename bool HashOA<Key_T, Data_T>::IteratorOA::operator==(const AbstractIterator& iter) const
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	return this->table == iter_oa.table && this->pos == iter_oa.pos;
}

template <class Key_T, class Data_T> typename bool HashOA<Key_T, Data_T>::IteratorOA::operator!=(const typename AbstractIterator& iter) const
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	return !(this->table == iter_oa.table && this->pos == iter_oa.pos);
}

template <class Key_T, class Data_T> const typename HashOA<Key_T, Data_T>::IteratorOA& HashOA<Key_T, Data_T>::IteratorOA::operator=(const AbstractIterator& iter)
{
	const IteratorOA& iter_oa = dynamic_cast<const IteratorOA&>(iter);

	this->table = iter_oa.table;
	this->pos = iter_oa.pos;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
