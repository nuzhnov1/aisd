// List.cpp


//*************************************************** Подключаемые заголовочные файлы *************************************************************//
#include <iostream>
#include <string>
#include <cstdlib>
#include <locale>
#include <ctime>
#include "List.h"
//*************************************************************************************************************************************************//


//************************************************************ Прототипы функций ******************************************************************//

// Получить число из стандартного потока ввода
int get_val();

//*************************************************************************************************************************************************//


//******************************************************** Используемые объекты из std ************************************************************//

using std::cout;
using std::cin;
using std::endl;

//*************************************************************************************************************************************************//



int main()
{
    List<int> lst;                              // Используемый список
    constexpr unsigned short MAX_SIZE = 1000;   // Максимальный размер списка
    int choose;                                 // Выбираемый пункт меню

    setlocale(LC_ALL, "Russian");

    cout << "\t\t\t\t\tПрограмма \"АТД-список\"." << endl << endl;

    cout << "Выберите тип заполнения списка:" << endl;
    cout << "\t1: Пустой список." << endl;
    cout << "\t2: Список с одинаковыми элементами." << endl;
    cout << "\t3: Список с случайными значениями." << endl;
    
    choose = get_val();
    while (choose < 1 || choose > 3)
    {
        cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
        choose = get_val();
    }
    switch (choose)
    {
    case 1:
    {
        break; // список изначально пустой, поэтому ничего не делаем
    }
    case 2:
    {
        int size;
        int val;

        cout << "Введите количество элементов списка(не больше " << MAX_SIZE << "):" << endl;
        size = get_val();
        while (size > MAX_SIZE || size < 0)
        {
            cout << "Ошибка: введённый размер меньше 0 или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
            size = get_val();
        }

        cout << "Введите значение элементов списка:" << endl;
        val = get_val();

        lst = List<int>(size, val);
        
        break;
    }
    case 3:
        int size;

        cout << "Введите количество элементов списка(не больше " << MAX_SIZE << "):" << endl;
        size = get_val();
        while (size > MAX_SIZE || size < 0)
        {
            cout << "Ошибка: введённый размер меньше 0 или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
            size = get_val();
        }
        lst = List<int>(size, 0);

        srand(static_cast<unsigned int>(time(nullptr)));
        for (List<int>::Iterator iter = lst.begin(), end = lst.end(); iter != end; iter++)
            *iter = rand();
        
        break;
    }
    cout << "Список успешно создан." << endl << endl;

    bool isExit = false;
    while (!isExit)
    {

        cout << "Меню программы \"АТД-Список\":" << endl;
        cout << "\t1: Получить размер текущего списка." << endl;
        cout << "\t2: Получить ёмкость списка." << endl;
        cout << "\t3: Получить количество переборов в списке на последней операции." << endl;
        cout << "\t4: Получить элемент списка по индексу." << endl;
        cout << "\t5: Установить значение элемента списка по указанному индексу." << endl;
        cout << "\t6: Получить индекс элемента списка." << endl;
        cout << "\t7: Проверка списка на пустоту." << endl;
        cout << "\t8: Проверка принадлежности данного элемента списку." << endl;
        cout << "\t9: Очистка списка." << endl;
        cout << "\t10: Добавить элемент в конец списка." << endl;
        cout << "\t11: Вставить элемент в указанную позицию списка." << endl;
        cout << "\t12: Удалить первый элемент, равный заданному, из списка." << endl;
        cout << "\t13: Удалить элемент из списка по указанному индексу." << endl;
        cout << "\t14: Вывести список." << endl;
        cout << "\t15: Перейти в раздел управления \"Прямой итератор\"." << endl;
        cout << "\t16: Перейти в раздел управления \"Обратный итератор\"." << endl;
        cout << "\t17: Выйти из программы." << endl;

        cout << "Выберите пункт меню:" << endl;
        choose = get_val();
        while (choose < 1 || choose > 17)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            choose = get_val();
        }

        cout << endl;

        switch (choose)
        {
        case 1:
        {
            cout << "Размер текущего списка: " << lst.getSize() << "." << endl;;
            
            break;
        }
        case 2:
        {
            cout << "Ёмкость списка: " << lst.getCapacity() << "." << endl;
            
            break;
        }
        case 3:
        {
            cout << "Количество переборов в списке на последней операции: " << lst.getLastOp() << "." << endl;
            
            break;
        }
        case 4:
        {
            cout << "Введите индекс элемента:" << endl;
            int index = get_val();
            
            try { 
                cout << "Элемент списка с данным индексом: " << lst[index] << "." << endl;
            }
            catch (List_exception& exception) 
            { 
                cout << "Ошибка: " << exception.what() << "." << endl; 
            }
            
            break;
        }
        case 5:
        {
            cout << "Введите индекс элемента:" << endl;
            int index = get_val();
            cout << "Введите элемента:" << endl;
            int val = get_val();

            try
            {
                lst[index] = val;
                cout << "Элемент успешно установлен." << endl;
            }
            catch (List_exception& exception) 
            { 
                cout << "Ошибка: " << exception.what() << "." << endl; 
            }
            
            break;
        }
        case 6:
        {
            cout << "Введите элемент:" << endl;
            int val = get_val();

            try
            {
                cout << "Данный элемент списка впервые встречается в нём под индексом: " << lst.index(val) << "." << endl;
            }
            catch (List_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }
            
            break;
        }
        case 7:
        {
            if (!lst.isEmpty())
                cout << "Список содержит элементы." << endl;
            else
                cout << "Список пуст." << endl;
            
            break;
        }
        case 8:
        {
            cout << "Введите элемент:" << endl;
            int val = get_val();

            if (lst.inList(val))
                cout << "Элемент присутствует в списке." << endl;
            else
                cout << "Элемента в списке нет." << endl;
            
            break;
        }
        case 9:
        {
            lst.clear();
            cout << "Список успешно очищен." << endl;
            
            break;
        }
        case 10:
        {
            if (lst.getSize() + 1 > MAX_SIZE)
                cout << "Ошибка: превышен допустимый размер списка: " << MAX_SIZE << "." << endl;
            else
            {
                cout << "Введите элемент:" << endl;
                int val = get_val();

                lst.append(val);
                cout << "Элемент успешно добавлен в список." << endl;
            }

            break;
        }
        case 11:
        {
            if (lst.getSize() + 1 > MAX_SIZE)
                cout << "Ошибка: превышен допустимый размер списка: " << MAX_SIZE << "." << endl;
            else
            {
                cout << "Введите индекс:" << endl;
                int index = get_val();
                cout << "Введите элемент:" << endl;
                int val = get_val();

                lst.insert(val, index);
                cout << "Элемент успешно добавлен в указанную позицию списка." << endl;
            }

            break;
        }
        case 12:
        {
            cout << "Введите элемент:" << endl;
            int val = get_val();

            try
            {
                lst.deleteItem(val);
                cout << "Элемент успешно удалён из списка." << endl;
            }
            catch (List_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }
           
            break;
        }
        case 13:
        {
            cout << "Введите индекс:" << endl;
            int index = get_val();

            try
            {
                lst.deleteItemByIndex(index);
                cout << "Элемент под данным индексом удалён из списка." << endl;
            }
            catch (List_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }
            
            break;
        }
        case 14:
        {
            cout << "Список:" << endl;
            lst.print();
            cout << endl;

            break;
        }
        case 15:
        {
            List<int>::Iterator iter;

            cout << "Раздел \"Прямой итератор\":" << endl << endl;

            cout << "Выберите тип создаваемого итератора:" << endl;
            cout << "\t1: Неустановленный." << endl;
            cout << "\t2: Установленный на определённый элемент." << endl;
            choose = get_val();
            while (choose < 1 || choose > 2)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_val();
            }
            switch (choose)
            {
            case 1:
            {
                iter = List<int>::Iterator(lst);
                
                break;
            }
            case 2:
            {
                cout << "Введите индекс:" << endl;
                int index = get_val();

                iter = List<int>::Iterator(lst, index);
                
                if (iter == lst.end())
                {
                    cout << "Внимание: итератор не был установлен из-за выхода индекса за пределы списка." << endl;
                    cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                }
                else
                    cout << "Итератор установлен в заданную позицию" << endl;
                
                break;
            }
            }
            cout << "Итератор успешно создан." << endl << endl;

            bool isExit = false;
            while (!isExit)
            {

                cout << "Меню \"Прямой итератор\":" << endl;
                cout << "\t1: Установить итератор по заданному индексу." << endl;
                cout << "\t2: Получить элемент итератора." << endl;
                cout << "\t3: Инкрементировать итератор." << endl;
                cout << "\t4: Декрементировать итератор." << endl;
                cout << "\t5: Прибавить к итератору \"n\"." << endl;
                cout << "\t6: Вычесть из итератора \"n\"." << endl;
                cout << "\t7: Выйти из данного раздела." << endl;

                cout << "Выберите пункт меню:" << endl;
                choose = get_val();
                while (choose < 1 || choose > 7)
                {
                    cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                    choose = get_val();
                }

                cout << endl;

                switch (choose)
                {
                case 1:
                {
                    cout << "Введите индекс:" << endl;
                    int index = get_val();

                    iter = List<int>::Iterator(lst, index);
                    if (iter == lst.end())
                    {
                        cout << "Внимание: итератор не был установлен из-за выхода индекса за пределы списка." << endl;
                        cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                    }
                    else
                        cout << "Итератор установлен в заданную позицию." << endl;
                   
                    break;
                }
                case 2:
                {
                    try
                    {
                        cout << "Элемент списка, на который в данный момент установлен итератор: " << *iter << "." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 3:
                {
                    try
                    {
                        iter++;
                        
                        if (iter == lst.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 4:
                {
                    try
                    {
                        iter--;
                        if (iter == lst.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 5:
                {
                    cout << "Введите значение \"n\":" << endl;
                    int n = get_val();

                    try
                    {
                        iter = iter + n;
                        if (iter == lst.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                   
                    break;
                }
                case 6:
                {
                    cout << "Введите значение \"n\":" << endl;
                    int n = get_val();

                    try
                    {
                        iter = iter - n;
                        if (iter == lst.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 7:
                {
                    cout << "Выходим из раздела \"Прямой итератор\"..." << endl;
                    isExit = true;
                    
                    break;
                }
                }
                cout << endl;
            }

            break;
        }
        case 16:
        {
            List<int>::RIterator riter;

            cout << "Раздел \"Обратный итератор\":" << endl << endl;

            cout << "Выберите тип создаваемого итератора:" << endl;
            cout << "\t1: Неустановленный." << endl;
            cout << "\t2: Установленный на определённый индекс." << endl;
            choose = get_val();
            while (choose < 1 || choose > 2)
            {
                cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                choose = get_val();
            }
            switch (choose)
            {
            case 1:
            {
                riter = List<int>::RIterator(lst);
                
                break;
            }
            case 2:
            {
                cout << "Введите индекс:" << endl;
                int index = get_val();

                riter = List<int>::RIterator(lst, index);
                if (riter == lst.rend())
                {
                    cout << "Внимание: итератор не был установлен из-за выхода индекса за пределы списка." << endl;
                    cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                }
                else
                    cout << "Итератор установлен в заданную позицию" << endl;
                
                break;
            }
            }
            cout << "Итератор успешно создан." << endl << endl;

            bool isExit = false;
            while (!isExit)
            {

                cout << "Меню \"Обратный итератор\":" << endl;
                cout << "\t1: Установить итератор по заданному индексу." << endl;
                cout << "\t2: Получить элемент итератора." << endl;
                cout << "\t3: Инкрементировать итератор." << endl;
                cout << "\t4: Декрементировать итератор." << endl;
                cout << "\t5: Прибавить к итератору \"n\"." << endl;
                cout << "\t6: Вычесть из итератора \"n\"." << endl;
                cout << "\t7: Выйти из данного раздела." << endl;

                cout << "Выберите пункт меню:" << endl;
                choose = get_val();
                while (choose < 1 || choose > 7)
                {
                    cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                    choose = get_val();
                }

                cout << endl;

                switch (choose)
                {
                case 1:
                {
                    cout << "Введите индекс:" << endl;
                    int index = get_val();

                    riter = List<int>::RIterator(lst, index);
                    if (riter == lst.rend())
                    {
                        cout << "Внимание: итератор не был установлен из-за выхода индекса за пределы списка." << endl;
                        cout << "Итератор перешёл в состояние \"неустановлен\"." << endl;
                    }
                    else
                        cout << "Итератор установлен в заданную позицию." << endl;
                    
                    break;
                }
                case 2:
                {
                    try
                    {
                        cout << "Элемент списка, на который в данный момент установлен итератор: " << *riter << "." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 3:
                {
                    try
                    {
                        riter++;
                        if (riter == lst.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 4:
                {
                    try
                    {
                        riter--;
                        if (riter == lst.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 5:
                {
                    cout << "Введите значение \"n\":" << endl;
                    int n = get_val();

                    try
                    {
                        riter = riter + n;
                        if (riter == lst.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 6:
                {
                    cout << "Введите значение \"n\":" << endl;
                    int n = get_val();

                    try
                    {
                        riter = riter - n;
                        if (riter == lst.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\", т.к. вышел за пределы списка." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (List_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }
                    
                    break;
                }
                case 7:
                {
                    cout << "Выходим из раздела \"Обратный итератор\"..." << endl;
                    isExit = true;
                    
                    break;
                }
                }
                cout << endl;
            }

            break;
        }
        case 17:
        {
            cout << "Выходим из программы..." << endl;
            isExit = true;
            
            break;
        }
        }
        cout << endl;
    }

    cout << "Программа успешно завершила свою работу. Нажмите Enter для выхода из программы...";
    cin.get();

    return 0;
}

int get_val()
{
    using std::string;
    using std::invalid_argument;
    using std::out_of_range;

    string buf;
    int val;

    cout << ">>> ";
    getline(cin, buf);

    while (true)
    {
        try
        {
            val = stoi(buf);
           
            break;
        }
        catch (invalid_argument)
        {
            cout << "Ошибка: неверный формат входных данных." << endl;
        }
        catch (out_of_range)
        {
            cout << "Ошибка: введённое число вышло из диапазона int." << endl;
        }

        cout << ">>> ";
        getline(cin, buf);
    }
    
    return val;
}
