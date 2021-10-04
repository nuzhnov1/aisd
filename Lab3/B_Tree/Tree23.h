// Tree23.h
#pragma once
#ifndef _INC_TREE23_
#define _INC_TREE23_
// Подключаемые заголовочные файлы
#include <iostream>
#include <string>


//*************************************************************** Класс исключений ****************************************************************//
class Tree23_exception
{
	
	using string = std::string;

	string error;

public:

	Tree23_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


//*********************************************************** Класс АТД "2-3 дерево" **************************************************************//
template <class Key_T, class Data_T>
class Tree23
{

	// Класс-перечисление типов узлов 2-3 дерева
	enum class Node_Type
	{

		LEAF,			// Лист
		INTERNAL_NODE	// Внутренний узел
	
	};

	// Абстрактный класс узла 2-3-дерева
	class Node
	{

	public:

		// Виртуальный деструктор для корректного освобождения памяти от дочерних узлов
		virtual ~Node() {}

		// Получить тип узла
		virtual Node_Type getNodeType() const = 0;

	};

	// Класс внутреннего узла
	class InternalNode : public Node
	{

	public:

		Key_T key2, key3;			// Дубликаты минимальных ключей в 2 и 3 поддеревьях
		Node* son1, * son2, * son3;	// Указатели на трёх сыновей

		// Конструктор внутреннего узла по умолчанию
		InternalNode() : key2(), key3(), son1(nullptr), son2(nullptr), son3(nullptr) {}

		// Получить тип узла
		virtual Node_Type getNodeType() const final { return Node_Type::INTERNAL_NODE; }

	};

	// Класс листа
	class Leaf : public Node
	{
	
	public:
		
		Key_T key;		// Ключ
		Data_T data;	// Данные

		// Конструктор листа с параметрами
		Leaf(const Key_T& key, const Data_T& data) : key(key), data(data) {}

		// Получить тип узла
		virtual Node_Type getNodeType() const final { return Node_Type::LEAF; }

	};

	size_t size;			// Количество элементов в дереве
	InternalNode* root;		// Указатель на корень дерева
	mutable size_t last_op;	// Количество переборов на последней операции

	// Реализация алгоритма поиска узла в 2-3 дереве
	Leaf* _search(const Key_T& key, const InternalNode* cur) const;
	// Реализация алгоритма копирования 2-3 дерева
	InternalNode* _copy(const InternalNode* src) const;
	// Реализация алгоритма очистки 2-3 дерева
	void _clear(InternalNode* root) const;
	// Реализация алгоритма вставки элемента в 2-3 дерево
	bool _insert(const Key_T& key, const Data_T& data);
	// Вспомогательная функция вставки для основной - _insert
	bool _insert1(Node* cur, Leaf* leaf, Node*& nroot, Key_T& kroot) const;
	// Реализация алгоритма удаления элемента из 2-3 дерева
	bool _delete(const Key_T& key);
	// Вспомогательная функция удаления для основной - _delete
	bool _delete1(InternalNode* cur, const Key_T& key, Leaf*& low1, bool& one_son) const;
	// Реализация алгоритма вывода 2-3 дерева в консоль в древовидной форме
	void _print(const Node* root, size_t level = 0) const;
	// Получить ближайшего левого родителя cur. Левым родителем может быть также средний узел
	InternalNode* _getLeftParent(const Leaf* leaf) const;
	// Получить ближайшего правого родителя cur. Правым родителем может быть также средний узел
	InternalNode* _getRightParent(const Leaf* leaf) const;
	// Получить минимальный элемент поддерева с корнем root
	Leaf* _getMin(Node* root) const;
	// Получить максимальный элемент поддерева с корнем root
	Leaf* _getMax(Node* root) const;

public:

	class Iterator;
	class RIterator;

	// Конструктор 2-3 дерева по умолчанию
	Tree23() : root(nullptr), size(0), last_op(0) {}
	// Конструктор копирования 2-3 дерева
	Tree23(const Tree23& tree) : 
		root(nullptr), size(tree.size), last_op(0) { this->root = _copy(tree.root); }
	// Деструктор 2-3 дерева
	~Tree23() { clear(); }

	// Оператор доступа к элементу по ключу
	Data_T& operator[](const Key_T& key);
	const Data_T& operator[](const Key_T& key) const { return const_cast<Tree23&>(*this)[key]; }
	// Оператор присваивания деревьев
	const Tree23& operator=(const Tree23& tree);

	// Получить размер дерева
	size_t getSize() const { return this->size; }
	// Получить количество переборов на последней операции
	size_t getLastOp() const { return this->last_op; }

	// Чтение элемента дерева по ключу
	const Data_T& getItemByKey(const Key_T& key) const { return const_cast<Tree23&>(*this)[key]; }
	// Запись элемента по заданному ключу
	void setItemByKey(const Key_T& key, const Data_T& data) { (*this)[key] = data; }

	// Проверка дерева на пустоту
	bool isEmpty() const { return this->size == 0; }
	// Очистка дерева
	void clear();
	// Добавить элемент с заданным ключом и данными в дерево
	void append(const Key_T& key, const Data_T& data);
	// Удалить элемент с заданным ключом
	void remove(const Key_T& key);

	// Вывести дерево в древовидной форме
	void print() const { _print(this->root); }
	// Вывести дерево в форме словаря: ключ-значение
	void printAsDict() const;

	// Установка итератора на минимальный элемент дерева
	Iterator begin() const { return Iterator(this, _getMin(this->root)); }
	// Установка итератора в "неустановленное" состояние
	Iterator end() const { return Iterator(*this); }
	// Установка обратного итератор на максимальный элемент списка
	RIterator rbegin() const { return RIterator(this, _getMax(this->root)); }
	// Установка обратного итератора в "неустановленное" состояние
	RIterator rend() const { return RIterator(*this); }

	// Класс прямого итератора
	class Iterator
	{

		friend class Tree23;

		const Tree23* tree;	// Указатель на текущее 2-3 дерево
		const Leaf* cur;	// Текущий лист, на который установлен итератор

		// private-конструктор прямого итератора с параметрами: tree - указатель на дерево, leaf - указатель на лист
		Iterator(const Tree23* tree, const Leaf* leaf) : tree(tree), cur(leaf) {}

	public:

		// Конструктор прямого итератора по умолчанию
		Iterator() : tree(nullptr), cur(nullptr) {}
		// Конструктор прямого итератора с параметром tree - ссылкой на 2-3 дерево
		Iterator(const Tree23& tree) : tree(&tree), cur(nullptr) {}
		// Конструктор прямого итератора с параметрами: tree - ссылка на 2-3 дерево, key - ключом
		Iterator(const Tree23& tree, const Key_T& key);
		// Конструктор копирования прямого итератора
		Iterator(const Iterator& iter) : tree(iter.tree), cur(iter.cur) {}

		// Оператор "разыменовывания" - получение текущего узла дерева по ссылке
		const Key_T& operator*() const;

		// Оператор префиксного инкрементирования прямого итератора
		Iterator& operator++();
		// Оператор префиксного декрементирования прямого итератора
		Iterator& operator--();
		// Оператор постфиксного инкрементирования прямого итератора
		Iterator operator++(int);
		// Оператор постфиксного декрементирования прямого итератора
		Iterator operator--(int);

		// Оператор равенства прямых итераторов
		bool operator==(const Iterator& iter) const;
		// Оператор неравенства прямых итераторов
		bool operator!=(const Iterator& iter) const;

		// Оператор присваивания прямого итератора
		const Iterator& operator=(const Iterator& iter);

	};

	// Класс обратного итератора
	class RIterator
	{

		friend class Tree23;

		const Tree23* tree;	// Указатель на текущее 2-3 дерево
		const Leaf* cur;	// Текущий лист, на который установлен итератор

		// private-конструктор обратного итератора с параметрами: tree - указатель на дерево, leaf - указатель на лист
		RIterator(const Tree23* tree, const Leaf* leaf) : tree(tree), cur(leaf) {}

	public:

		// Конструктор обратного итератора по умолчанию
		RIterator() : tree(nullptr), cur(nullptr) {}
		// Конструктор обратного итератора с параметром tree - ссылкой на 2-3 дерево
		RIterator(const Tree23& tree) : tree(&tree), cur(nullptr) {}
		// Конструктор обратного итератора с параметрами: tree - ссылка на 2-3 дерево, key - ключом
		RIterator(const Tree23& tree, const Key_T& key);
		// Конструктор копирования обратного итератора
		RIterator(const RIterator& iter) : tree(iter.tree), cur(iter.cur) {}

		// Оператор "разыменовывания" - получение текущего элемента узла дерева по ссылке
		const Key_T& operator*() const;

		// Оператор префиксного инкрементирования обратного итератора
		RIterator& operator++();
		// Оператор префиксного декрементирования обратного итератора
		RIterator& operator--();
		// Оператор постфиксного инкрементирования обратного итератора
		RIterator operator++(int);
		// Оператор постфиксного декрементирования обратного итератора
		RIterator operator--(int);

		// Оператор равенства обратных итераторов
		bool operator==(const RIterator& iter) const;
		// Оператор неравенства обратных итераторов
		bool operator!=(const RIterator& iter) const;

		// Оператор присваивания обратного итератора
		const RIterator& operator=(const RIterator& iter);

	};

};
//***************************************************************************************************************************************I*********//


//********************************************************* Реализация методов Tree23 *************************************************************//
// private-методы:
template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_search(const Key_T& key, const InternalNode* cur) const
{
	this->last_op++;

	// Случай, если поддерево пусто

	if (cur == nullptr)
		return nullptr;

	// Случай, если сыновья поддерева - листы

	if (cur->son1->getNodeType() == Node_Type::LEAF)
	{
		// Проверяем всех сыновей на совпадения ключа с key

		if (dynamic_cast<Leaf*>(cur->son1)->key == key)
			return dynamic_cast<Leaf*>(cur->son1);
		else if (cur->son2 != nullptr && dynamic_cast<Leaf*>(cur->son2)->key == key)
			return dynamic_cast<Leaf*>(cur->son2);
		else if (cur->son3 != nullptr && dynamic_cast<Leaf*>(cur->son3)->key == key)
			return dynamic_cast<Leaf*>(cur->son3);
		else
			return nullptr;
	}

	// Случай, если сыновья поддерева - внутренние узлы

	// Случай перехода к первому сыну
	if (key < cur->key2)
		return _search(key, dynamic_cast<InternalNode*>(cur->son1));
	// Случай перехода к третьему сыну
	else if (cur->son3 != nullptr && key >= cur->key3)
		return _search(key, dynamic_cast<InternalNode*>(cur->son3));
	// Случай перехода к второму сыну
	else
		return _search(key, dynamic_cast<InternalNode*>(cur->son2));
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_copy(const InternalNode* src) const
{
	// Случай, если корня поддерева src нет
	if (src == nullptr)
		return nullptr;

	// Случай, если корень поддерева src есть

	InternalNode* inode = new InternalNode();
	inode->key2 = src->key2;
	inode->key3 = src->key3;

	// Случай, если сыновья корня src - листы
	if (src->son1->getNodeType() == Node_Type::LEAF)
	{
		inode->son1 = new Leaf(dynamic_cast<Leaf*>(src->son1)->key, dynamic_cast<Leaf*>(src->son1)->data);
		if (src->son2 != nullptr)
			inode->son2 = new Leaf(dynamic_cast<Leaf*>(src->son2)->key, dynamic_cast<Leaf*>(src->son2)->data);
		if (src->son3 != nullptr)
			inode->son3 = new Leaf(dynamic_cast<Leaf*>(src->son3)->key, dynamic_cast<Leaf*>(src->son3)->data);
	}
	// Случай, если сыновья корня src - внутренние узлы
	else
	{
		inode->son1 = _copy(dynamic_cast<InternalNode*>(src->son1));
		inode->son2 = _copy(dynamic_cast<InternalNode*>(src->son2));
		inode->son3 = _copy(dynamic_cast<InternalNode*>(src->son3));
	}

	return inode;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::_clear(InternalNode* root) const
{
	// Если поддерево/дерево и так пустое, то ничего не делаем
	if (root == nullptr)
		return;

	// Случай, если сыновья поддерева - листы
	if (root->son1->getNodeType() == Node_Type::LEAF)
	{
		delete root->son1;
		if (root->son2 != nullptr)
			delete root->son2;
		if (root->son3 != nullptr)
			delete root->son3;
		delete root;
	}
	// Случай, если сыновья поддерева - внутренние узлы
	else
	{
		_clear(dynamic_cast<InternalNode*>(root->son1));
		_clear(dynamic_cast<InternalNode*>(root->son2));
		_clear(dynamic_cast<InternalNode*>(root->son3));
		delete root;
	}
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_insert(const Key_T& key, const Data_T& data)
{
	// Сразу создаём лист, если добавление не произойдёт, то он будет удалён
	Leaf* leaf = new Leaf(key, data); 

	// Случай, если элементов ещё нет
	if (this->root == nullptr)
	{
		this->root = new InternalNode();
		this->root->son1 = leaf;
		this->size++;
		this->last_op++;

		return true;
	}

	// Случай, если уже имеется один элемент
	if (this->root->son2 == nullptr)
	{
		Leaf* son1 = dynamic_cast<Leaf*>(this->root->son1);

		this->last_op++;

		// В зависимости от значения ключа key вставляем или не вставляем новый элемент
		if (key < son1->key)
		{
			this->root->son2 = son1;
			this->root->son1 = leaf;
			this->root->key2 = son1->key;
			this->size++;

			return true;
		}
		else if (key == son1->key)
		{
			delete leaf;

			return false;
		}
		else
		{
			this->root->son2 = leaf;
			this->root->key2 = leaf->key;
			this->size++;

			return true;
		}
	}

	Node* nroot = nullptr;	// Указатель на возможный новый корень
	Key_T kroot;			// Минимальный ключ в поддереве этого корня
	bool is_inserted;		// Был ли вставлен элемент

	// Остальные случаи вставки в дерево
	is_inserted = _insert1(this->root, leaf, nroot, kroot);
	
	if (is_inserted == false) 
	{
		delete leaf;

		return false;
	}

	this->size++;

	// Если был создан братский корень относительно исходного корня
	if (nroot != nullptr)
	{
		// То создаём корень более высокого порядка, сыновьями которого будут корень исходного дерева(первый)
		// и новый братский ему корень(второй сын)
		InternalNode* temp = this->root;

		this->root = new InternalNode();
		this->root->son1 = temp;
		this->root->son2 = nroot;
		this->root->key2 = kroot;
		
		this->last_op++;
	}

	return true;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_insert1(Node* cur, Leaf* leaf, Node*& nroot, Key_T& kroot) const
{
	// cur - текущий корень 2-3 дерева/поддерева
	// leaf - ранее созданный лист
	// nroot - выходной параметр. Ссылка на новый братский корень относительно cur
	// kroot - выходной параметр. Минимальный ключ поддерева с корнем nroot

	nroot = nullptr;
	this->last_op++;

	// Случай, если cur - лист
	if (cur->getNodeType() == Node_Type::LEAF)
	{
		// Случай, если элемент с таким ключом уже есть в дереве
		if (dynamic_cast<Leaf*>(cur)->key == leaf->key)
			return false;
		else
		{
			// В иных случаях, сохраняем в nroot новый узел leaf
			nroot = leaf;
			// Если ключ вставляемого элемента больше чем у cur, то в kroot сохраняем его ключ
			if (dynamic_cast<Leaf*>(cur)->key < leaf->key)
				kroot = leaf->key;
			else
			{
				// Иначе переставляем эти узлы: в cur записываем вставляемый элемент, а в leaf - текущий элемент cur

				Key_T key_temp = dynamic_cast<Leaf*>(cur)->key;
				Data_T data_temp = dynamic_cast<Leaf*>(cur)->data;

				kroot = key_temp;
				dynamic_cast<Leaf*>(cur)->key = leaf->key;
				dynamic_cast<Leaf*>(cur)->data = leaf->data;
				leaf->key = key_temp;
				leaf->data = data_temp;
			}

			return true;
		}
	}

	// Случай, если cur - внутренний узел

	unsigned char child = 0;							// Номер выбираемого сына
	bool is_inserted = false;							// Была ли вставка
	Node* tbk = nullptr;								// Братский узел для выбранного ниже сына cur
	Key_T lbk = dynamic_cast<InternalNode*>(cur)->key2; // Минимальный ключ в поддереве с корнем tbk

	// Осуществляем вставку в поддерево cur

	// Случай вставки в дерево первого сына
	if (leaf->key < dynamic_cast<InternalNode*>(cur)->key2) 
	{
		child = 1;
		is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son1, leaf, tbk, lbk);
	}
	// Случай вставки в дерево третьего сына
	else if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr && leaf->key >= dynamic_cast<InternalNode*>(cur)->key3)
	{
		child = 3;
		is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son3, leaf, tbk, lbk);
	}
	// Случай вставки в корень второго сына
	else
	{
		child = 2;
		is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son2, leaf, tbk, lbk);
	}

	// Случай, если вставка элемента прошла успешно 
	// и был создан братский узел для сына cur, в поддерево которого был вставлен элемент
	if (is_inserted && tbk != nullptr)
	{
		// Случай, если у узла только два сына
		if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr)
		{
			// Случай, если вставки осуществлялась во вторую ветвь
			if (child == 2)
			{
				dynamic_cast<InternalNode*>(cur)->son3 = tbk;
				dynamic_cast<InternalNode*>(cur)->key3 = lbk;
			}
			// Случай, если вставка осуществлялась в первую ветвь
			else
			{
				dynamic_cast<InternalNode*>(cur)->son3 = dynamic_cast<InternalNode*>(cur)->son2;
				dynamic_cast<InternalNode*>(cur)->key3 = dynamic_cast<InternalNode*>(cur)->key2;
				dynamic_cast<InternalNode*>(cur)->son2 = tbk;
				dynamic_cast<InternalNode*>(cur)->key2 = lbk;
			}
		}
		// Случай, если у узла есть ещё третий сын
		else
		{
			// Создаём братский узел для cur
			nroot = new InternalNode();
			this->last_op++;

			// Случай, если вставка осуществлялась в третью ветвь
			if (child == 3)
			{
				dynamic_cast<InternalNode*>(nroot)->son1 = dynamic_cast<InternalNode*>(cur)->son3;
				dynamic_cast<InternalNode*>(nroot)->son2 = tbk;
				dynamic_cast<InternalNode*>(nroot)->key2 = lbk;
				kroot = dynamic_cast<InternalNode*>(cur)->key3;
			}
			// Случай, если вставка осуществлялась в первую или вторую ветвь
			else
			{
				dynamic_cast<InternalNode*>(nroot)->son2 = dynamic_cast<InternalNode*>(cur)->son3;
				dynamic_cast<InternalNode*>(nroot)->key2 = dynamic_cast<InternalNode*>(cur)->key3;

				// Случай, если вставка осуществлялась во вторую ветвь
				if (child == 2)
				{
					dynamic_cast<InternalNode*>(nroot)->son1 = tbk;
					kroot = lbk;
				}
				// Случай, если вставка осуществлялась в первую ветвь
				else
				{
					dynamic_cast<InternalNode*>(nroot)->son1 = dynamic_cast<InternalNode*>(cur)->son2;
					dynamic_cast<InternalNode*>(cur)->son2 = tbk;
					kroot = dynamic_cast<InternalNode*>(cur)->key2;
					dynamic_cast<InternalNode*>(cur)->key2 = lbk;
				}
			}

			dynamic_cast<InternalNode*>(cur)->son3 = nullptr;
		}
	}
	
	return is_inserted;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_delete(const Key_T& key)
{
	Leaf* tmin;
	bool is_one;
	bool is_deleted;

	// Случай, если дерево пустое
	if (this->root == nullptr)
	{
		this->last_op++;

		return false;
	}

	// Случай, если в дереве один элемент
	if (this->root->son2 == nullptr)
	{
		this->last_op++;

		if (dynamic_cast<Leaf*>(this->root->son1)->key == key)
		{
			delete this->root->son1;
			delete this->root;
			this->root = nullptr;
			this->size--;

			return true;
		}
		else
			return false;
	}

	// Иные случаи
	is_deleted = _delete1(this->root, key, tmin, is_one);

	if (is_deleted == true)
	{
		this->size--;

		// Если в дереве, после удаления остался один элемент, и при этом первый сын корня - внутренний узел
		if (is_one == true && this->root->son1->getNodeType() == Node_Type::INTERNAL_NODE)
		{
			InternalNode* temp = dynamic_cast<InternalNode*>(this->root->son1);

			delete this->root;
			this->root = temp;
			this->last_op++;
		}
	}
	
	return is_deleted;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_delete1(InternalNode* cur, const Key_T& key, Leaf*& low1, bool& one_son) const
{
	// cur - текущий корень дерева/поддерева
	// key - ключ удаляемого элемента
	// low1 - возвращаемый адрес узла с минимальным ключом в первом поддереве
	// one_son - возвращаемый признак узла с одним сыном

	low1 = nullptr;
	one_son = false;
	this->last_op++;

	// Случай, если сыновья текущего узла - листы
	if (cur->son1->getNodeType() == Node_Type::LEAF)
	{
		// В таком случае ищем среди сыновей, тот, у которого совпадает ключ с key
		// Если сын найден, то удаляем его и смещаем старших сыновей к нему
		// Если не найден, то возвращаем false

		if (dynamic_cast<Leaf*>(cur->son1)->key == key)
		{
			delete cur->son1;
			cur->son1 = cur->son2;
			cur->son2 = cur->son3;
			cur->son3 = nullptr;
			cur->key2 = cur->key3;
		}
		else if (dynamic_cast<Leaf*>(cur->son2)->key == key)
		{
			delete cur->son2;
			cur->son2 = cur->son3;
			cur->son3 = nullptr;
			cur->key2 = cur->key3;
		}
		else if (cur->son3 != nullptr && dynamic_cast<Leaf*>(cur->son3)->key == key)
		{
			delete cur->son3;
			cur->son3 = nullptr;
		}
		else
			return false;

		// После успешного удаления сына в low1 записываем адрес младшего сына в поддерево с корнем cur
		// И также, если остался только один сын, то возвращаем этот признак через one_son
		low1 = dynamic_cast<Leaf*>(cur->son1);
		if (cur->son2 == nullptr)
			one_son = true;

		return true;
	}

	// Если сыновья являются внутренними узлами

	InternalNode* w = nullptr;	// Выбираемый сын, в котором будет происходить удаление
	Leaf* low1_bk = nullptr;	// Узел с минимальный ключом в первом поддереве выбранного узла
	unsigned char child = 0;	// Номер выбранного сына: 1, 2 или 3
	bool one_son_bk = false;	// Признак того, что при удалении в одном из внутренних узлов 
								// остался только один сын-лист

	// В зависимости от ключа key, переходим к какому-либо сыну
	if (key < cur->key2)
	{
		child = 1;
		w = dynamic_cast<InternalNode*>(cur->son1);
	}
	else if (cur->son3 != nullptr && key >= cur->key3)
	{
		child = 3;
		w = dynamic_cast<InternalNode*>(cur->son3);
	}
	else
	{
		child = 2;
		w = dynamic_cast<InternalNode*>(cur->son2);
	}

	// Перейдя к какому-либо сыну, пытаемся удалить элемент с ключом key в его поддереве
	// Если элемента с ключом key нет в дереве, то выходим из функции и возвращаем false
	if (_delete1(w, key, low1_bk, one_son_bk) == false)
		return false;
	
	// Случай, если удаление произошло успешно

	// Сначала в текущем узле меняем ключи, т.н. если удалялся элемент из второй ветви,
	// то ключ минимального элемента этой ветви, записываем в поле key2 текущего внутреннего узла cur
	// Если удалялся элемент из третьей ветви, то записываем в поле key3
	low1 = low1_bk;
	if (low1_bk != nullptr)
	{
		if (child == 2)
		{
			cur->key2 = low1_bk->key;
			low1 = nullptr;
		}
		else if (child == 3)
		{
			cur->key3 = low1_bk->key;
			low1 = nullptr;
		}
	}

	// Если после удаления элемента не возникло ситуации, когда внутренний узел имеет менее двух сыновей,
	// то выходим из функции и возвращаем true
	if (one_son_bk == false)
		return true;

	// Случай, если в каком-либо сыновьем внутреннем узле cur остался только один сын
	
	InternalNode* y = nullptr;
	InternalNode* z = nullptr;

	// Переставляем ветви потомков cur в зависимости от того, какой узел и где удаляли

	// Случай, если удаление было у первого сына cur и у него остался один сын
	if (child == 1)
	{
		// w - это первый сын cur: w == cur->son1
		// y - это второй сын cur: y == cur->son2
		y = dynamic_cast<InternalNode*>(cur->son2);

		// Случай, если у второго сына cur есть собственный третий сын
		if (y->son3 != nullptr)
		{
			w->son2 = y->son1;
			w->key2 = cur->key2;
			cur->key2 = y->key2;
			y->son1 = y->son2;
			y->son2 = y->son3;
			y->key2 = y->key3;
			y->son3 = nullptr;
		}
		// Случай, если у второго сына cur нет собственного третьего сына
		else
		{
			y->son3 = y->son2;
			y->key3 = y->key2;
			y->son2 = y->son1;
			y->key2 = cur->key2;
			y->son1 = w->son1;
			delete w;
			cur->son1 = cur->son2;
			cur->son2 = cur->son3;
			cur->key2 = cur->key3;
			cur->son3 = nullptr;
			if (cur->son2 == nullptr)
				one_son = true;
		}
	}
	// Случай, если удаление было у второго сына cur, и у него остался один сын
	else if (child == 2)
	{
		// w - второй сын cur: w == cur->son2
		// y - это первый сын cur: y == cur->son1
		y = dynamic_cast<InternalNode*>(cur->son1);

		// Случай, если у первого сына cur есть собственный третий сын
		if (y->son3 != nullptr)
		{
			w->son2 = w->son1;
			w->key2 = cur->key2;
			w->son1 = y->son3;
			y->son3 = nullptr;
			cur->key2 = y->key3;
		}
		// Случай, если у первого сына cur нет собственного третьего сына
		else
		{
			// z - третий сын cur: z == cur->son3
			z = dynamic_cast<InternalNode*>(cur->son3);

			// Если у cur есть третий сын, и у него есть собственный третий сын
			if (z != nullptr && z->son3 != nullptr)
			{
				w->son2 = z->son1;
				w->key2 = cur->key3;
				cur->key3 = z->key2;
				z->son1 = z->son2;
				z->son2 = z->son3;
				z->key2 = z->key3;
				z->son3 = nullptr;
			}
			// Если у cur нет третьего сына, или он есть, но у него нет собственного третьего сына
			else
			{
				y->son3 = w->son1;
				y->key3 = cur->key2;
				delete w;
				cur->son2 = cur->son3;
				cur->key2 = cur->key3;
				cur->son3 = nullptr;

				if (cur->son2 == nullptr)
					one_son = true;
			}
		}
	}
	// Случай, если удаление было у третьего сына cur, и у него остался один сын
	else
	{
		// w - третий сын cur: w == cur->son3
		// Теперь y - второй сын cur: y == cur->son2
		y = dynamic_cast<InternalNode*>(cur->son2);

		// Случай, если у второго сына cur есть собственный третий сын 
		if (y->son3 != nullptr)
		{
			w->son2 = w->son1;
			w->key2 = cur->key3;
			w->son1 = y->son3;
			cur->key3 = y->key3;
			y->son3 = nullptr;
		}
		// Случай, если у второго сына cur нет собственного третьего сына
		else
		{
			y->son3 = w->son1;
			y->key3 = cur->key3;
			cur->son3 = nullptr;
			delete w;
		}
	}

	return true;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::_print(const Node* root, size_t level) const
{
	using std::cout;
	using std::endl;

	// Если поддерево/дерево пустое, то ничего не выводим
	if (root == nullptr)
		return;

	// Если текущий узел - лист, то выводим его ключ
	if (root->getNodeType() == Node_Type::LEAF)
	{
		if (level > 0)
			for (size_t i = 0; i < level - 1; i++)
				cout << "  ";
		for (size_t i = 0; i < 6; i++)
			cout << "--";

		cout << dynamic_cast<const Leaf*>(root)->key << endl << endl;
	}
	// Если текущий узел - внутренний узел, то выводим сначала поддерево третьего сына, затем второго,
	// затем ключи данного внутреннего узла, и, наконец, первого сына
	else
	{
		if (dynamic_cast<const InternalNode*>(root)->son3 != nullptr)
			_print(dynamic_cast<const InternalNode*>(root)->son3, level + 1);
		
		if (dynamic_cast<const InternalNode*>(root)->son2 != nullptr)
			_print(dynamic_cast<const InternalNode*>(root)->son2, level + 1);
		
		if (level > 0)
		{
			for (size_t i = 0; i < level - 1; i++)
				cout << "  ";

			cout << "--";
		}
		if (dynamic_cast<const InternalNode*>(root)->son2 != nullptr)
			cout << dynamic_cast<const InternalNode*>(root)->key2;
		else
			cout << "NULL";
		cout << "-";
		if (dynamic_cast<const InternalNode*>(root)->son3 != nullptr)
			cout << dynamic_cast<const InternalNode*>(root)->key3;
		else
			cout << "NULL";
		cout << endl << endl;

		_print(dynamic_cast<const InternalNode*>(root)->son1, level + 1);
	}
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_getLeftParent(const Leaf* leaf) const
{
	InternalNode* par = nullptr;	// Возвращаемый родительский узел
	Node* cur = this->root;			// Текущий узел

	// Ищем лист leaf и указатель на его ближайшего левого или серидинного родителя сохраняем в par
	while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
	{
		// Переход к первому, т.н. левому сыну
		if (leaf->key < dynamic_cast<InternalNode*>(cur)->key2)
		{
			par = dynamic_cast<InternalNode*>(cur);
			cur = dynamic_cast<InternalNode*>(cur)->son1;
		}
		// Переход к второму сыну
		else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || leaf->key < dynamic_cast<InternalNode*>(cur)->key3)
		{
			// В par заносим cur в том случае, если второй сын - серединный
			if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr)
				par = dynamic_cast<InternalNode*>(cur);

			cur = dynamic_cast<InternalNode*>(cur)->son2;
		}
		// Переход к третьему, т.н. правому сыну
		else
			cur = dynamic_cast<InternalNode*>(cur)->son3;
	}

	return par;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_getRightParent(const Leaf* leaf) const
{
	InternalNode* par = nullptr;	// Возвращаемый родительский узел
	Node* cur = this->root;			// Текущий узел

	// Ищем лист leaf и указатель на его ближайшего правого или серединного родителя сохраняем в par
	while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
	{
		// Переход к первому, т.н. левому сыну
		if (leaf->key < dynamic_cast<InternalNode*>(cur)->key2)
			cur = dynamic_cast<InternalNode*>(cur)->son1;
		// Переход к второму сыну
		else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || leaf->key < dynamic_cast<InternalNode*>(cur)->key3)
		{
			par = dynamic_cast<InternalNode*>(cur);
			cur = dynamic_cast<InternalNode*>(cur)->son2;
		}
		// Переход к третьему, т.н. правому сыну
		else
		{
			par = dynamic_cast<InternalNode*>(cur);
			cur = dynamic_cast<InternalNode*>(cur)->son3;
		}
	}

	return par;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_getMin(Node* root) const
{
	Node* cur = root; // Текущий узел дерева

	// Переходим по левой ветви до листа
	while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
		cur = dynamic_cast<InternalNode*>(cur)->son1;

	return dynamic_cast<Leaf*>(cur);
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_getMax(Node* root) const
{
	Node* cur = root; // Текущий узел дерева

	// Переходим по правой ветви до листа
	while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
	{
		if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr)
			cur = dynamic_cast<InternalNode*>(cur)->son3;
		else if (dynamic_cast<InternalNode*>(cur)->son2 != nullptr)
			cur = dynamic_cast<InternalNode*>(cur)->son2;
		else
			cur = dynamic_cast<InternalNode*>(cur)->son1;
	}

	return dynamic_cast<Leaf*>(cur);
}


// public-методы:
template <class Key_T, class Data_T> typename Data_T& Tree23<Key_T, Data_T>::operator[](const Key_T& key)
{
	this->last_op = 0;

	Leaf* temp = _search(key, this->root);

	if (temp == nullptr)
		throw Tree23_exception("элемента с таким ключом нет в 2-3 дереве");
	else
		return temp->data;
}

template <class Key_T, class Data_T> const typename Tree23<Key_T, Data_T>& Tree23<Key_T, Data_T>::operator=(const Tree23& tree)
{
	if (this == &tree)
		return *this;

	clear();
	
	this->root = _copy(tree.root);
	this->size = tree.size;
	this->last_op = 0;
	
	return *this;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::clear()
{
	_clear(this->root);

	this->root = nullptr;
	this->size = 0;
	this->last_op = 0;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	this->last_op = 0;

	if (!_insert(key, data))
		throw Tree23_exception("элемент с таким ключом уже есть в 2-3 дереве");
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::remove(const Key_T& key)
{
	this->last_op = 0;

	if (!_delete(key))
		throw Tree23_exception("элемента с таким ключом нет в 2-3 дереве");
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::printAsDict() const
{
	using std::cout;
	using std::endl;
	
	if (this->size == 0)
		cout << "{}" << endl;
	else
	{
		Iterator iter = begin();

		cout << "{ ";
		for (size_t i = 0; i < this->size - 1; iter++, i++)
			cout << *iter << ": " << getItemByKey(*iter) << ", ";
		cout << *iter << ": " << getItemByKey(*iter) << " }" << endl;
	}
}
//*************************************************************************************************************************************************//


//*********************************************************** Реализация методов Iterator *********************************************************//
// public-методы:
template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::Iterator::Iterator(const Tree23& tree, const Key_T& key)
{
	this->tree = &tree;
	this->cur = this->tree->_search(key, this->tree->root);
}

template <class Key_T, class Data_T> const typename Key_T& Tree23<Key_T, Data_T>::Iterator::operator*() const
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator++()
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");

	// Ищем ближайшего предка от которого к данному узлу переходили НЕ по правой ветви
	InternalNode* par = this->tree->_getLeftParent(this->cur);

	// Если такого предка не нашлось, значит ключ данного листа наибольший
	if (par == nullptr)
		this->cur = nullptr;
	// Иначе переходим к следующему элементу
	else
	{
		// Если были в первом сыне родителя, то переходим к минимальному элементу второго сына
		if (this->cur->key < par->key2)
			this->cur = this->tree->_getMin(par->son2);
		// Если же были в серединном узле, то переходим к минимальному элементу третьего сына
		else
			this->cur = this->tree->_getMin(par->son3);
	}

	return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator--()
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");

	// Ищем ближайшего предка от которого к данному узлу переходили НЕ по левой ветви
	InternalNode* par = this->tree->_getRightParent(this->cur);

	// Если такого предка не нашлось, значит ключ данного листа наименьший
	if (par == nullptr)
		this->cur = nullptr;
	// Иначе переходим к предыдущему элементу
	else
	{
		// Если были в третьем сыне родителя, то переходим к максимальному элементу второго сына
		if (par->son3 != nullptr && this->cur->key >= par->key3)
			this->cur = this->tree->_getMax(par->son2);
		// Если были во втором сыне родителя, то переходим к максимальному элементу первого сына
		else
			this->cur = this->tree->_getMax(par->son1);
	}

	return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator Tree23<Key_T, Data_T>::Iterator::operator++(int)
{
	Iterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator Tree23<Key_T, Data_T>::Iterator::operator--(int)
{
	Iterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::Iterator::operator==(const Iterator& iter) const
{
	return this->tree == iter.tree && this->cur == iter.cur;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::Iterator::operator!=(const Iterator& iter) const
{
	return !(this->tree == iter.tree && this->cur == iter.cur);
}

template <class Key_T, class Data_T> const typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	this->tree = iter.tree;
	this->cur = iter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


//*************************************************************** Реализация методов RIterator ****************************************************//
template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::RIterator::RIterator(const Tree23& tree, const Key_T& key)
{
	this->tree = &tree;
	this->cur = this->tree->_search(key, this->tree->root);
}

template <class Key_T, class Data_T> const typename Key_T& Tree23<Key_T, Data_T>::RIterator::operator*() const
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator++()
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");

	// Ищем ближайшего предка от которого к данному узлу переходили НЕ по левой ветви
	InternalNode* par = this->tree->_getRightParent(this->cur);

	// Если такого предка не нашлось, значит ключ данного листа наименьший
	if (par == nullptr)
		this->cur = nullptr;
	// Иначе переходим к предыдущему элементу
	else
	{
		// Если были в третьем сыне родителя, то переходим к максимальному элементу второго сына
		if (par->son3 != nullptr && this->cur->key >= par->key3)
			this->cur = this->tree->_getMax(par->son2);
		// Если были во втором сыне родителя, то переходим к максимальному элементу первого сына
		else
			this->cur = this->tree->_getMax(par->son1);
	}

	return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator--()
{
	if (this->cur == nullptr)
		throw Tree23_exception("итератор не установлен");

	// Ищем ближайшего предка от которого к данному узлу переходили НЕ по правой ветви
	InternalNode* par = this->tree->_getLeftParent(this->cur);

	// Если такого предка не нашлось, значит ключ данного листа наибольший
	if (par == nullptr)
		this->cur = nullptr;
	// Иначе переходим к следующему элементу
	else
	{
		// Если были в первом сыне родителя, то переходим к минимальному элементу второго сына
		if (this->cur->key < par->key2)
			this->cur = this->tree->_getMin(par->son2);
		// Если же были в серединном узле, то переходим к минимальному элементу третьего сына
		else
			this->cur = this->tree->_getMin(par->son3);
	}

	return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator Tree23<Key_T, Data_T>::RIterator::operator++(int)
{
	RIterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator Tree23<Key_T, Data_T>::RIterator::operator--(int)
{
	RIterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::RIterator::operator==(const RIterator& riter) const
{
	return this->tree == riter.tree && this->cur == riter.cur;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::RIterator::operator!=(const RIterator& riter) const
{
	return !(this->tree == riter.tree && this->cur == riter.cur);
}

template <class Key_T, class Data_T> const typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator=(const RIterator& riter)
{
	if (this == &riter)
		return *this;

	this->tree = riter.tree;
	this->cur = riter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
