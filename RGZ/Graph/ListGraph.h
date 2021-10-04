// ListGraph.h
#pragma once
#ifndef _INC_LIST_GRAPH_
#define _INC_LIST_GRAPH_
// Подключаемые заголовочные файлы
#include <iostream>
#include <vector>
#include <list>
#include "GraphForm.h"
#include "Graph_exception.h"
#include "Vertex.h"
#include "Edge.h"


//********************************************** Класс списковой формы представления графа ********************************************************//
template <class Vertex_T, class Edge_T>
class ListGraph : public GraphForm<Vertex_T, Edge_T>
{
	
	struct Node;

	template <typename T> using vector = std::vector<T>;
	template <typename T> using list = std::list<T>;
	using list_iterator = typename list<Node>::const_iterator;
	using GraphForm = GraphForm<Vertex_T, Edge_T>;
	using EdgeIteratorForm = typename GraphForm::EdgeIteratorForm;
	using OutEdgeIteratorForm = typename GraphForm::OutEdgeIteratorForm;

	// Узел списка
	struct Node
	{

		Edge_T* edge;	// Указатель на ребро
		size_t v2;		// Номер второй вершины

		Node(Edge_T* edge, size_t v2) : edge(edge), v2(v2) {}

	};

	vector<list<Node>> matrix;	// Вектор списков смежности

	// Есть ли ребро с вершинами outV и inV в графе. В "iter" записывается итератор на это ребро
	bool _isHasEdge(size_t outV, size_t inV, list_iterator* iter = nullptr) const;

public:

	// Конструктор списковой формы графа с параметрами: ссылкой на вектор вершин vectorVertex и ориентированностью графа directed
	ListGraph(const vector<Vertex_T*>& vectorVertex, bool directed) : GraphForm(vectorVertex, directed), matrix() {}
	// Конструктор копирования списковой формы графа
	ListGraph(const ListGraph& graph, const vector<Vertex_T*>& vectorVertex);
	// Деструктор списковой формы графа
	~ListGraph();

	// Оператор присваивания графов
	virtual const ListGraph& operator=(const GraphForm& graph) final;

	// Есть ли ребро в графе
	virtual bool isHasEdge(size_t outV, size_t inV) const final;
	// Получить ребро, соединающее вершины outV и inV
	virtual Edge_T& getEdge(size_t outV, size_t inV) const final;

	// Добавить вершину в граф
	virtual void insertVertex(size_t index) final;
	// Удалить вершину графа
	virtual void deleteVertex(size_t index) final;
	// Добавить ребро в граф, соединающего вершины outV и inV, с заданными параметрами в edge
	virtual void insertEdge(size_t outV, size_t inV, Edge_T* edge) final;
	// Удалить ребро из графа, соединающего вершины outV и inV
	virtual void deleteEdge(size_t outV, size_t inV) final;

	// Вывести граф
	virtual void print() const final;

	// Класс итератора рёбер графа списковой формы
	class ListEdgeIterator : public EdgeIteratorForm
	{

		const ListGraph* graph_p;	// Указатель на текущий L-граф
		size_t curV;				// Номер текущей вершины
		list_iterator curE;			// Итератор текущего ребра

	public:

		// Конструктор итератора с параметрами: ссылкой на листовой граф graph и состоянием итератора toBegin
		ListEdgeIterator(const ListGraph& graph, bool toBegin = true);
		// Конструктор копирования итераторов
		ListEdgeIterator(const ListEdgeIterator& iter) : graph_p(iter.graph_p), curV(iter.curV), curE(iter.curE) {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const final;

		// Префиксный оператор инкрементирования - переход к следующему ребру графа
		virtual ListEdgeIterator& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const EdgeIteratorForm& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const EdgeIteratorForm& iter) const final;

		// Оператор присваивания итераторов
		virtual const ListEdgeIterator& operator=(const EdgeIteratorForm& iter) final;

		// Возвращает итератор, установленный на начало
		static ListEdgeIterator begin(const ListGraph& graph) { return ListEdgeIterator(graph, true); }
		// Возвращает итератор, установленный на конец
		static ListEdgeIterator end(const ListGraph& graph) { return ListEdgeIterator(graph, false); }
	
	};

	// Класс итератора исходящих рёбер графа списковой формы
	class ListOutEdgeIterator : public OutEdgeIteratorForm
	{

		const ListGraph* graph_p;	// Указатель на текущий L-граф
		size_t curV;				// Номер текущей вершины
		list_iterator curE;			// Итератор текущего ребра

	public:

		// Конструктор итератора с параметрами: ссылкой на матричный граф graph, индексом вершины index и состоянием итератора toBegin
		ListOutEdgeIterator(const ListGraph& graph, size_t index, bool toBegin = true);
		// Конструктор копирования итераторов
		ListOutEdgeIterator(const ListOutEdgeIterator& iter) : graph_p(iter.graph_p), curV(iter.curV), curE(iter.curE) {}

		// Получить дескриптор ребра графа, на который в данный момент указывает итератор
		virtual Edge_T& operator*() const final;

		// Префиксный оператор инкрементирования - переход к следующему исходящему ребру вершины графа
		virtual ListOutEdgeIterator& operator++() final;

		// Оператор равенства итераторов
		virtual bool operator==(const OutEdgeIteratorForm& iter) const final;
		// Оператор неравенства итераторов
		virtual bool operator!=(const OutEdgeIteratorForm& iter) const final;

		// Оператор присваивания итераторов
		virtual const ListOutEdgeIterator& operator=(const OutEdgeIteratorForm& iter) final;

		// Возвращает итератор, установленный на начало
		static ListOutEdgeIterator begin(const ListGraph& graph, size_t index) { return ListOutEdgeIterator(graph, index, true); }

		// Возвращает итератор, установленный на конец
		static ListOutEdgeIterator end(const ListGraph& graph, size_t index) { return ListOutEdgeIterator(graph, index, false); }

	};

};
//*************************************************************************************************************************************************//


//***************************************************** Реализация методов ListGraph **************************************************************//
// private-методы:
template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::_isHasEdge(size_t outV, size_t inV, list_iterator* iter) const
{
	// Проходим по списку под номером "outV" в поиске узла с входной вершины, равным inV
	// В "iter" записываем итератор на найденный элемент
	list_iterator temp_iter = this->matrix[outV].begin();
	for (auto end = this->matrix[outV].end(); temp_iter != end; temp_iter++)
		if ((*temp_iter).v2 == inV)
		{
			if (iter != nullptr)
				*iter = temp_iter;

			return true;
		}

	if (iter != nullptr)
		*iter = temp_iter;

	return false;
}


// public-методы:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListGraph(const ListGraph& graph, const vector<Vertex_T*>& vectorVertex): 
	GraphForm(vectorVertex, graph.directed)
{
	this->directed = graph.directed;
	this->edgesCount = graph.edgesCount;
	this->matrix = vector<list<Node>>();

	// Копируем матрицу списков смежности, добавляя в вектор список рёбер,
	// а затем сами рёбра, связывающие те же номера вершин, что в копируемом графе
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		this->matrix.push_back(list<Node>());	// Добавляем пустой список

		// Проходим по рёбрам в текущем списке копируемой матрицы списков
		for (const auto& node : graph.matrix[i])
		{
			size_t j = node.v2;

			// Создаём ребро
			Edge_T* tmp_edge = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

			// Пытаемся установить вес в ребро
			try { tmp_edge->setWeight(node.edge->getWeight()); }
			catch (Graph_exception&) {}

			// Пытаемся установить данные в ребро
			try { tmp_edge->setData(node.edge->getData()); }
			catch (Graph_exception&) {}

			// Добавляем ребро в список
			this->matrix[i].push_back(Node(tmp_edge, j));
		}
	}
}

template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::~ListGraph()
{
	// Проходим по вектору списков рёбер, очищая каждое из них
	for (auto& listNode : this->matrix)
	{
		// Проходим по списку, удаляя каждое ребро
		for (const auto& node : listNode)
			if (node.edge != nullptr)
				delete node.edge;

		listNode.clear();
	}

	this->matrix.clear();
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>& ListGraph<Vertex_T, Edge_T>::operator=(const GraphForm& graph)
{
	// Приводим абстрактную форму графа к списковой форме
	const ListGraph& l_graph = dynamic_cast<const ListGraph&>(graph);

	if (this == &l_graph)
		return *this;

	// Проходим по вектору списков рёбер, очищая каждое из них
	for (auto& listNode : this->matrix)
	{
		// Проходим по списку, удаляя каждое ребро
		for (const auto& node : listNode)
			if (node.edge != nullptr)
				delete node.edge;

		listNode.clear();
	}
	this->matrix.clear();

	this->directed = l_graph.directed;
	this->edgesCount = l_graph.edgesCount;
	this->matrix = vector<list<Node>>();

	// Копируем матрицу списков смежности, добавляя в вектор список рёбер,
	// а затем сами рёбра, связывающие те же номера вершин, что в копируемом графе
	for (size_t i = 0, size = this->_getVertexCount(); i < size; i++)
	{
		this->matrix.push_back(list<Node>());	// Добавляем пустой список

		// Проходим по рёбрам в текущем списке копируемой матрицы списков
		for (const auto& node : l_graph.matrix[i])
		{
			size_t j = node.v2;

			// Создаём ребро
			Edge_T* tmp_edge = new Edge_T(*this->vectorVertex[i], *this->vectorVertex[j]);

			// Пытаемся установить вес в ребро
			try { tmp_edge->setWeight(node.edge->getWeight()); }
			catch (Graph_exception&) {}

			// Пытаемся установить данные в ребро
			try { tmp_edge->setData(node.edge->getData()); }
			catch (Graph_exception&) {}

			// Добавляем ребро в список
			this->matrix[i].push_back(Node(tmp_edge, j));
		}
	}

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::isHasEdge(size_t outV, size_t inV) const
{
	// Проходим по списку под номером outV, проверяя индексы входных вершин на совпадение с inV
	for (const auto& edge : this->matrix[outV])
		if (edge.v2 == inV)
			return true;

	return false;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::getEdge(size_t outV, size_t inV) const
{
	typename list<Node>::iterator cur{};

	// Проверяем, есть ли заданное ребро в графе
	if (!_isHasEdge(outV, inV, &cur))
		throw Graph_exception("такого ребра нет в графе");

	return (*(*cur).edge);
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::insertVertex(size_t index)
{
	// В вектор списков смежности добавляем пустой список в позицию index
	this->matrix.insert(this->matrix.begin() + index, list<Node>());

	// Обновляем дескрипторы вершин в рёбрах, увеличивая индексы вершин входа,
	// больших или равных относительно данного
	for (auto& listEdge : this->matrix)
		for (auto& edge : listEdge)
			if (edge.v2 >= index)
				edge.v2++;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::deleteVertex(size_t index)
{
	// Сначала удаляем все рёбра, исходящие из неё
	for (const auto& node : this->matrix[index])
		delete node.edge;

	this->edgesCount -= this->matrix[index].size();
	this->matrix[index].clear();
	this->matrix.erase(this->matrix.begin() + index);

	// А затем удаляем все входящие рёбра

	size_t j = 0; // Число удалённых входящих рёбер

	// Лямбда-предикат для перебора и удаления рёбер, у которых номер входящей вершины совпадает с index.
	// Также лямбда-предикатом ведётся подсчёт количества удалённых рёбер через переменную j
	auto predicate = [index, &j](Node node) -> bool 
	{ 
		if (node.v2 == index) { j++; delete node.edge; return true; }
		else return false;
	};

	// Удаляем все рёбра, входящие в вершину под номером index
	for (auto& listEdge : this->matrix)
		listEdge.remove_if(predicate);

	this->edgesCount -= j;

	// Обновляем дескрипторы вершин в рёбрах, уменьшая индексы вершин входа, больших чем данный
	for (auto& listEdge : this->matrix)
		for (auto& edge : listEdge)
			if (edge.v2 > index)
				--edge.v2;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::insertEdge(size_t outV, size_t inV, Edge_T* edge)
{
	// Проверяем, есть ли заданное ребро в графе
	if (_isHasEdge(outV, inV))
		throw Graph_exception("такое ребро уже есть в графе");

	// Если ребро не принадлежит графу, то добавляем его
	this->matrix[outV].push_front(Node(edge, inV));

	this->edgesCount++;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::deleteEdge(size_t outV, size_t inV)
{
	typename list<Node>::iterator cur{};

	// Проверяем, есть ли заданное ребро в графе
	if (!_isHasEdge(outV, inV, &cur))
		throw Graph_exception("такого ребра нет в графе");

	// Если ребро принадлежит графу, то удаляем его
	Edge_T* edge = (*cur).edge;
	this->matrix[outV].erase(cur);
	delete edge;

	this->edgesCount--;
}

template <class Vertex_T, class Edge_T>
typename void ListGraph<Vertex_T, Edge_T>::print() const
{
	using std::cout;
	using std::endl;

	setlocale(LC_ALL, "Russian");

	// Если нет вершин, то переходим на следующую строку и выходим
	if (this->matrix.size() == 0)
	{
		cout << endl;

		return;
	}

	// Проходим по вектору списков рёбер
	for (size_t i = 0, end = this->matrix.size(); i < end; i++)
	{
		cout << "[" << this->vectorVertex[i]->getName() << "]: -> ";
		// Проходим по каждому списку рёбер
		for (const auto& node : this->matrix[i])
		{
			cout << "{" << node.edge->getVertexIn().getName() << ", ";

			try { cout << node.edge->getWeight(); }		// Пытаемся прочитать вес ребра
			catch (Graph_exception&) { cout << "?"; }	// Если он не установлен, выводим знак - ?
			cout << "} -> ";
		}
		cout << "null" << endl;
	}

	// Выводим информацию об обозначениях
	cout << "В квадратных скобках обозначено имя вершины исхода. Затем идёт список рёбер," << endl;
	cout << "исходящих из этой вершины. В фигурных скобках сначала обозначена вершина входа ребра," << endl;
	cout << "а затем вес этого ребра. Знаком \"?\" обозначены рёбра, вес которых не установлен." << endl;

	cout << endl;
}
//*************************************************************************************************************************************************//


//************************************************* Реализация методов ListEdgeIterator ***********************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::ListEdgeIterator(const ListGraph& graph, bool toBegin)
{
	this->graph_p = &graph;

	// Если решили изначально не устанавливать итератор
	if (!toBegin)
	{
		this->curV = -1;

		return;
	}

	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	// Ищем первый непустой список рёбер, и устанавливаем итератор на первое ребро в нём
	for (this->curV = 0; this->curV < size; this->curV++)
	{
		auto end = matrix[this->curV].end(); // Конец списка

		for (this->curE = matrix[this->curV].begin(), end = matrix[this->curV].end(); this->curE != end; this->curE++)
			// Если данный граф ориентирован, то устанавливаем итератор на первое ребро
			// Если не ориентирован, но при этом вершина входа больше вершины исхода,
			// то также устанавливаем итератор на него
			if (this->graph_p->directed || (!this->graph_p->directed && (*this->curE).v2 >= this->curV))
				return;
	}

	// Если итератор так и не был установлен
	this->curV = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator*() const
{
	if (this->curV == -1)
		throw Graph_exception("итератор не установлен");

	return (*(*this->curE).edge);
}

template <class Vertex_T, class Edge_T>
typename ListGraph<Vertex_T, Edge_T>::ListEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator++()
{
	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	if (this->curV == -1)
		throw Graph_exception("итератор не установлен");

	// Инкрементируем итератор
	this->curE++; 

	// Затем осуществляем следующую проверку
	while (this->curV < size)
	{
		auto end = matrix[this->curV].end(); // Конец списка

		// Проверяем не вышел ли итератор за пределы списка
		for (; this->curE != end; this->curE++)
			// Если не вышел, и при этом граф ориентирован, то возвращаем итератор
			// Также возвращаем итератор, если граф не ориентирован и номер входной вершины ребра больше
			// или равен номеру данной вершины
			if (this->graph_p->directed || (!this->graph_p->directed && (*this->curE).v2 >= this->curV))
				return *this;

		this->curV++;

		if (this->curV < size)
			this->curE = matrix[this->curV].begin();
	}

	this->curV = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator==(const EdgeIteratorForm& iter) const
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	// Если итераторы указывают на разные графы
	if (this->graph_p != l_iter.graph_p)
		return false;
	// Если они оба неустановлены на одном и том же графе
	else if (this->curV == -1 && l_iter.curV == -1)
		return true;
	// Если они установлены на одном и том же ребре графа
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return true;
	// Иные случаи
	else
		return false;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator!=(const EdgeIteratorForm& iter) const
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	// Если итераторы указывают на разные графы
	if (this->graph_p != l_iter.graph_p)
		return true;
	// Если они оба неустановлены на одном и том же графе
	else if (this->curV == -1 && l_iter.curV == -1)
		return false;
	// Если они установлены на одном и том же ребре графа
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return false;
	// Иные случаи
	else
		return true;
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>::ListEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListEdgeIterator::operator=(const EdgeIteratorForm& iter)
{
	const ListEdgeIterator& l_iter = dynamic_cast<const ListEdgeIterator&>(iter);

	if (this == &l_iter)
		return *this;

	this->graph_p = l_iter.graph_p;
	this->curV = l_iter.curV;
	this->curE = l_iter.curE;

	return *this;
}
//*************************************************************************************************************************************************//


//*************************************************** Реализация методов ListOutEdgeIterator ******************************************************//
// public-методы:
template <class Vertex_T, class Edge_T>
ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::ListOutEdgeIterator(const ListGraph& graph, size_t index, bool toBegin)
{
	this->graph_p = &graph;
	this->curV = index;

	// Если решили изначально не устанавливать итератор
	if (!toBegin)
	{
		this->curV = -1;

		return;
	}

	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	// Устанавливаем на первое ребро в списке исходящих рёбер для вершины с номером index 
	this->curE = matrix[this->curV].begin();

	// Если у данной вершины нет исходящих рёбер
	if (this->curE == matrix[this->curV].end())
		this->curV = -1;
}

template <class Vertex_T, class Edge_T>
typename Edge_T& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator*() const
{
	if (this->curV == -1)
		throw Graph_exception("итератор не установлен");

	return (*(*this->curE).edge);
}

template <class Vertex_T, class Edge_T>
typename ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator++()
{
	const vector<list<Node>>& matrix = this->graph_p->matrix;
	size_t size = this->graph_p->matrix.size();

	if (this->curV == -1)
		throw Graph_exception("итератор не установлен");

	this->curE++;

	// Проверяем перешёл ли итератор в неустановленнное состояние
	if (this->curE == matrix[this->curV].end())
		this->curV = -1;

	return *this;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator==(const OutEdgeIteratorForm& iter) const
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	// Если итераторы указывают на разные графы
	if (this->graph_p != l_iter.graph_p)
		return false;
	// Если они оба неустановлены на одном и том же графе
	else if (this->curV == -1 && l_iter.curV == -1)
		return true;
	// Если они установлены на одном и том же ребре графа
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return true;
	// Иные случаи
	else
		return false;
}

template <class Vertex_T, class Edge_T>
typename bool ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator!=(const OutEdgeIteratorForm& iter) const
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	// Если итераторы указывают на разные графы
	if (this->graph_p != l_iter.graph_p)
		return true;
	// Если они оба неустановлены на одном и том же графе
	else if (this->curV == -1 && l_iter.curV == -1)
		return false;
	// Если они установлены на одном и том же ребре графа
	else if (this->curV == l_iter.curV && this->curE == l_iter.curE)
		return false;
	// Иные случаи
	else
		return true;
}

template <class Vertex_T, class Edge_T>
const typename ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator& ListGraph<Vertex_T, Edge_T>::ListOutEdgeIterator::operator=(const OutEdgeIteratorForm& iter)
{
	const ListOutEdgeIterator& l_iter = dynamic_cast<const ListOutEdgeIterator&>(iter);

	if (this == &l_iter)
		return *this;

	this->graph_p = l_iter.graph_p;
	this->curV = l_iter.curV;
	this->curE = l_iter.curE;

	return *this;
}
//*************************************************************************************************************************************************//


#endif
