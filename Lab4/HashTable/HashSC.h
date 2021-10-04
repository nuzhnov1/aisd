// HashSC.h
#pragma once
#ifndef _INC_HASH_SC_
#define _INC_HASH_SC_
// Подключаемые заголовочные файлы
#include <iostream>
#include "AbstractHash.h"
#include "Hash_exception.h"


//**************************************************** Хеш-таблица с цепочками коллизий ***********************************************************//
template <class Key_T, class Data_T>
class HashSC : public AbstractHash<Key_T, Data_T>
{
	
	class Chain;

	using AbstractHash = AbstractHash<Key_T, Data_T>;
	using AbstractIterator = typename AbstractHash::AbstractIterator;
	using Node = typename Chain::Node;

	// Класс цепочки коллизий
	class Chain
	{

		friend class HashSC;

		// Структура узла цепочки
		struct Node
		{
			Key_T key;		// Ключ
			Data_T data;	// Данные
			Node* next;		// Следующий узел

			Node(const Key_T& key, const Data_T& data, Node* next = nullptr)
				: key(key), data(data), next(next) {}
		};

		Node* head;				// Начало цепочки
		mutable size_t last_op;	// Количество просмотров элементов на последней операции				

	public:

		// Конструктор цепочки коллизий по-умолчанию
		Chain() : head(nullptr), last_op(0) {}
		// Конструктор копирования цепочки коллизий
		Chain(const Chain& сhain);
		// Деструктор цепочки коллизий
		~Chain();							

		// Получить количество просмотров на последней операции
		size_t getLastOp() const { return this->last_op; }
		
		// Поиск узла по ключу в цепочке коллизий
		Node* search(const Key_T& key) const;
		
		// Добавление узла в цепочку коллизий
		void append(const Node& node);
		// Удаление узла из цепочки коллизий по заданному ключу
		void remove(const Key_T& key);
		
		// Вывод цепочки коллизий
		void print() const;

	};

	Chain** chains; // Динамический массив цепочек коллизий

	// Копирования хеш-таблиц
	void _copy(const HashSC& src);

public:

	class IteratorSC;

	// Конструктор хеш-таблицы с цепочками коллизий с заданным размером
	HashSC(size_t size);
	// Конструктор копирования хеш-таблицы с цепочками коллизий
	HashSC(const HashSC& table);
	// Деструктор хеш-таблицы с цепочками коллизий
	virtual ~HashSC();

	// Оператор доступа к элементу по ключу
	virtual Data_T& operator[](const Key_T& key) final;
	// Оператор присваивания хеш-таблиц с цепочками коллизий
	virtual const HashSC& operator=(const AbstractHash& table) final;

	// Получить тип хеш-таблицы
	virtual TableType getType() const final { return TableType::SEPARATE_CHAINING; }
	
	// Очистка хеш-таблицы c цепочками коллизий
	virtual void clear() final;
	// Добавить элемент в хеш-таблицу с цепочками коллизий по заданному ключу key и данными data
	virtual void append(const Key_T& key, const Data_T& data) final;
	// Удалить элемент из хеш-таблицы с цепочками коллизий по ключу key
	virtual void remove(const Key_T& key) final;
	
	// Вывод хеш-таблицы с цепочками коллизий
	virtual void print() const final;

	// Установка итератора на начало хеш-таблицы
	IteratorSC begin() const;
	// Установка итератора в "неустановленное" состояние
	IteratorSC end() const { return IteratorSC(*this); }

	// Класс итератора хеш-таблицы с цепочками коллизий
	class IteratorSC : public AbstractIterator
	{

		friend class HashSC;

		const HashSC* table;	// Указатель на хеш-таблицу
		size_t pos;				// Номер текущей цепочки
		const Node* cur;		// Указатель на текущий элемент в цепочке

		// private-конструктор итератора с параметрами: table - указатель на хеш-таблицу с цепочками коллизий,
		// pos - номером текущей цепочки и node - указателем на текущий элемент в цепочке
		IteratorSC(const HashSC* table, size_t pos, const Node* node) : table(table), pos(pos), cur(node) {}

	public:

		// Конструктор итератора по умолчанию
		IteratorSC() : table(nullptr), pos(-1), cur(nullptr) {}
		// Конструктор итератора с ссылкой на хеш-таблицу
		IteratorSC(const HashSC& table) : table(&table), pos(-1), cur(nullptr) {}
		// Конструктор итератора с ссылкой на хеш-таблицу и с ключом
		IteratorSC(const HashSC& table, const Key_T& key);
		// Конструктор копирования итератора
		IteratorSC(const IteratorSC& iter) : table(iter.table), pos(iter.pos), cur(iter.cur) {}

		// Оператор "разыменовывания" - получение текущего элемента хеш-таблицы по ссылке
		virtual const Key_T& operator*() const final;

		// Оператор префиксного инкрементирования
		virtual IteratorSC& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const AbstractIterator& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const AbstractIterator& iter) const final;

		// Оператор присваивания итераторов
		virtual const IteratorSC& operator=(const AbstractIterator& iter) final;

	};

};
//*************************************************************************************************************************************************//


//*************************************************** Реализация методов Chain ********************************************************************//
// public-методы:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::Chain::Chain(const Chain& chain)
{
	Node* node_copy = chain.head;

	// Обходим цепочку chain, добавляя её узлы в данную
	if (node_copy != nullptr)
	{
		Node* node_this = this->head = new Node(*node_copy);

		for (node_copy = node_copy->next; node_copy != nullptr; node_this = node_this->next, node_copy = node_copy->next)
			node_this->next = new Node(*node_copy);
	}
	else
		this->head = nullptr;

	this->last_op = 0;
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::Chain::~Chain()
{
	Node* cur = this->head;
	Node* prev = nullptr;

	// Обходим цепочку, удаляя её узлы
	while (cur != nullptr)
	{
		prev = cur;
		cur = cur->next;

		delete prev;
	}
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::Chain::Node* HashSC<Key_T, Data_T>::Chain::search(const Key_T& key) const
{
	Node* cur = this->head;

	this->last_op = 0;

	// Обходим цепочку по не встретим узел с ключом, равным key, или пока не дойдём до конца
	for (; cur != nullptr && cur->key != key; cur = cur->next, this->last_op++);

	return cur;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::append(const Node& node)
{
	// Проверяем есть ли уже элемент с таким ключом в цепочке
	if (search(node.key) != nullptr)
		throw Hash_exception("элемент с таким ключом уже есть в хеш-таблице");

	// Если нет, то добавляем в начало цепочки
	Node* temp = this->head;
	this->head = new Node(node);
	this->head->next = temp;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::remove(const Key_T& key)
{
	Node* cur = this->head;
	Node* prev = nullptr;
	
	this->last_op = 0;

	// Обходим цепочку по не встретим узел с ключом, равным key, или пока не дойдём до конца
	for (; cur != nullptr && cur->key != key; prev = cur, cur = cur->next, this->last_op++);

	// Если дошли до конца цепочки, значит в ней нет узла с ключом, равным key
	if (cur == nullptr)
		throw Hash_exception("элемента с таким ключом нет в хеш-таблице");
	// Если вышли из цикла раньше, т.н. не дошли до конца, то встретили элемент с ключом, равным key
	else
	{
		// Если удаляем головной элемент, то головным элементов цепочки становится следующий элемент
		if (prev == nullptr)
			this->head = this->head->next;
		// Иначе связываем соседей удаляемого узла
		else
			prev->next = cur->next;

		delete cur;
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::Chain::print() const
{
	using std::cout;
	using std::endl;

	for (Node* cur = this->head; cur != nullptr; cur = cur->next)
		cout << cur->key << " -> ";

	cout << "null" << endl;
}
//*************************************************************************************************************************************************//


//*************************************************** Реализация методов HashSC *******************************************************************//
// private-методы:
template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::_copy(const HashSC& table)
{
	for (size_t i = 0; i < table.size; i++)
		if (table.chains[i] == nullptr)
			this->chains[i] = nullptr;
		else
			this->chains[i] = new Chain(*table.chains[i]);

	this->size = table.size;
	this->count = table.count;
	this->last_op = 0;
}


// public-методы:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::HashSC(size_t size)
{
	this->size = HashSC::_clp2(size);
	this->chains = new Chain*[this->size]();
	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::HashSC(const HashSC& table)
{
	this->chains = new Chain * [table.size];

	_copy(table);
}

template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::~HashSC()
{
	clear();

	delete[] this->chains;
}

template <class Key_T, class Data_T> typename Data_T& HashSC<Key_T, Data_T>::operator[](const Key_T& key)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// Если по данному хешу в таблице вообще нет элементов
	if (this->chains[i] == nullptr)
		throw Hash_exception("элемента с таким ключом нет в хеш-таблице");

	// Если есть, хотя бы один элемент
	Node* node = this->chains[i]->search(key);

	this->last_op += this->chains[i]->getLastOp();

	if (node == nullptr)
		throw Hash_exception("элемента с таким ключом нет в хеш-таблице");
	else
		return node->data;
}

template <class Key_T, class Data_T> const typename HashSC<Key_T, Data_T>& HashSC<Key_T, Data_T>::operator=(const AbstractHash& table)
{
	const HashSC& table_sc = dynamic_cast<const HashSC&>(table);

	if (this == &table_sc)
		return *this;

	clear();

	delete[] this->chains;
	
	this->chains = new Chain * [table_sc.size];

	_copy(table_sc);

	return *this;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::clear()
{
	for (size_t i = 0; i < this->size; i++)
	{
		if (this->chains[i] != nullptr)
		{
			delete this->chains[i];

			this->chains[i] = nullptr;
		}
	}
	
	this->count = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// Если по данному хешу нет вообще элементов
	if (this->chains[i] == nullptr)
	{
		// Создаём цепочку
		this->chains[i] = new Chain();
		// И добавляем в неё элемент
		this->chains[i]->append(Node(key, data, nullptr));
		this->count++;
	}
	// Если есть хотя бы один элемент
	else
	{
		// Пытаемся добавить в эту цепочку данный элемент
		try
		{
			this->chains[i]->append(Node(key, data, nullptr));
			// Успешно добавили
			this->last_op += this->chains[i]->getLastOp();
			this->count++;
		}
		// Если во время добавления в цепочку возникло исключение, то в last_op запоминаем число операций
		// и снова выбрасываем это исключение
		catch (Hash_exception&)
		{
			this->last_op += this->chains[i]->getLastOp();
			
			throw;
		}
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::remove(const Key_T& key)
{
	size_t i = HashSC::_hash(key);

	this->last_op = 1;

	// Если по данному хешу нет вообще элементов
	if (this->chains[i] == nullptr)
		throw Hash_exception("элемента с таким ключом нет в хеш-таблице");

	// Если есть хотя бы один элемент, то пытаемся найти элемент с ключом key и удалить его
	try
	{
		this->chains[i]->remove(key);
		// Успешно удалили
		this->last_op += this->chains[i]->getLastOp();
		// Если в цепочке не осталось элементов, то удаляем её
		if (this->chains[i]->head == nullptr)
		{
			
			delete this->chains[i];

			this->chains[i] = nullptr;
		}
		this->count--;
	}
	// Если во время удаления из цепочки возникло исключение, то в last_op запоминаем число операций
	// и снова выбрасываем это исключение
	catch (Hash_exception&)
	{
		this->last_op += this->chains[i]->getLastOp();
		
		throw;
	}
}

template <class Key_T, class Data_T> typename void HashSC<Key_T, Data_T>::print() const
{
	using std::cout;
	using std::endl;

	for (size_t i = 0; i < this->size; i++)
	{
		cout << "[" << i << "]: ";

		if (this->chains[i] == nullptr)
			cout << "null" << endl;
		else
			this->chains[i]->print();
	}
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::IteratorSC HashSC<Key_T, Data_T>::begin() const
{
	// Если в хеш-таблице нет элементов
	if (this->count == 0)
		return IteratorSC(*this);

	// Ищем первую непустую цепочку
	size_t i = 0;

	for (; i < this->size; i++)
		if (this->chains[i] != nullptr)
			break;

	return IteratorSC(this, i, this->chains[i]->head);
}
//*************************************************************************************************************************************************//


//**************************************************** Реализация методов класса IteratorSC *******************************************************//
// public-методы:
template <class Key_T, class Data_T> HashSC<Key_T, Data_T>::IteratorSC::IteratorSC(const HashSC& table, const Key_T& key)
{
	this->table = &table;
	// Ищем номер цепочки
	size_t& i = this->pos = table._hash(key);

	// Если по данному хешу нет элементов
	if (table.chains[i] == nullptr)
	{
		this->pos = -1;
		this->cur = nullptr;

		return;
	}

	// Если есть хотя бы один, то пытаемся найти элемент в этой цепочки с ключом, равным key
	this->cur = table.chains[i]->search(key);

	// Если элемент с данным ключом не был найден
	if (this->cur == nullptr)
		this->pos = -1;
}

template <class Key_T, class Data_T> const typename Key_T& HashSC<Key_T, Data_T>::IteratorSC::operator*() const
{
	if (this->cur == nullptr)
		throw Hash_exception("итератор не установлен");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename HashSC<Key_T, Data_T>::IteratorSC& HashSC<Key_T, Data_T>::IteratorSC::operator++()
{
	if (this->cur == nullptr)
		throw Hash_exception("итератор не установлен");

	// Если следующий элемент в цепочке есть, то переходим к нему
	if (this->cur->next != nullptr)
		this->cur = this->cur->next;
	// Если его нет
	else
	{
		// Переходим к следующей цепочке
		size_t& i = this->pos;
		i++;

		// Ищем первую непустую цепочку
		for (; i < this->table->size; i++)
			if (this->table->chains[i] != nullptr)
				break;

		// Если следующей цепочки нет, то итератор переходит в неустановленное состояние
		if (i == this->table->size)
		{
			this->pos = -1;
			this->cur = nullptr;
		}
		// Если есть, то текущий элемент будет головным элементом найденной цепочки
		else
			this->cur = this->table->chains[i]->head;
	}

	return *this;
}

template <class Key_T, class Data_T> typename bool HashSC<Key_T, Data_T>::IteratorSC::operator==(const AbstractIterator& iter) const
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	return this->table == iter_sc.table && this->pos == iter_sc.pos && this->cur == iter_sc.cur;
}

template <class Key_T, class Data_T> typename bool HashSC<Key_T, Data_T>::IteratorSC::operator!=(const AbstractIterator& iter) const
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	return !(this->table == iter_sc.table && this->pos == iter_sc.pos && this->cur == iter_sc.cur);
}

template <class Key_T, class Data_T> const typename HashSC<Key_T, Data_T>::IteratorSC& HashSC<Key_T, Data_T>::IteratorSC::operator=(const AbstractIterator& iter)
{
	const typename HashSC<Key_T, Data_T>::IteratorSC& iter_sc = dynamic_cast<const HashSC<Key_T, Data_T>::IteratorSC&>(iter);

	if (this == &iter_sc)
		return *this;

	this->table = iter_sc.table;
	this->pos = iter_sc.pos;
	this->cur = iter_sc.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
