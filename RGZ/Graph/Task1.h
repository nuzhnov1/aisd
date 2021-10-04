// Task1.h
#pragma once
#ifndef _INC_TASK1_
#define _INC_TASK1_
// ������������ ������������ �����
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Graph_exception.h"


//********************************************************* ����� ������ ������ *******************************************************************//
// ������ 16 - ������������ ������ ���� ������������������ ����� � ������� �������������� ���������� �����
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

	vector<char> colors;					// ����� ������: 'w' - �����, 'g' - ����� � 'b' - ������
	vector<size_t> sub_graph;			// ������� ���������������
	vector<vector<size_t>> matrix;		// ������� ������� ������
	size_t n;							// ���������� ������
	stack<size_t> stk;					// ��������������� ����
	vector<string> names;				// ������ ��� ������
	string res;							// ��������� ������� ������
	const Graph_T* graph;				// ����, ��� �������� �������� ������

	// �������� ������ ������� �� �����
	size_t _getIndex(const string& str);
	// ������ ������
	void _solve();
	// ����� � ������� ��� �������
	void DFS_visit(size_t i);

public:

	// ����������� ������ � ������� �� ����
	Task1(const Graph_T& graph);
	// ����������� ����������� �����
	Task1(const Task1& task);
	// ���������� ������
	~Task1();

	// ������� ������ � ������ "graph" � ������ �
	void set(const Graph_T& graph);
	// �������� ��������� ������� ������
	void restart();
	// ���������� ��������� ������� ������
	string result();

};
//*************************************************************************************************************************************************//


//************************************************************** ���������� ������� Task1 *********************************************************//
// private-������:
size_t Task1::_getIndex(const string& str)
{
	// �������� �� ������� ��� � ������ ����, ������� ��������� � str
	for (size_t i = 0, size = this->names.size(); i < size; i++)
		if (this->names[i] == str)
			return i;

	return -1;
}

void Task1::_solve()
{
	OE_Iterator oe_iter;	// �������� ��������� ����
	OE_Iterator oe_end;		// ����� ��� ��������� ��������� ����
	V_Iterator v_iter;		// �������� ������
	V_Iterator v_end;		// ����� ��� ��������� ������

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	// �������� �� ���� ��������
	for (size_t i = 0; i < this->n && v_iter != v_end; i++, v_iter++) 
	{
		oe_iter = OE_Iterator::begin(*this->graph, *v_iter);
		oe_end = OE_Iterator::end(*this->graph, *v_iter);

		// �������� �� ��������� ����� ������ �������
		for (; oe_iter != oe_end; oe_iter++) 
		{
			const Edge_T& edge = *oe_iter;

			this->matrix[i].push_back(_getIndex(edge.getVertexIn().getName()));
		}
	}

	// ������ ����� � ������� ��� ������ �������. ���������� ������� ������� � ��������������� ����
	for (size_t i = 0; i < this->n; i++)
		if (this->colors[i] == 'w') 
			DFS_visit(i);

	vector<size_t> temp(this->stk.size());
	
	// ������� ����, ������ ������� � ������ temp
	for (size_t i = 0, size = temp.size(); i < size; i++) 
	{
		temp[i] = this->stk.top();
		this->stk.pop();
	}

	// ������� ������ � ��������� ������ ����
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
	this->colors[i] = 'g';	// ������ ������� ������ �����
	
	// ������� � ������� �������
	for (auto& item : this->matrix[i]) 
	{
		// ���� ������ ������� ������� �����
		if (this->colors[item] == 'w') 
		{
			// �� �������� ����� (i, item) � ����
			this->stk.push(i);
			this->stk.push(item);

			// � �������� �������� ������� item ���������� ������� i
			this->sub_graph[item] = i;
			// ���������� ������ ����� � ������� ��� ������� �������
			DFS_visit(item);

			// � ���� �������� ����� (item, i)
			this->stk.push(item);
			this->stk.push(i);
		}
		// ���� ������� ������� - �����, � ��� �� �������� ��������� ������ �������
		else if (this->colors[item] == 'g' && this->sub_graph[i] != item) 
		{
			// �� �������� ����� (i, item)
			this->stk.push(i);
			this->stk.push(item);
			// � ��������������� ����� (item, i)
			this->stk.push(item);
			this->stk.push(i);
		}
	}

	// ������ ������ ������� ������
	this->colors[i] = 'b';
}


// public-������:
Task1::Task1(const Graph_T& graph)
{
	// ��������� ������ � ������
	this->graph = &graph;

	// ������������� �������� ���������
	this->n = graph.getVertexCount();
	this->colors.resize(this->n, 'w');
	this->sub_graph.resize(this->n, -1);
	this->matrix.resize(this->n);
	this->stk = stack<size_t>();
	this->res = "";

	// �������������� ����� ������
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();
}

Task1::Task1(const Task1& task)
{
	// �������� �������� ���������
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
	// ������� ������� ������ � ������ ����� ���������������
	this->colors.clear();
	this->sub_graph.clear();
	
	// ������� ������� � ������� � ���� �������
	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	// ������� ��������������� ����
	while (!this->stk.empty())
		this->stk.pop();
}

void Task1::set(const Graph_T& graph)
{
	// ������� ���������� ������� ����������� ����������� ������
	this->colors.clear();
	this->sub_graph.clear();

	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	while (!this->stk.empty())
		this->stk.pop();

	// ����� ������������� ����� ���������, ������ �� ���������� ��������� �����
	if (graph.isDirected())
		throw Graph_exception("���� ���������������");

	this->graph = &graph;

	this->n = graph.getVertexCount();
	this->colors.resize(this->n, 'w');
	this->sub_graph.resize(this->n, -1);
	this->matrix.resize(this->n);
	this->stk = stack<size_t>();

	// �������������� ����� ������
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
	// ������� ���������� ������� ����������� �����������
	this->colors.clear();
	this->sub_graph.clear();

	for (auto& vec : this->matrix)
		vec.clear();

	this->matrix.clear();

	this->names.clear();

	while (!this->stk.empty())
		this->stk.pop();

	// ����� ������������� ����� ���������, ������ �� ���������� ��������� �����
	if (this->graph->isDirected())
		throw Graph_exception("���� ���������������");

	// ������������� � ��������� �������� �� ���������
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
		throw Graph_exception("������ ��� �� ���� ������");
	else
		return this->res;
}
//*************************************************************************************************************************************************//


#endif
