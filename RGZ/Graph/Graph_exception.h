// Graph_exception.h
#pragma once
#ifndef _INC_GRAPH_EXCEPTION_
#define _INC_GRAPH_EXCEPTION_
// Подключаемые заголовочные файлы
#include <string>


//********************************************************* Класс исключений графа ****************************************************************//
class Graph_exception
{

	using string = std::string;

protected:

	string error;	// Строка ошибки

public:

	Graph_exception(const string& error) : error(error) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


#endif
