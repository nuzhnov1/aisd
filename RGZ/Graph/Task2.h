// Task2.h
#pragma once
#ifndef _INC_TASK2_
#define _INC_TASK2_
// Подключаемые заголовочные файлы
#include <iostream>
#include <limits>
#include <vector>
#include <stack>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Graph_exception.h"


//********************************************************* Класс второй задачи *******************************************************************//
// Задача 1 - определение диаметра и пути, соответствующего диаметру, на основе алгоритма Флойда
class Task2
{
	template <typename T> using vector = std::vector<T>;
	template <typename T> using stack = std::stack<T>;
	using string = std::string;
	using Vertex_T = Vertex<string, int>;
	using Edge_T = Edge<Vertex_T, int, int>;
	using Graph_T = Graph<Vertex_T, Edge_T>;
	using V_Iterator = Graph_T::VertexIterator;
	using E_Iterator = Graph_T::EdgeIterator;
	using OE_Iterator = Graph_T::OutEdgeIterator;

	const Graph_T* graph;	// Граф, для которого решается задача
	size_t n;				// Число вершин графа
	vector<string> names;	// Вектор имён вершин
	string res;				// Результат решения задачи

	// Получить индекс вершины по имени
	size_t _getIndex(const string& str);
	// Инициализировать матрицу весов
	void _InitWeightMatrix(vector<vector<double>>& W_Matrix);
	// Записать в res путь, соответствующий диаметру
	void _WritePath(const vector<vector<size_t>> P_Matrix, size_t start, size_t end);
	// Функция решения задачи
	void _solve();

public:

	// Конструктор задачи с ссылкой на граф
	Task2(const Graph_T& graph);
	// Конструктор копирования задач
	Task2(const Task2& task);
	// Деструктор задачи
	~Task2();

	// Связать задачу с графом "graph" и решить её
	void set(const Graph_T& graph);
	// Повторно выполнить решение задачи
	void restart();
	// Возвратить результат решения задачи
	string result();

};
//*************************************************************************************************************************************************//


//************************************************************* Реализация методов Task2 **********************************************************//
// private-методы:
size_t Task2::_getIndex(const string& str)
{
	// Проходим по вектору имён в поиске того, который совпадает с str
	for (size_t i = 0, size = this->names.size(); i < size; i++)
		if (this->names[i] == str)
			return i;

	return -1;
}

void Task2::_InitWeightMatrix(vector<vector<double>>& W_Matrix)
{
	V_Iterator v_iter = V_Iterator::begin(*this->graph);
	V_Iterator v_end = V_Iterator::end(*this->graph);

	for (size_t i = 0; v_iter != v_end; i++, v_iter++)
	{
		OE_Iterator oe_iter = OE_Iterator::begin(*this->graph, *v_iter);
		OE_Iterator oe_end = OE_Iterator::end(*this->graph, *v_iter);

		for (size_t j = 0; oe_iter != oe_end; j++, oe_iter++)
		{
			Edge_T edge = *oe_iter;
			size_t out_v = _getIndex(edge.getVertexOut().getName());
			size_t inp_v = _getIndex(edge.getVertexIn().getName());
			
			W_Matrix[out_v][inp_v] = static_cast<double>(edge.getWeight());
		}
	}
}

void Task2::_WritePath(const vector<vector<size_t>> P_Matrix, size_t start, size_t end)
{
	if (start == end)
		this->res += this->names[end] + " -> ";
	else
	{
		_WritePath(P_Matrix, start, P_Matrix[start][end]);
		this->res += this->names[end] + " -> ";
	}
}

void Task2::_solve()
{
	constexpr double inf = std::numeric_limits<double>::infinity();			// Условная бесконечность
	vector<vector<double>> W_Matrix(this->n, vector<double>(this->n, inf));	// Матрица весов
	vector<vector<double>> D_Matrix(this->n, vector<double>(this->n, 0));	// Матрица весов кратчайших путей
	vector<vector<size_t>> P_Matrix(this->n, vector<size_t>(this->n, -1));	// Матрица предшествования

	// Инициализируем все три матрицы
	_InitWeightMatrix(W_Matrix);

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (i != j)
				D_Matrix[i][j] = W_Matrix[i][j];

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (i != j && W_Matrix[i][j] != inf)
				P_Matrix[i][j] = i;

	// Выполняем алгоритм Флойда для данных матриц
	for (size_t k = 0; k < this->n; k++)
		for (size_t i = 0; i < this->n; i++)
			for (size_t j = 0; j < this->n; j++)
				if (D_Matrix[i][j] > D_Matrix[i][k] + D_Matrix[k][j])
				{
					D_Matrix[i][j] = D_Matrix[i][k] + D_Matrix[k][j];
					P_Matrix[i][j] = P_Matrix[k][j];
				}

	// По полученной матрицы весов кратчайших путей находим диаметр и индексы вершин,
	// которые ему соответствуют
	double diam = -inf;				// Диаметр графа
	size_t out_v = -1, inp_v = -1;	// Конечные вершины, образующие путь диаметра

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (D_Matrix[i][j] != inf && D_Matrix[i][j] > diam)
			{
				diam = D_Matrix[i][j];
				out_v = i;
				inp_v = j;
			}

	this->res = "Диаметр графа: " + std::to_string(diam) + ".\n";
	if (diam < inf)
	{
		this->res += "Путь, соответствующий данному диаметру:\n";
		_WritePath(P_Matrix, out_v, inp_v);
		this->res.erase(res.size() - 4);
	}

	// Очищаем матрицы
	for (auto& vec : P_Matrix)
		vec.clear();

	P_Matrix.clear();

	for (auto& vec : D_Matrix)
		vec.clear();

	D_Matrix.clear();

	for (auto& vec : W_Matrix)
		vec.clear();

	W_Matrix.clear();
}


// public-методы:
Task2::Task2(const Graph_T& graph)
{
	// Связываем задачу с графом
	this->graph = &graph;

	// Инициализиуем основные параметры
	this->n = graph.getVertexCount();
	this->res = "";

	// Инициализируем имена вершин
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();
}

Task2::Task2(const Task2& task)
{
	// Копируем основные параметры
	this->graph = task.graph;
	this->n = task.n;
	this->names = task.names;
	this->res = task.res;
}

Task2::~Task2()
{
	// Очищаем вектор имён вершин
	this->names.clear();
}

void Task2::set(const Graph_T& graph)
{
	// Сначала производим очистку вектора имён вершин
	this->names.clear();

	if (graph.getEdgesCount() == 0)
		throw Graph_exception("в графе нет рёбер");

	// Связываем задачу с графом
	this->graph = &graph;

	// Инициализиуем основные параметры
	this->n = graph.getVertexCount();
	this->res = "";

	// Инициализируем имена вершин
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();

	_solve();
}

void Task2::restart()
{
	// Сначала производим очистку предыдущего содержимого задачи,
	// поскольку до перезапуска задачи граф мог поменяться
	this->names.clear();

	if (this->graph->getEdgesCount() == 0)
		throw Graph_exception("в графе нет рёбер");

	this->n = this->graph->getVertexCount();
	this->res = "";

	// Заново инициализируем имена вершин
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();

	_solve();
}

Task2::string Task2::result()
{
	if (this->res == "")
		throw Graph_exception("задача ещё не была решена");
	else
		return this->res;
}
//*************************************************************************************************************************************************//



#endif
