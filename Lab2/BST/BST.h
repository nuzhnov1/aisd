// BST.h
#pragma once
#ifndef _INC_BST_
#define _INC_BST_
// Подключаемые заголовочные файлы
#include <iostream>
#include <string>
#include <stack>


//************************************************************ Класс исключений *******************************************************************//
class BST_exception
{

	using string = std::string;

	string error;

public:

	BST_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


//**************************************************** Класс АТД "Бинарное дерево поиска" *********************************************************//
template <class Key_T, class Data_T>
class BST
{
	// Класс - узел BST-дерева
	struct Node
	{

		Key_T key;		// Ключ узла
		Data_T data;	// Данные узла
		Node* left;		// Указатель на левое поддерево
		Node* right;	// Указатель на правое поддерево

		// Конструктор узла с параметрами
		Node(const Key_T& key, const Data_T& data) : key(key), data(data), left(nullptr), right(nullptr) {}

	};

	size_t size;			// Количество элементов в дереве
	Node* root;				// Указатель на корень дерева
	mutable size_t last_op;	// Количество переборов на последней операции

	// Тело конструктора копирования
	void _copy(const BST& tree);

public:

	class Iterator;
	class RIterator;

	// Конструктор по умолчанию
	BST() : size(0), root(nullptr), last_op(0) {}
	// Конструктор копирования
	BST(const BST& tree) { _copy(tree); }
	// Деструктор
	~BST() { clear(); }

	// Оператор доступа к элементу по ключу
	Data_T& operator[](const Key_T& key);
	const Data_T& operator[](const Key_T& key) const { return const_cast<BST&>(*this)[key]; }
	// Оператор присваивания деревьев
	const BST& operator=(const BST& tree);

	// Получить размер дерева
	size_t getSize() const { return this->size; }
	// Получить количество переборов на последней операции
	size_t getLastOp() const { return this->last_op; }
	// Получить длину внешнего пути дерева за время O(n)
	size_t getExternalPath() const;
	// Проверка дерева на пустоту
	bool isEmpty() const { return this->size == 0; }

	// Чтение элемента дерева по ключу
	const Data_T& getItemByKey(const Key_T& key) const { return const_cast<BST&>(*this)[key]; }
	// Запись элемента по заданному ключу
	void setItemByKey(const Key_T& key, const Data_T& data) { (*this)[key] = data; }

	// Очистка дерева
	void clear();
	// Добавить элемент с заданным ключом в дерево
	void append(const Key_T& key, const Data_T& data);
	// Удалить элемент с заданным ключом
	void remove(const Key_T& key);
	
	// Вывести дерево в древовидной форме
	void print() const;
	// Вывести дерево в форме словаря: ключ-значение
	void printAsDict() const;
	// Вывести ключи дерева в симметричном порядке
	void printKeys() const;

	// Установка итератора на минимальный элемент дерева
	Iterator begin() const;
	// Установка итератора в "неустановленное" состояние
	Iterator end() const { return Iterator(*this); }
	// Установка обратного итератор на максимальный элемент списка
	RIterator rbegin() const;
	// Установка обратного итератора в "неустановленное" состояние
	RIterator rend() const { return RIterator(*this); }

	// Класс прямого итератора
	class Iterator
	{

		friend class BST;

		const BST* tree;	// Указатель на текущее BST-дерево
		const Node* cur;	// Текущий узел, на который установлен итератор

		// private-конструктор прямого итератора с параметрами: tree - указатель на дерево, node - указатель на узел
		Iterator(const BST* tree, const Node* node) : tree(tree), cur(node) {}

	public:

		// Конструктор прямого итератора по умолчанию
		Iterator() : tree(nullptr), cur(nullptr) {}
		// Конструктор прямого итератора с параметром tree - ссылкой на BST-дерево
		Iterator(const BST& tree) : tree(&tree), cur(nullptr) {}
		// Конструктор прямого итератора с параметрами: tree - ссылка на BST-дерево, key - ключом
		Iterator(const BST& tree, const Key_T& key);
		// Конструктор копирования прямого итератора
		Iterator(const Iterator& iter) : tree(iter.tree), cur(iter.cur) {}

		// Оператор "разыменовывания" - получение текущего элемента узла дерева по ссылке
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

		friend class BST;

		const BST* tree;	// Указатель на текущее BST-дерево
		const Node* cur;	// Текущий узел, на который установлен итератор

		// private-конструктор обратного итератора с параметрами: tree - указатель на дерево, node - указатель на узел
		RIterator(const BST* tree, const Node* node) : tree(tree), cur(node) {}

	public:

		// Конструктор обратного итератора по умолчанию
		RIterator() : tree(nullptr), cur(nullptr) {}
		// Конструктор обратного итератора с параметром tree - ссылкой на BST-дерево
		RIterator(const BST& tree) : tree(&tree), cur(nullptr) {}
		// Конструктор обратного итератора с параметрами: tree - ссылка на BST-дерево, key - ключ
		RIterator(const BST& tree, const Key_T& key);
		// Конструктор копирования обратного итератора
		RIterator(const RIterator& riter) : tree(riter.tree), cur(riter.cur) {}

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
		bool operator==(const RIterator& riter) const;
		// Оператор неравенства обратных итераторов
		bool operator!=(const RIterator& riter) const;

		// Оператор присваивания обратного итератора
		const RIterator& operator=(const RIterator& riter);

	};

};
//*************************************************************************************************************************************************//


//************************************************************* Реализация методов BST ************************************************************//
// private-методы:
template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::_copy(const BST& tree)
{
	// Обходим дерево tree и данное дерево this префиксно, создавая узлы-копии дерева tree для дерева this

	using std::stack;

	Node* tree_cur = tree.root, * this_cur = nullptr;	// Указатели на текущие узлы tree и this 
	stack<Node*> tree_stk, this_stk;					// Вспомогательные стеки для обхода деревьев

	this->root = nullptr;
	this->size = 0;
	this->last_op = 0;

	// Если tree изначально пусто, то создаём пустое дерево
	if (tree_cur == nullptr)
		return;
	// Иначе копируем корень и продолжаем обход
	else
		this_cur = this->root = new Node(*tree_cur);

	// Обходим дерево tree и заодно дерево this, добавляя просматриваемые узлы tree в соответствующие узлы this
	tree_stk.push(tree_cur);
	this_stk.push(this_cur);
	while (!tree_stk.empty())
	{
		tree_cur = tree_stk.top();
		tree_stk.pop();

		this_cur = this_stk.top();
		this_stk.pop();

		if (tree_cur->right != nullptr)
		{
			this_cur->right = new Node(*tree_cur->right);
			tree_stk.push(tree_cur->right);
			this_stk.push(this_cur->right);
		}
		if (tree_cur->left != nullptr)
		{
			this_cur->left = new Node(*tree_cur->left);
			tree_stk.push(tree_cur->left);
			this_stk.push(this_cur->left);
		}
	}

	this->size = tree.size;
}


// public-методы:
template <class Key_T, class Data_T> typename Data_T& BST<Key_T, Data_T>::operator[](const Key_T& key)
{
	// Обходим дерево в поиске элемента с ключом, равным заданному

	Node* cur = this->root;

	this->last_op = 1;

	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;

		this->last_op++;
	}

	if (cur == nullptr)
		throw BST_exception("элемента с таким ключом нет в BST-дереве");
	else
		return cur->data;
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>& BST<Key_T, Data_T>::operator=(const BST& tree)
{
	if (this == &tree)
		return *this;

	clear();
	_copy(tree);

	return *this;
}

template <class Key_T, class Data_T> typename size_t BST<Key_T, Data_T>::getExternalPath() const
{
	// Обходим дерево инфиксно, находя внешние узлы и складывая их уровни

	using std::stack;

	Node* cur = this->root;					// Текущий узел дерева
	size_t cur_level = 0;					// Текущий уровень элемента
	size_t result = 0;						// Длина внешнего пути
	stack<Node*> stk1, stk2;				// Вспомогательные стеки узлов
	stack<size_t> level_stk1, level_stk2;	// Вспомогательные стеки уровней

	this->last_op = 1;

	if (cur == nullptr)
		return result;

	stk1.push(cur);
	level_stk1.push(cur_level);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();
			cur_level = level_stk1.top();
			level_stk1.pop();

			stk2.push(cur);
			level_stk2.push(cur_level);

			if (cur->right != nullptr)
			{
				stk1.push(cur->right);
				level_stk1.push(cur_level + 1);
			}
		}
		else
		{
			cur = stk2.top();
			stk2.pop();
			cur_level = level_stk2.top();
			level_stk2.pop();

			// Если узел внешний, то добавляем его уровень к результату
			if (cur->left == nullptr || cur->right == nullptr)
				result += cur_level;

			if (cur->left != nullptr)
			{
				stk1.push(cur->left);
				level_stk1.push(cur_level + 1);
			}
		}

		this->last_op++;
	}

	return result;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::clear()
{
	// Обходим дерево постфиксно, добавляя пройденные вершины в стек stk2,
	// и затем очищаем его, путём удаление каждой вершины в стеке

	using std::stack;

	Node* cur = this->root;		// Текущий узел
	stack<Node*> stk1, stk2;	// Вспомогательные стеки для обхода вершин

	// Если дерево пусто, то выходим
	if (cur == nullptr)
		return;

	// Обходим дерево, формируя стек вершин
	stk1.push(cur);
	while (!stk1.empty())
	{
		cur = stk1.top();
		stk1.pop();

		stk2.push(cur);

		if (cur->left != nullptr)
			stk1.push(cur->left);
		if (cur->right != nullptr)
			stk1.push(cur->right);
	}

	// Проходя по стеку, освобождаем память из под узлов дерева и удаляем их из стека
	while (!stk2.empty())
	{
		cur = stk2.top();
		stk2.pop();

		delete cur;
	}

	this->root = nullptr;
	this->size = 0;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
	// Ищем элемент дерева с ключом key, если он есть, то выбрасываем исключение, если его нет то добавляем влево или вправо
	// в зависимости от key и ключа внутреннего узла, до которого дошли. Если key меньше ключа в этом узле, то добавляем влево,
	// если больше, то вправо

	Node* cur = this->root; // Текущий элемент

	this->last_op = 1;

	// Если дерево изначально пусто, то добавляем элемент в корень дерева
	if (cur == nullptr)
	{
		this->root = new Node(key, data);
		this->size++;

		return;
	}

	// Поиск свободной позиции или элемента с тем же ключом
	while (true)
	{
		// Если элемент с таким ключом уже есть в дереве, то выбрасываем исключение
		if (key == cur->key)
			throw BST_exception("элемент с таким ключом уже есть в BST-дереве");
		if (key < cur->key)
			if (cur->left != nullptr)
				cur = cur->left;
			else
				break;
		else
			if (cur->right != nullptr)
				cur = cur->right;
			else
				break;

		this->last_op++;
	}

	// Добавление элемента в дерево в зависимости от заданного ключа
	if (key < cur->key)
		cur->left = new Node(key, data);
	else
		cur->right = new Node(key, data);

	this->size++;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::remove(const Key_T& key)
{
	// Для удаления элемента из дерева сначала ищем элемент с заданным ключом, если такого элемента нет,
	// то выбрасываем исключение, иначе удаляем элемент

	Node* cur = this->root;	// Текущий элемент дерева
	Node* par = nullptr;	// Родительский узел для текущего

	this->last_op = 1;

	// Поиск элемента
	while (cur != nullptr && key != cur->key)
	{
		par = cur;

		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;

		this->last_op++;
	}

	// Если элемент не найден, то выбрасываем исключение
	if (cur == nullptr)
		throw BST_exception("элемента с таким ключом нет в BST-дереве");

	// Четыре возможных ситуации при удалении найденного элемента
	// Первая - элемент не содержит потомков
	if (cur->left == nullptr && cur->right == nullptr)
	{
		if (cur == this->root) // Если найденный элемент - корень дерева
			this->root = nullptr;
		else
			// Родительской ветви, которая идёт к данному элементу присваиваем nullptr
			if (par->left == cur)
				par->left = nullptr;
			else
				par->right = nullptr;
	}
	// Вторая - элемент содержит только правого потомка
	else if (cur->left == nullptr)
	{
		if (cur == this->root)
			this->root = this->root->right;
		else
		{
			// Родительской ветви присваиваем правую ветвь найденного элемента
			if (par->left == cur)
				par->left = cur->right;
			else
				par->right = cur->right;
		}
	}
	// Третья - элемент содержит только левого потомка
	else if (cur->right == nullptr)
	{
		if (cur == this->root)
			this->root = this->root->left;
		else
		{
			// Родительской ветви присваиваем левую ветвь найденного элемента
			if (par->left == cur)
				par->left = cur->left;
			else
				par->right = cur->left;
		}
	}
	// Четвёртая - элемент содержит оба потомка
	else
	{
		Node* del = cur; // Указатель на удаляемый элемент

		cur = cur->right; // Переходим к корню правых потомков. Теперь cur - указатель на замещающий элемент
		this->last_op++;

		// Переходим к элементу с минимальным ключом среди потомков cur
		while (cur->left != nullptr)
		{
			par = cur;
			cur = cur->left;

			this->last_op++;
		}

		// Ключ и данные замещающего элемента присваиваем соотвествующим атрибутам удаляемого элемента
		del->key = cur->key;
		del->data = cur->data;
		if (del->right == cur)  // Если замещающий элемент - это корень правых потомков
			del->right = cur->right; // то связываем правую ветвь удаляемого элемента с правой ветвью замещающего
		else
			par->left = cur->right; // иначе левой ветви родителя замещающего элемента присваиваем правую ветвь замещающего элемента
	}

	delete cur;

	this->size--;
	this->last_op++;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::print() const
{
	// Обходим дерево инфиксно, выводя в консоль структуру дерева

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;		// Текущий узел дерева
	size_t cur_level = 0;		// Текущий уровень элемента
	stack<Node*> stk1, stk2, stk3;						// Вспомогательные стеки узлов
	stack<size_t> level_stk1, level_stk2, level_stk3;	// Вспомогательные стеки уровней

	if (cur == nullptr)
	{
		cout << endl;

		return;
	}

	stk1.push(cur);
	level_stk1.push(cur_level);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();
			cur_level = level_stk1.top();
			level_stk1.pop();

			stk2.push(cur);
			level_stk2.push(cur_level);

			if (cur->left != nullptr)
			{
				stk1.push(cur->left);
				level_stk1.push(cur_level + 1);
			}
		}
		else
		{
			cur = stk2.top();
			stk2.pop();
			cur_level = level_stk2.top();
			level_stk2.pop();

			stk3.push(cur);
			level_stk3.push(cur_level);

			if (cur->right != nullptr)
			{
				stk1.push(cur->right);
				level_stk1.push(cur_level + 1);
			}
		}
	}

	while (!stk3.empty())
	{
		cur = stk3.top();
		stk3.pop();
		cur_level = level_stk3.top();
		level_stk3.pop();

		if (cur_level > 0)
		{
			for (size_t i = 0; i < cur_level - 1; i++)
				cout << "  ";
			cout << "--";
		}
		cout << cur->key << endl << endl;
	}
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::printAsDict() const
{
	// Обходим дерево инфиксно, выводя в консоль структуру дерева в виде словаря

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;			// Текущий узел дерева
	stack<Node*> stk1, stk2, stk3;	// Вспомогательные стеки узлов

	if (cur == nullptr)
	{
		cout << "{}" << endl;

		return;
	}

	stk1.push(cur);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();

			stk2.push(cur);

			if (cur->right != nullptr)
				stk1.push(cur->right);
		}
		else
		{
			cur = stk2.top();
			stk2.pop();

			stk3.push(cur);

			if (cur->left != nullptr)
				stk1.push(cur->left);
		}
	}

	cout << "{";
	while (stk3.size() > 1)
	{
		cur = stk3.top();
		stk3.pop();

		cout << cur->key << ": " << cur->data << ", ";
	}
	cur = stk3.top();
	stk3.pop();

	cout << cur->key << ": " << cur->data << "}" << endl;
}

template <class Key_T, class Data_T> typename void BST<Key_T, Data_T>::printKeys() const
{
	// Обходим дерево инфиксно, выводя в консоль список ключей дерева

	using std::stack;
	using std::cout;
	using std::endl;

	Node* cur = this->root;			// Текущий узел дерева
	stack<Node*> stk1, stk2, stk3;	// Вспомогательные стеки узлов

	if (cur == nullptr)
	{
		cout << "[]" << endl;
		cout << endl;
		return;
	}

	stk1.push(cur);
	while (!stk1.empty() || !stk2.empty())
	{
		if (!stk1.empty())
		{
			cur = stk1.top();
			stk1.pop();

			stk2.push(cur);

			if (cur->right != nullptr)
				stk1.push(cur->right);
		}
		else
		{
			cur = stk2.top();
			stk2.pop();

			stk3.push(cur);

			if (cur->left != nullptr)
				stk1.push(cur->left);
		}
	}

	cout << "[";
	while (stk3.size() > 1)
	{
		cur = stk3.top();
		stk3.pop();

		cout << cur->key << ", ";
	}
	cur = stk3.top();
	stk3.pop();

	cout << cur->key << "]" << endl;
	cout << endl;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::begin() const
{
	Node* cur = this->root;

	if (cur == nullptr)
		return Iterator(*this);

	// Ищем элемент дерева с минимальным ключом и устанавливаем итератор на него
	while (cur->left != nullptr)
		cur = cur->left;

	return Iterator(this, cur);
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::rbegin() const
{
	Node* cur = this->root;

	if (cur == nullptr)
		return RIterator(*this);

	// Ищем элемент дерева с максимальным ключом и устанавливаем итератор на него
	while (cur->right != nullptr)
		cur = cur->right;

	return RIterator(this, cur);
}
//*************************************************************************************************************************************************//


//********************************************************** Реализация методов Iterator **********************************************************//
template <class Key_T, class Data_T> BST<Key_T, Data_T>::Iterator::Iterator(const BST& tree, const Key_T& key)
{
	Node* cur = tree.root;

	this->tree = &tree;

	// Ищем элемент с заданным ключом
	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;
	}

	this->cur = cur;
}

template <class Key_T, class Data_T> const typename Key_T& BST<Key_T, Data_T>::Iterator::operator*() const
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator++()
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");

	// Если у данного узла есть правый потомок, то переходим к нему
	if (this->cur->right != nullptr)
	{
		this->cur = this->cur->right;
		// Если у этого потомка есть левая ветвь, то проходим по ней до конца
		while (this->cur->left != nullptr)
			this->cur = this->cur->left;
	}
	// Иначе ищем ближайшего левого родителя
	else
	{
		Node* left_par = nullptr;		// Ближайший левый родитель
		Node* cur = this->tree->root;	// Текущий узел дерева

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
			{
				left_par = cur;
				cur = cur->left;
			}
			else
				cur = cur->right;
		}

		this->cur = left_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator--()
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");

	// Если у данного узла есть левый потомок, то переходим к нему
	if (this->cur->left != nullptr)
	{
		this->cur = this->cur->left;
		// Если у этого потомка есть правая ветвь, то проходим по ней до конца
		while (this->cur->right != nullptr)
			this->cur = this->cur->right;
	}
	// Иначе ищем ближайщего правого родителя
	else
	{
		Node* right_par = nullptr;		// Ближайший правый родитель
		Node* cur = this->tree->root;	// Текущий узел дерева

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
				cur = cur->left;
			else
			{
				right_par = cur;
				cur = cur->right;
			}
		}

		this->cur = right_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::Iterator::operator++(int)
{
	Iterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::Iterator BST<Key_T, Data_T>::Iterator::operator--(int)
{
	Iterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::Iterator::operator==(const Iterator& iter) const
{
	return this->tree == iter.tree && this->cur == iter.cur;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::Iterator::operator!=(const Iterator& iter) const
{
	return !(this->tree == iter.tree && this->cur == iter.cur);
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>::Iterator& BST<Key_T, Data_T>::Iterator::operator=(const Iterator& iter)
{
	if (this == &iter)
		return *this;

	this->tree = iter.tree;
	this->cur = iter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


//********************************************************** Реализация методов RIterator *********************************************************//
template <class Key_T, class Data_T> BST<Key_T, Data_T>::RIterator::RIterator(const BST& tree, const Key_T& key)
{
	Node* cur = tree.root;

	this->tree = &tree;

	// Ищем элемент с заданным ключом
	while (cur != nullptr && key != cur->key)
	{
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;
	}

	this->cur = cur;
}

template <class Key_T, class Data_T> const typename Key_T& BST<Key_T, Data_T>::RIterator::operator*() const
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");
	else
		return this->cur->key;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator++()
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");

	// Если у данного узла есть левый потомок, то переходим к нему
	if (this->cur->left != nullptr)
	{
		this->cur = this->cur->left;
		// Если у этого потомка есть правая ветвь, то проходим по ней до конца
		while (this->cur->right != nullptr)
			this->cur = this->cur->right;
	}
	// Иначе ищем ближайщего правого родителя
	else
	{
		Node* right_par = nullptr;		// Ближайший правый родитель
		Node* cur = this->tree->root;	// Текущий узел дерева

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
				cur = cur->left;
			else
			{
				right_par = cur;
				cur = cur->right;
			}
		}

		this->cur = right_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator--()
{
	if (this->cur == nullptr)
		throw BST_exception("итератор не установлен");

	// Если у данного узла есть правый потомок, то переходим к нему
	if (this->cur->right != nullptr)
	{
		this->cur = this->cur->right;
		// Если у этого потомка есть левая ветвь, то проходим по ней до конца
		while (this->cur->left != nullptr)
			this->cur = this->cur->left;
	}
	// Иначе ищем ближайшего левого родителя
	else
	{
		Node* left_par = nullptr;		// Ближайший левый родитель
		Node* cur = this->tree->root;	// Текущий узел дерева

		while (cur->key != this->cur->key)
		{
			if (this->cur->key < cur->key)
			{
				left_par = cur;
				cur = cur->left;
			}
			else
				cur = cur->right;
		}

		this->cur = left_par;
	}

	return *this;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::RIterator::operator++(int)
{
	RIterator temp = *this;

	++(*this);

	return temp;
}

template <class Key_T, class Data_T> typename BST<Key_T, Data_T>::RIterator BST<Key_T, Data_T>::RIterator::operator--(int)
{
	RIterator temp = *this;

	--(*this);

	return temp;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::RIterator::operator==(const RIterator& riter) const
{
	return this->tree == riter.tree && this->cur == riter.cur;
}

template <class Key_T, class Data_T> typename bool BST<Key_T, Data_T>::RIterator::operator!=(const RIterator& riter) const
{
	return !(this->tree == riter.tree && this->cur == riter.cur);
}

template <class Key_T, class Data_T> const typename BST<Key_T, Data_T>::RIterator& BST<Key_T, Data_T>::RIterator::operator=(const RIterator& riter)
{
	if (this == &riter)
		return *this;

	this->tree = riter.tree;
	this->cur = riter.cur;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
