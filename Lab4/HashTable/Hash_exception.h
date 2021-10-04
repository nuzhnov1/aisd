// Exception.h
#pragma once
#ifndef _INC_HASH_EXCEPTION_
#define _INC_HASH_EXCEPTION_
// Подлкючаемые заголовочные файлы
#include <string>


//*************************************************************** Класс исключений ****************************************************************//
class Hash_exception
{
	
	using string = std::string;

	string error;

public:

	Hash_exception(const string& str) : error(str) {}

	string what() const { return this->error; }

};
//*************************************************************************************************************************************************//


#endif
