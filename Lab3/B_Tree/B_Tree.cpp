// B_Tree.cpp


//*************************************************** Подключаемые заголовочные файлы *************************************************************//
#include <iostream>
#include <string>
#include <cstdlib>
#include <locale>
#include <ctime>
#include "Tree23.h"
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
    Tree23<int, int> tree;                      // Используемое 2-3 дерево
    constexpr unsigned short MAX_SIZE = 1000;   // Максимальный размер дерева
    int choose;                                 // Выбираемый пункт меню
    
    setlocale(LC_ALL, "Russian");

    cout << "\t\t\t\t\tПрограмма \"АТД 2-3 дерево\"." << endl << endl;

    cout << "Выберите тип заполнения 2-3 дерева:" << endl;
    cout << "\t1: Пустое дерево." << endl;
    cout << "\t2: Дерево с случайными данными." << endl;

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
        break; // дерево изначально пустое, поэтому ничего не делаем
    }

    case 2:
    {
        Tree23<int, int> temp;
        int size;

        cout << "Введите количество элементов дерева(не больше " << MAX_SIZE << "):" << endl;
        size = get_val();
        while (size > MAX_SIZE || size < 0)
        {
            cout << "Ошибка: введённый размер меньше 0 или превышает " << MAX_SIZE << ". Повторите ввод." << endl;
            size = get_val();
        }

        srand(static_cast<unsigned int>(time(nullptr)));
        for (size_t i = 0; i < static_cast<size_t>(size); i++)
        {
            try { temp.append(rand(), rand()); }
            catch (Tree23_exception&) { i--; }
        } 
        
        tree = temp;

        break;
    }
    }
    cout << "2-3 дерево успешно создано." << endl << endl;

    bool isExit = false;
    while (!isExit)
    {
        cout << "Меню программы \"АТД 2-3 дерево\":" << endl;
        cout << "\t1: Получить размер текущего дерева." << endl;
        cout << "\t2: Получить количество переборов в дереве на последней операции." << endl;
        cout << "\t3: Получить элемент дерева по ключу." << endl;
        cout << "\t4: Установить новое значение элемента дерева по ключу." << endl;
        cout << "\t5: Проверка дерева на пустоту." << endl;
        cout << "\t6: Очистка дерева." << endl;
        cout << "\t7: Добавить элемент в дерево с заданным ключом и данными." << endl;
        cout << "\t8: Удалить элемент из дерева по ключу." << endl;
        cout << "\t9: Вывести дерево в древовидной форме." << endl;
        cout << "\t10: Вывести дерево в форме словаря(ключ-значение)." << endl;
        cout << "\t11: Перейти в раздел управления \"Прямой итератор\"." << endl;
        cout << "\t12: Перейти в раздел управления \"Обратный итератор\"." << endl;
        cout << "\t13: Выйти из программы." << endl;

        cout << "Выберите пункт меню:" << endl;
        choose = get_val();
        while (choose < 1 || choose > 13)
        {
            cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
            choose = get_val();
        }

        cout << endl;

        switch (choose)
        {
        case 1:
        {
            cout << "Размер текущего дерева: " << tree.getSize() << "." << endl;
            break;
        }

        case 2:
        {
            cout << "Количество переборов в дереве на последней операции: " << tree.getLastOp() << "." << endl;
            break;
        }

        case 3:
        {
            cout << "Введите ключ:" << endl;
            int key = get_val();

            try
            {
                cout << "Элемент дерева с данным ключом: " << tree[key] << "." << endl;
            }
            catch (Tree23_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 4:
        {
            cout << "Введите ключ:" << endl;
            int key = get_val();
            cout << "Введите элемент:" << endl;
            int val = get_val();

            try
            {
                tree[key] = val;
                cout << "Элемент успешно установлен." << endl;
            }
            catch (Tree23_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 5:
        {
            if (!tree.isEmpty())
                cout << "Дерево содержит элементы." << endl;
            else
                cout << "Дерево пусто." << endl;
            
            break;
        }

        case 6:
        {
            tree.clear();
            cout << "Дерево успешно очищено." << endl;
            
            break;
        }

        case 7:
        {
            if (tree.getSize() + 1 > MAX_SIZE)
            {
                cout << "Ошибка: превышен допустимый размер дерева: " << MAX_SIZE << "." << endl;

                break;
            }

            cout << "Введите ключ:" << endl;
            int key = get_val();
            cout << "Введите элемент:" << endl;
            int val = get_val();

            try
            {
                tree.append(key, val);
                cout << "Элемент с данным ключом успешно добавлен в дерево." << endl;
            }
            catch (Tree23_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }   
            
            break;
        }

        case 8:
        {
            cout << "Введите ключ:" << endl;
            int key = get_val();

            try
            {
                tree.remove(key);
                cout << "Элемент с заданным ключом успешно удалён из дерева." << endl;
            }
            catch (Tree23_exception& exception)
            {
                cout << "Ошибка: " << exception.what() << "." << endl;
            }

            break;
        }

        case 9:
        {
            cout << "Структура текущего 2-3 дерева в древовидном виде:" << endl;
            tree.print();
            
            break;
        }

        case 10:
        {
            cout << "Структура текущего 2-3 дерева в виде словаря \"ключ-значение\":" << endl;
            tree.printAsDict();
            
            break;
        }

        case 11:
        {
            Tree23<int, int>::Iterator iter;

            cout << "Раздел \"Прямой итератор\":" << endl << endl;

            cout << "Выберите тип создаваемого итератора:" << endl;
            cout << "\t1: Неустановленный." << endl;
            cout << "\t2: Установленный на элемент с определённым ключом." << endl;
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
                iter = Tree23<int, int>::Iterator(tree);
                
                break;
            }

            case 2:
            {
                cout << "Введите ключ:" << endl;
                int key = get_val();

                iter = Tree23<int, int>::Iterator(tree, key);

                if (iter == tree.end())
                    cout << "Итератор перешёл в состояние \"неустановлен\" из-за отсутствия элемента с данным ключом в дереве." << endl;
                else
                    cout << "Итератор установлен на элемент с заданным ключом." << endl;
                
                break;
            }
            }
            cout << "Итератор успешно создан." << endl << endl;

            bool isExit = false;
            while (!isExit)
            {
                cout << "Меню \"Прямой итератор\":" << endl;
                cout << "\t1: Установить итератор по заданному ключу." << endl;
                cout << "\t2: Получить элемент итератора." << endl;
                cout << "\t3: Инкрементировать итератор." << endl;
                cout << "\t4: Декрементировать итератор." << endl;
                cout << "\t5: Выйти из данного раздела." << endl;

                cout << "Выберите пункт меню:" << endl;
                choose = get_val();
                while (choose < 1 || choose > 5)
                {
                    cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                    choose = get_val();
                }

                cout << endl;

                switch (choose)
                {
                case 1:
                {
                    cout << "Введите ключ:" << endl;
                    int key = get_val();

                    iter = Tree23<int, int>::Iterator(tree, key);
                    
                    if (iter == tree.end())
                        cout << "Итератор перешёл в состояние \"неустановлен\" из-за отсутствия элемента с данным ключом в дереве." << endl;
                    else
                        cout << "Итератор установлен на элемент с заданным ключом." << endl;

                    break;
                }

                case 2:
                {
                    try
                    {
                        cout << "Ключ элемента дерева, на который в данный момент установлен итератор: " << *iter << "." << endl;
                    }
                    catch (Tree23_exception& exception)
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

                        if (iter == tree.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\"." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (Tree23_exception& exception)
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

                        if (iter == tree.end())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\"." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (Tree23_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 5:
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

        case 12:
        {
            Tree23<int, int>::RIterator riter;

            cout << "Раздел \"Обратный итератор\":" << endl << endl;

            cout << "Выберите тип создаваемого итератора:" << endl;
            cout << "\t1: Неустановленный." << endl;
            cout << "\t2: Установленный на элемент с определённым ключом." << endl;
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
                riter = Tree23<int, int>::RIterator(tree);
                
                break;
            }

            case 2:
            {
                cout << "Введите ключ:" << endl;
                int key = get_val();

                riter = Tree23<int, int>::RIterator(tree, key);
                
                if (riter == tree.rend())
                    cout << "Итератор перешёл в состояние \"неустановлен\" из-за отсутствия элемента с данным ключом в дереве." << endl;
                else
                    cout << "Итератор установлен на элемент с заданным ключом." << endl;

                break;
            }
            }
            cout << "Итератор успешно создан." << endl << endl;

            bool isExit = false;
            while (!isExit)
            {
                cout << "Меню \"Обратный итератор\":" << endl;
                cout << "\t1: Установить итератор по заданному ключу." << endl;
                cout << "\t2: Получить элемент итератора." << endl;
                cout << "\t3: Инкрементировать итератор." << endl;
                cout << "\t4: Декрементировать итератор." << endl;
                cout << "\t5: Выйти из данного раздела." << endl;

                cout << "Выберите пункт меню:" << endl;
                choose = get_val();
                while (choose < 1 || choose > 5)
                {
                    cout << "Ошибка: данного пункта меню не существует. Повторите ввод." << endl;
                    choose = get_val();
                }

                cout << endl;

                switch (choose)
                {
                case 1:
                {
                    cout << "Введите ключ:" << endl;
                    int key = get_val();

                    riter = Tree23<int, int>::RIterator(tree, key);

                    if (riter == tree.rend())
                        cout << "Итератор перешёл в состояние \"неустановлен\" из-за отсутствия элемента с данным ключом в дереве." << endl;
                    else
                        cout << "Итератор установлен на элемент с заданным ключом." << endl;

                    break;
                }

                case 2:
                {
                    try
                    {
                        cout << "Ключ элемента дерева, на который в данный момент установлен итератор: " << *riter << "." << endl;
                    }
                    catch (Tree23_exception& exception)
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
                        
                        if (riter == tree.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\"." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (Tree23_exception& exception)
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
                        
                        if (riter == tree.rend())
                            cout << "Внимание: итератор перешёл в состояние \"неустановлен\"." << endl;
                        else
                            cout << "Итератор успешно изменён." << endl;
                    }
                    catch (Tree23_exception& exception)
                    {
                        cout << "Ошибка: " << exception.what() << "." << endl;
                    }

                    break;
                }

                case 5:
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

        case 13:
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
