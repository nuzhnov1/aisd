// HashTable.h
#pragma once
#ifndef _INC_HASH_TABLE_
#define _INC_HASH_TABLE_
// Подключаемые заголовочные файлы
#include "AbstractHash.h"
#include "HashSC.h"
#include "HashOA.h"


//************************************************* Пользовательский класс хеш-таблицы ************************************************************//
template <class Key_T, class Data_T>
class HashTable
{

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;
	using HashSC = HashSC<Key_T, Data_T>;
	using IteratorSC = typename HashSC::IteratorSC;
	using HashOA = HashOA<Key_T, Data_T>;
	using IteratorOA = typename HashOA::IteratorOA;

	AbstractHash* tab_p; // Указатель на абстрактную хеш-таблицу

public:

	class Iterator;

	// Конструктор хеш-таблицы по умолчанию
	HashTable() : HashTable(0) {}
	// Конструктор хеш-таблицы с параметрами: размером size и типом таблицы type
	HashTable(size_t size, TableType type = TableType::SEPARATE_CHAINING);
	// Конструктор копирования хеш-таблиц
	HashTable(const HashTable& table);
	// Деструктор хеш-таблиц
	~HashTable() { delete this->tab_p; }

	// Оператор доступа к элементу по ключу
	Data_T& operator[](const Key_T& key) { return (*this->tab_p)[key]; }
	const Data_T& operator[](const Key_T& key) const { return const_cast<HashTable&>(*this)[key]; }
	// Оператор присваивания хеш-таблиц
	const HashTable& operator=(const HashTable& table);
	
	// Получить тип хеш-таблицы
	TableType getType() const { return this->tab_p->getType(); }
	// Получить размер хеш-таблицы
	size_t getSize() const { return this->tab_p->getSize(); }
	// Получить число элементов в хеш-таблице
	size_t getCount() const { return this->tab_p->getCount(); }
	// Получить число переборов на последней операции
	size_t getLastOp() const { return this->tab_p->getLastOp(); }
	// Проверка хеш-таблицы на пустоту
	bool isEmpty() const { return this->tab_p->isEmpty(); }

	// Чтение элемента хеш-таблицы по ключу
	const Data_T& getItemByKey(const Key_T& key) const { return const_cast<HashTable&>(*this)[key]; }
	// Запись элемента по заданному ключу
	void setItemByKey(const Key_T& key, const Data_T& data) { (*this)[key] = data; }

	// Очистка хеш-таблицы
	void clear() { this->tab_p->clear(); }
	// Добавить с заданным ключом и данными в хеш-таблицу
	void append(const Key_T& key, const Data_T& data) { this->tab_p->append(key, data); }
	// Удалить элемент с заданным ключом
	void remove(const Key_T& key) { this->tab_p->remove(key); }
	
	// Вывести хеш-таблицу
	void print() const { this->tab_p->print(); }

	// Установка итератора на начало хеш-таблицы
	Iterator begin() const;
	// Установка итератора в неустановленное состояние
	Iterator end() const;

	// Класс итератора хеш-таблицы
	class Iterator
	{
		friend class HashTable;

		AbstractIterator* iter_p;	// Указатель на абстрактный итератор
		TableType type;				// Тип таблицы, на которую установлен итератор

		// private-конструктор итератора с параметрами: iter_p - указателем на абстрактный итератор
		// и type - типом хеш-таблицы
		Iterator(typename AbstractIterator* iter_p, TableType type) : iter_p(iter_p), type(type) {}

	public:

		// Конструктор итератора по умолчанию
		Iterator() : iter_p(nullptr), type(TableType::SEPARATE_CHAINING) {}
		// Конструктор итератора с ссылкой на хеш-таблицу
		Iterator(const HashTable& table);
		// Конструктор итератора с параметрами: table - ссылка на хеш-таблицу, key - ключ
		Iterator(const HashTable& table, const Key_T& key);
		// Конструктор копирования итератора
		Iterator(const Iterator& iter);
		// Деструктор итератора
		~Iterator();

		// Оператор "разыменовывания" - получение текущего элемента хеш-таблицы по ссылке
		const Key_T& operator*() const;

		// Оператор префиксного инкрементирования
		Iterator& operator++();
		// Оператор постфиксного инкрементирования
		Iterator operator++(int);

		// Оператор равенства итераторов
		bool operator==(const Iterator& iter) const;
		// Оператор неравенства итераторов
		bool operator!=(const Iterator& iter) const;

		// Оператор присваивания итераторов
		const Iterator& operator=(const Iterator& iter);

	};

};
//*************************************************************************************************************************************************//


//*************************************************** Реализация методов HashTable ****************************************************************//
//public-методы:
template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::HashTable(size_t size, TableType type)
{
	// В зависимости от заданного типа таблицы создаём хеш-таблицу с цепочками коллизий
	// или с открытой адресацией

	if (type == TableType::SEPARATE_CHAINING)
		this->tab_p = new HashSC(size);
	else if (type == TableType::OPEN_ADDRESSING)
		this->tab_p = new HashOA(size);
	else
		throw Hash_exception("неизвестный тип хеш-таблицы");
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::HashTable(const HashTable& table)
{
	// В зависимости от типа таблицы table создаём хеш-таблицу с цепочками коллизий
	// или с открытой адресацией, идентичную table

	if (table.tab_p->getType() == TableType::SEPARATE_CHAINING)
		this->tab_p = new HashSC(dynamic_cast<const HashSC&>(*table.tab_p));
	else
		this->tab_p = new HashOA(dynamic_cast<const HashOA&>(*table.tab_p));
}

template <class Key_T, class Data_T> const typename HashTable<Key_T, Data_T>& HashTable<Key_T, Data_T>::operator=(const HashTable& table)
{
	if (this == &table)
		return *this;

	// Если типы таблиц совпадают, то вызываем виртуальный оператор присваивания
	if (table.tab_p->getType() == this->tab_p->getType())
		*this->tab_p = *table.tab_p;
	// Иначе удаляем текущую таблицу и создаём копию таблицы table
	else
	{
		delete this->tab_p;

		if (table.tab_p->getType() == TableType::SEPARATE_CHAINING)
			this->tab_p = new HashSC(dynamic_cast<const HashSC&>(*table.tab_p));
		else
			this->tab_p = new HashOA(dynamic_cast<const HashOA&>(*table.tab_p));
	}

	return *this;
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::begin() const
{
	// В зависимости от типа таблицы создаём итератор IteratorSC или IteratorOA

	if (this->tab_p->getType() == TableType::SEPARATE_CHAINING)
	{
		IteratorSC* iter_sc = new IteratorSC(dynamic_cast<const HashSC&>(*this->tab_p).begin());

		return Iterator(iter_sc, TableType::SEPARATE_CHAINING);
	}
	else 
	{
		IteratorOA* iter_oa = new IteratorOA(dynamic_cast<const HashOA&>(*this->tab_p).begin());

		return Iterator(iter_oa, TableType::OPEN_ADDRESSING);
	}
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::end() const
{
	// В зависимости от типа таблицы создаём итератор IteratorSC или IteratorOA

	if (this->tab_p->getType() == TableType::SEPARATE_CHAINING)
	{
		IteratorSC* iter_sc = new IteratorSC(dynamic_cast<const HashSC&>(*this->tab_p).end());

		return Iterator(iter_sc, TableType::SEPARATE_CHAINING);
	}
	else
	{
		IteratorOA* iter_oa = new IteratorOA(dynamic_cast<const HashOA&>(*this->tab_p).end());

		return Iterator(iter_oa, TableType::OPEN_ADDRESSING);
	}
}
//*************************************************************************************************************************************************//


//*************************************************** Реализация методов Iterator *****************************************************************//
//public-методы:
template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const HashTable& table)
{
	// Определяем тип таблицы
	this->type = table.tab_p->getType();

	// Согласно данному типу строим необходимый итератор
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const HashSC&>(*table.tab_p));
	else
		this->iter_p = new IteratorOA(dynamic_cast<const HashOA&>(*table.tab_p));
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const HashTable& table, const Key_T& key)
{
	// Определяем тип таблицы
	this->type = table.tab_p->getType();

	// Согласно данному типу строим необходимый итератор
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const HashSC&>(*table.tab_p), key);
	else
		this->iter_p = new IteratorOA(dynamic_cast<const HashOA&>(*table.tab_p), key);
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::Iterator(const Iterator& iter)
{
	// Определяем тип таблицы
	this->type = iter.type;

	// Согласно данному типу строим необходимый итератор
	if (this->type == TableType::SEPARATE_CHAINING)
		this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
	else
		this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));
}

template <class Key_T, class Data_T> HashTable<Key_T, Data_T>::Iterator::~Iterator()
{
	if (this->iter_p != nullptr)
		delete this->iter_p;
}

template <class Key_T, class Data_T> const typename Key_T& HashTable<Key_T, Data_T>::Iterator::operator*() const
{
	if (this->iter_p == nullptr)
		throw Hash_exception("итератор не установлен");
	else
		return *(*this->iter_p);
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator& HashTable<Key_T, Data_T>::Iterator::operator++()
{
	if (this->iter_p == nullptr)
		throw Hash_exception("итератор не установлен");
	else
		++(*this->iter_p);

	return *this;
}

template <class Key_T, class Data_T> typename HashTable<Key_T, Data_T>::Iterator HashTable<Key_T, Data_T>::Iterator::operator++(int)
{
	Iterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool HashTable<Key_T, Data_T>::Iterator::operator==(const Iterator& iter) const
{
	if (this->iter_p == nullptr && iter.iter_p == nullptr)
		return true;
	else if ((this->iter_p != nullptr && iter.iter_p == nullptr) || (this->iter_p == nullptr && iter.iter_p != nullptr))
		return false;
	else if (this->type != iter.type)
		return false;
	else
		return (*this->iter_p == *iter.iter_p);
}

template <class Key_T, class Data_T> typename bool HashTable<Key_T, Data_T>::Iterator::operator!=(const Iterator& iter) const
{
	if (this->iter_p == nullptr && iter.iter_p == nullptr)
		return false;
	else if ((this->iter_p != nullptr && iter.iter_p == nullptr) || (this->iter_p == nullptr && iter.iter_p != nullptr))
		return true;
	else if (this->type != iter.type)
		return true;
	else
		return (*this->iter_p == *iter.iter_p);
}

template <class Key_T, class Data_T> const typename HashTable<Key_T, Data_T>::Iterator& HashTable<Key_T, Data_T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	// Если данный итератор не был создан
	if (this->iter_p == nullptr)
	{
		// Если iter был создан
		if (iter.iter_p != nullptr)
			// Тогда копируем данный итератор в зависимости от типа таблицы, на которую он установлен
			if (iter.type == TableType::SEPARATE_CHAINING)
				this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
			else
				this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));

		this->type = iter.type;

		return *this;
	}

	// Если итератор iter не был создан
	if (iter.iter_p == nullptr)
	{
		delete this->iter_p;

		this->iter_p = nullptr;
		this->type = iter.type;

		return *this;
	}

	// Если типы итераторов совпадают, то вызываем виртуальный оператор присваивания
	if (this->type == iter.type)
		*this->iter_p = *iter.iter_p;
	// Иначе удаляем текущий итератор и создаём новый, идентичный итератору iter
	else
	{
		delete this->iter_p;

		this->type = iter.type;

		if (this->type == TableType::SEPARATE_CHAINING)
			this->iter_p = new IteratorSC(dynamic_cast<const IteratorSC&>(*iter.iter_p));
		else
			this->iter_p = new IteratorOA(dynamic_cast<const IteratorOA&>(*iter.iter_p));
	}

	return *this;
}
//*************************************************************************************************************************************************//


#endif
