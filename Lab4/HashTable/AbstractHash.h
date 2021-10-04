// AbstarctHash.h
#pragma once
#ifndef _INC_ABSTRACT_HASH_
#define _INC_ABSTRACT_HASH_
// Подключаемые заголовочные файлы
#include "Hash_functions.h"
#include "Hash_exception.h"


//******************************************************* Класс-перечесление типов хеш-таблицы ****************************************************//
enum class TableType
{
	SEPARATE_CHAINING,	// Цепочки коллизий
	OPEN_ADDRESSING		// Открытая адресация
};
//************************************************************************************************************************************************//


//**************************************************** Абстрактный класс хеш-таблицы **************************************************************//
template <class Key_T, class Data_T>
class AbstractHash
{

protected:

	size_t size;			// Размер хеш-таблицы
	size_t count;			// Число элементов в хеш-таблице
	mutable size_t last_op;	// Число проб, выполненных последней итерацией
	
	// Мультипликативная функция хеширования
	size_t _hash(const Key_T& key) const { return mul_hash<Key_T>(key, this->size); }
	// Поиск ближайшей степени двойки большей, чем num
	size_t _clp2(size_t num) const;							

public:
	
	// Конструктор абстрактной части хеш-таблицы по-умолчанию
	AbstractHash(): size(0), count(0), last_op(0) {}
	// Виртуальный деструктор
	virtual ~AbstractHash() {}
	
	// Оператор доступа к элементу по ключу
	virtual Data_T& operator[](const Key_T& key) = 0;
	// Оператор присваивания хеш-таблиц
	virtual const AbstractHash& operator=(const AbstractHash& table) = 0;

	// Получить тип хеш-таблицы
	virtual TableType getType() const = 0;		
	// Получить размер хеш-таблицы
	size_t getSize() const { return this->size; }
	// Получить число элементов в хеш-таблице
	size_t getCount() const { return this->count; }
	// Получить число проб на последней операции
	size_t getLastOp() const { return this->last_op; }
	// Проверить хеш-таблицу на пустоту
	bool isEmpty() const { return this->count == 0; }

	// Очистка хеш-таблицы
	virtual void clear() = 0;
	// Добавить элемент в хеш-таблицу с заданным ключом key и данными data
	virtual void append(const Key_T& key, const Data_T& data) = 0;
	// Удалить элемент из хеш-таблицы по ключу key
	virtual void remove(const Key_T& key) = 0;
	
	// Вывод хеш-таблицы
	virtual void print() const = 0;								

	class AbstractIterator
	{

	public:

		// Виртуальный деструктор
		virtual ~AbstractIterator() {}

		// Оператор "разыменовывания" - получение текущего элемента хеш-таблицы по ссылке
		virtual const Key_T& operator*() const = 0;
		
		// Оператор префиксного инкрементирования
		virtual AbstractIterator& operator++() = 0;	
		
		// Оператор присваивания итераторов
		virtual bool operator==(const AbstractIterator& iter) const = 0;
		// Оператор неравенства итераторов
		virtual bool operator!=(const AbstractIterator& iter) const = 0;
		
		virtual const AbstractIterator& operator=(const AbstractIterator& iter) = 0;
		// Оператор равенства итераторов
	};

};
//*************************************************************************************************************************************************//


//************************************************************* Реализация методов AbstractHash ***************************************************//
// protected-методы:
template <class Key_T, class Data_T> typename size_t AbstractHash<Key_T, Data_T>::_clp2(size_t num) const
{
	size_t result = 1;

	for (; result < num; result <<= 1);

	return result;
}
//*************************************************************************************************************************************************//


#endif
