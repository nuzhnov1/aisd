// Graph_exception.h
#pragma once
#ifndef _INC_GRAPH_EXCEPTION_
#define _INC_GRAPH_EXCEPTION_
// ������������ ������������ �����
#include <string>


//********************************************************* ����� ���������� ����� ****************************************************************//
class Graph_exception
{

	using string = std::string;

protected:

	string error;	// ������ ������

public:

	Graph_exception(const string& error) : error(error) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


#endif
