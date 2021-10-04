// Task1.h
#pragma once
#ifndef _INC_TASK1_
#define _INC_TASK1_
// Подключаемые заголовочные файлы
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Graph_exception.h"


//********************************************************* Класс первой задачи *******************************************************************//
// Задача 16 - формирование списка рёбер неориентированного графа в порядке двухпроходного эйлерового цикла
class Task1
{

	template <typename T> using vector = std::vector<T>;
	template <typename T> using stack = std::stack<T>;
	using string = std::string;
	using Vertex_T = Vertex<string, int>;
	using Edge_T = Edge<Vertex_T, int, int>;
	using Graph_T = Graph<Vertex_T, Edge_T>;
	using V_Iterator = Graph_T::VertexIterator;
	using OE_Iterator = Graph_T::OutEdgeIterator;

	vector<char> colors;					// Цвета вершин: 'w' - белый, 'g' - серый и 'b' - чёрный
	vector<size_t> sub_graph;			// Подграф предшествования
	vector<vector<size_t>> matrix;		// Матрица смежных вершин
	size_t n;							// Количество вершин
	stack<size_t> stk;					// Вспомогательный стек
	vector<string> names;				// Вектор имён вершин
	string res;							// Результат решения задачи
	const Graph_T* graph;				// Граф, для которого решается задача

	// Получить индекс вершины по имени
	size_t _getIndex(const string& str);
	// Решить задачу
	void _solve();
	// Поиск в глубину для вершины
	void DFS_visit(size_t i);

public:

	// Конструктор задачи с ссылкой на граф
	Task1(const Graph_T& graph);
	// Конструктор копирования задач
	Task1(const Task1& task);
	// Деструктор задачи
	~Task1();

	// Связать задачу с графом "graph" и решить её
	void set(const Graph_T& graph);
	// Повторно выполнить решение задачи
	void restart();
	// Возвратить результат решения задачи
	string result();

};
//*************************************************************************************************************************************************//


//************************************************************** Реализация методов Task1 *********************************************************//
// private-методы:
size_t Task1::_getIndex(const string& str)
{
	// Проходим по вектору имён в поиске того, который совпадает с str
	for (size_t i = 0, size = this->names.size(); i < size; i++)
		if (this->names[i] == str)
			return i;

	return -1;
}

void Task1::_solve()
{
	OE_Iterator oe_iter;	// Итератор исходящих рёбер
	OE_Iterator oe_end;		// Конец для итератора исходящих рёбер
	V_Iterator v_iter;		// Итератор вершин
	V_Iterator v_end;		// Конец для итератора вершин

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	// Проходим по всем вершинам
	for (size_t i = 0; i < this->n && v_iter != v_end; i++, v_iter++) 
	{
		oe_iter = OE_Iterator::begin(*this->graph, *v_iter);
		oe_end = OE_Iterator::end(*this->graph, *v_iter);

		// Проходим по исходящим рёбрам данной вершины
		for (; oe_iter != oe_end; oe_iter++) 
		{
			const Edge_T& edge = *oe_iter;

			this->matrix[i].push_back(_getIndex(edge.getVertexIn().getName()));
		}
	}

	// Делаем обход в глубину для каждой вершины. Результаты прохода заносим в вспомогательный стек
	for (size_t i = 0; i < this->n; i++)
		if (this->colors[i] == 'w') 
			DFS_visit(i);

	vector<size_t> temp(this->stk.size());
	
	// Обходим стек, вынося вершины в вектор temp
	for (size_t i = 0, size = temp.size(); i < size; i++) 
	{
		temp[i] = this->stk.top();
		this->stk.pop();
	}

	// Обходим вектор и формируем строку рёбер
	this->res = "[";

	if (temp.size() > 0)
	{
		for (size_t i = temp.size() - 1; i > 1; i -= 2)
			this->res += "(" + this->names[temp[i]] + ", " + this->names[temp[i - 1]] + "), ";

		if (temp.size() >= 2)
			this->res += "(" + this->names[temp[1]] + ", " + this->names[temp[0]] + ")";

		temp.clear();
	}

	this->res += "]";
}

void Task1::DFS_visit(size_t i) 
{
	this->colors[i] = 'g';	// Данную вершину делаем серой
	
	// Обходим её смежные вершины
	for (auto& item : this->matrix[i]) 
	{
		// Если данная смежная вершина белая
		if (this->colors[item] == 'w') 
		{
			// То помещаем ребро (i, item) в стек
			this->stk.push(i);
			this->stk.push(item);

			// В качестве родителя вершины item записываем вершину i
			this->sub_graph[item] = i;
			// Рекурсивно делаем обход в глубину для смежной вершины
			DFS_visit(item);

			// В стек помещаем ребро (item, i)
			this->stk.push(item);
			this->stk.push(i);
		}
		// Если смежная вершина - серая, и она не является родителем данной вершины
		else if (this->colors[item] == 'g' && this->sub_graph[i] != item) 
		{
			// То помещаем ребро (i, item)
			this->stk.push(i);
			this->stk.push(item);
			// И противоположное ребро (item, i)
			this->stk.push(item);
			this->stk.push(i);
		}
	}

	// Делаем данную вершину чёрной
	this->colors[i] = 'b';
}


// public-методы:
Task1::Task1(const Graph_T& graph)
{
	// Связываем задачу с графом
	this->graph = &graph;

	// Инициализиуем основные параметры
	this->n = graph.getVertexCount();
	this->colors.resize(this->n, 'w');
	this->sub_graph.resize(this->n, -1);
	this->matrix.resize(this->n);
	this->stk = stack<size_t>();
	this->res = "";

	// Инициализируем имена вершин
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();
}

Task1::Task1(const Task1& task)
{
	// Копируем основные параметры
	this->graph = task.graph;
	this->n = task.n;
	this->colors = task.colors;
	this->sub_graph = task.sub_graph;
	this->matrix.resize(task.matrix.size());
	this->names = task.names;
	this->stk = task.stk;
	this->res = task.res;

	for (size_t i = 0; i < n; i++)
		this->matrix[i] = task.matrix[i];
}

Task1::~Task1()
{
	// Очищаем вектора цветов и вектор графа предшествования
	this->colors.clear();
	this->sub_graph.clear();
	
	// Очищаем векторы в матрице и саму матрицу
	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	// Очищаем вспомогательный стек
	while (!this->stk.empty())
		this->stk.pop();
}

void Task1::set(const Graph_T& graph)
{
	// Сначала производим очистку предыдущего содержимого задачи
	this->colors.clear();
	this->sub_graph.clear();

	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	while (!this->stk.empty())
		this->stk.pop();

	// Затем устанавливаем новые параметры, исходя из параметров заданного графа
	if (graph.isDirected())
		throw Graph_exception("граф ориентированный");

	this->graph = &graph;

	this->n = graph.getVertexCount();
	this->colors.resize(this->n, 'w');
	this->sub_graph.resize(this->n, -1);
	this->matrix.resize(this->n);
	this->stk = stack<size_t>();

	// Инициализируем имена вершин
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();

	_solve();
}

void Task1::restart()
{
	// Сначала производим очистку предыдущего содержимого
	this->colors.clear();
	this->sub_graph.clear();

	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	while (!this->stk.empty())
		this->stk.pop();

	// Затем устанавливаем новые параметры, исходя из параметров заданного графа
	if (this->graph->isDirected())
		throw Graph_exception("граф ориентированный");

	// Устанавливаем в структуры значения по умолчанию
	this->n = this->graph->getVertexCount();
	this->colors.resize(this->n, 'w');
	this->sub_graph.resize(this->n, -1);
	this->matrix.resize(this->n);
	this->stk = stack<size_t>();

	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();

	_solve();
}

Task1::string Task1::result()
{
	if (this->res == "")
		throw Graph_exception("задача ещё не была решена");
	else
		return this->res;
}
//*************************************************************************************************************************************************//


#endif
