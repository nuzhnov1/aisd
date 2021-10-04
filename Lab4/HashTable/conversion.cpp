// conversion.cpp


// Подключаемые заголовочные файлы
#include <string>


unsigned int conversion(const std::string& val)
{
	// Важной составляющей данного алгоритма является арифметическое переполнение, которое происходит практически всегда.
	// В качестве возвращаемого типа был выбран 32-битный безнаковый тип - unsigned int, поскольку результирующее число 
	// после преобразования не должно быть слишком большим.

	const unsigned char base = 32; // Число букв в русском алфавите без буквы Ё 

	unsigned int sum = 0;

	for (auto& iter : val)
		sum = sum * base + (static_cast<unsigned char>(iter) - static_cast<unsigned char>('А') + 1);

	return sum;
}

unsigned int conversion(const unsigned long long& val)
{
	return static_cast<unsigned int>(val);
}
