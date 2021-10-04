// Task2.h
#pragma once
#ifndef _INC_TASK2_
#define _INC_TASK2_
// ������������ ������������ �����
#include <iostream>
#include <limits>
#include <vector>
#include <stack>
#include <string>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Graph_exception.h"


//********************************************************* ����� ������ ������ *******************************************************************//
// ������ 1 - ����������� �������� � ����, ���������������� ��������, �� ������ ��������� ������
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

	const Graph_T* graph;	// ����, ��� �������� �������� ������
	size_t n;				// ����� ������ �����
	vector<string> names;	// ������ ��� ������
	string res;				// ��������� ������� ������

	// �������� ������ ������� �� �����
	size_t _getIndex(const string& str);
	// ���������������� ������� �����
	void _InitWeightMatrix(vector<vector<double>>& W_Matrix);
	// �������� � res ����, ��������������� ��������
	void _WritePath(const vector<vector<size_t>> P_Matrix, size_t start, size_t end);
	// ������� ������� ������
	void _solve();

public:

	// ����������� ������ � ������� �� ����
	Task2(const Graph_T& graph);
	// ����������� ����������� �����
	Task2(const Task2& task);
	// ���������� ������
	~Task2();

	// ������� ������ � ������ "graph" � ������ �
	void set(const Graph_T& graph);
	// �������� ��������� ������� ������
	void restart();
	// ���������� ��������� ������� ������
	string result();

};
//*************************************************************************************************************************************************//


//************************************************************* ���������� ������� Task2 **********************************************************//
// private-������:
size_t Task2::_getIndex(const string& str)
{
	// �������� �� ������� ��� � ������ ����, ������� ��������� � str
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
	constexpr double inf = std::numeric_limits<double>::infinity();			// �������� �������������
	vector<vector<double>> W_Matrix(this->n, vector<double>(this->n, inf));	// ������� �����
	vector<vector<double>> D_Matrix(this->n, vector<double>(this->n, 0));	// ������� ����� ���������� �����
	vector<vector<size_t>> P_Matrix(this->n, vector<size_t>(this->n, -1));	// ������� ���������������

	// �������������� ��� ��� �������
	_InitWeightMatrix(W_Matrix);

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (i != j)
				D_Matrix[i][j] = W_Matrix[i][j];

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (i != j && W_Matrix[i][j] != inf)
				P_Matrix[i][j] = i;

	// ��������� �������� ������ ��� ������ ������
	for (size_t k = 0; k < this->n; k++)
		for (size_t i = 0; i < this->n; i++)
			for (size_t j = 0; j < this->n; j++)
				if (D_Matrix[i][j] > D_Matrix[i][k] + D_Matrix[k][j])
				{
					D_Matrix[i][j] = D_Matrix[i][k] + D_Matrix[k][j];
					P_Matrix[i][j] = P_Matrix[k][j];
				}

	// �� ���������� ������� ����� ���������� ����� ������� ������� � ������� ������,
	// ������� ��� �������������
	double diam = -inf;				// ������� �����
	size_t out_v = -1, inp_v = -1;	// �������� �������, ���������� ���� ��������

	for (size_t i = 0; i < this->n; i++)
		for (size_t j = 0; j < this->n; j++)
			if (D_Matrix[i][j] != inf && D_Matrix[i][j] > diam)
			{
				diam = D_Matrix[i][j];
				out_v = i;
				inp_v = j;
			}

	this->res = "������� �����: " + std::to_string(diam) + ".\n";
	if (diam < inf)
	{
		this->res += "����, ��������������� ������� ��������:\n";
		_WritePath(P_Matrix, out_v, inp_v);
		this->res.erase(res.size() - 4);
	}

	// ������� �������
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


// public-������:
Task2::Task2(const Graph_T& graph)
{
	// ��������� ������ � ������
	this->graph = &graph;

	// ������������� �������� ���������
	this->n = graph.getVertexCount();
	this->res = "";

	// �������������� ����� ������
	V_Iterator v_iter, v_end;

	v_iter = V_Iterator::begin(*this->graph);
	v_end = V_Iterator::end(*this->graph);

	this->names.resize(this->n);

	for (size_t i = 0; v_iter != v_end; v_iter++, i++)
		this->names[i] = (*v_iter).getName();
}

Task2::Task2(const Task2& task)
{
	// �������� �������� ���������
	this->graph = task.graph;
	this->n = task.n;
	this->names = task.names;
	this->res = task.res;
}

Task2::~Task2()
{
	// ������� ������ ��� ������
	this->names.clear();
}

void Task2::set(const Graph_T& graph)
{
	// ������� ���������� ������� ������� ��� ������
	this->names.clear();

	if (graph.getEdgesCount() == 0)
		throw Graph_exception("� ����� ��� ����");

	// ��������� ������ � ������
	this->graph = &graph;

	// ������������� �������� ���������
	this->n = graph.getVertexCount();
	this->res = "";

	// �������������� ����� ������
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
	// ������� ���������� ������� ����������� ����������� ������,
	// ��������� �� ����������� ������ ���� ��� ����������
	this->names.clear();

	if (this->graph->getEdgesCount() == 0)
		throw Graph_exception("� ����� ��� ����");

	this->n = this->graph->getVertexCount();
	this->res = "";

	// ������ �������������� ����� ������
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
		throw Graph_exception("������ ��� �� ���� ������");
	else
		return this->res;
}
//*************************************************************************************************************************************************//



#endif
