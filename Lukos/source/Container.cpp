/**
    @copyright Lukos Budgeting Application copyright © 2020 Jérémy Godde

    @par License :
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../header/Calendar.h"

template <typename T>
Container<T>::Container()
    : array_elements(nullptr),
      size(0)
{
}

template <typename T>
typename Container<T>::
    Iterator Container<T>::begin()
    {
        return Iterator(*this,0);
    }

template <typename T>
unsigned Container<T>::getSize() const
{
    return size;
}

template <typename T>
void Container<T>::sort()
{
    unsigned __i = 1;
    for(;__i < size; __i++)
    {
        while(__i > 0 && array_elements[__i]->operator<(array_elements[__i-1]))
        {
            T element = array_elements[__i];
            array_elements[__i] = array_elements[__i-1];
            array_elements[__i-1] = element;

            __i--;
        }
    }
}

template <typename T>
void Container<T>::remove(unsigned _position)
{
    if(size > 0 && _position < size)
    {
        T* __array_elements = new T[size-1];

        unsigned __i = 0;

        for(;__i < _position;__i++)
        {
            __array_elements[__i] = array_elements[__i];
        }

        delete array_elements[__i];

        for(;__i < size-1;__i++)
        {
            __array_elements[__i] = array_elements[__i+1];
        }

        delete[] array_elements;
        size-=1;
        array_elements = __array_elements;
    }
}

template <typename T>
void Container<T>::remove(T _element)
{
    T* __array_elements = new T[size-1];

    unsigned __i = 0;

    for(;__i < size-1 && array_elements[__i] != _element;__i++)
    {
        __array_elements[__i] = array_elements[__i];
    }

    if(array_elements[__i] == _element)
    {
        delete array_elements[__i];

        for(;__i < size-1;__i++)
        {
            __array_elements[__i] = array_elements[__i+1];
        }

        delete[] array_elements;
        size-=1;
        array_elements = __array_elements;
    }
}

template <typename T>
void Container<T>::insert(T _element)
{
    T* __array_elements = new T[size+1];

    unsigned __i = 0;

    for(;__i < size;__i++)
    {
        __array_elements[__i] = array_elements[__i];
    }
    __array_elements[__i] = _element;

    delete[] array_elements;
    size += 1;
    array_elements = __array_elements;

    sort();
}

template <typename T>
T Container<T>::operator[](unsigned _position)
{
    if(_position >= size)
        throw Exception("Débordement d'éléments de container");
    return array_elements[_position];
}

template <typename T>
T Container<T>::operator[](unsigned _position) const
{
    if(_position >= size)
        throw Exception("Débordement d'éléments de container");
    return array_elements[_position];
}


template <typename T>
Container<T>::Iterator::Iterator(Container<T>& _container, unsigned _position)
    : position(_position),
      container(_container),
      termine(false)
{
}

template <typename T>
bool Container<T>::Iterator::isTerminated() const
{
    return container.getSize() == 0 || termine;
}

template <typename T>
typename Container<T>::
    Iterator& Container<T>::Iterator::operator++(int)
    {
        if(position <= container.getSize() - 1)
        {
            position ++;
        }

        termine = (position >= container.getSize());

        return *this;
    }

template <typename T>
T Container<T>::Iterator::operator*()
{
    if(termine)
        return nullptr;
    return container[position];
}

template Container<Operation*>::Container();
template Container<Operation*>::Iterator Container<Operation*>::begin();
template unsigned Container<Operation*>::getSize() const;
template void Container<Operation*>::sort();
template void Container<Operation*>::remove(unsigned _position);
template void Container<Operation*>::remove(Operation* _element);
template void Container<Operation*>::insert(Operation* _element);
template Operation* Container<Operation*>::operator[](unsigned _position);
template Operation* Container<Operation*>::operator[](unsigned _position) const;
template Container<Operation*>::Iterator::Iterator(Container<Operation*>& _container, unsigned _position);
template bool Container<Operation*>::Iterator::isTerminated() const;
template Container<Operation*>::Iterator& Container<Operation*>::Iterator::operator++(int);
template Operation* Container<Operation*>::Iterator::operator*();

template Container<Budget*>::Container();
template Container<Budget*>::Iterator Container<Budget*>::begin();
template unsigned Container<Budget*>::getSize() const;
template void Container<Budget*>::sort();
template void Container<Budget*>::remove(unsigned _position);
template void Container<Budget*>::remove(Budget* _element);
template void Container<Budget*>::insert(Budget* _element);
template Budget* Container<Budget*>::operator[](unsigned _position);
template Budget* Container<Budget*>::operator[](unsigned _position) const;
template Container<Budget*>::Iterator::Iterator(Container<Budget*>& _container, unsigned _position);
template bool Container<Budget*>::Iterator::isTerminated() const;
template Container<Budget*>::Iterator& Container<Budget*>::Iterator::operator++(int);
template Budget* Container<Budget*>::Iterator::operator*();

template Container<BudgetCategory*>::Container();
template Container<BudgetCategory*>::Iterator Container<BudgetCategory*>::begin();
template unsigned Container<BudgetCategory*>::getSize() const;
template void Container<BudgetCategory*>::sort();
template void Container<BudgetCategory*>::remove(unsigned _position);
template void Container<BudgetCategory*>::remove(BudgetCategory* _element);
template void Container<BudgetCategory*>::insert(BudgetCategory* _element);
template BudgetCategory* Container<BudgetCategory*>::operator[](unsigned _position);
template BudgetCategory* Container<BudgetCategory*>::operator[](unsigned _position) const;
template Container<BudgetCategory*>::Iterator::Iterator(Container<BudgetCategory*>& _container, unsigned _position);
template bool Container<BudgetCategory*>::Iterator::isTerminated() const;
template Container<BudgetCategory*>::Iterator& Container<BudgetCategory*>::Iterator::operator++(int);
template BudgetCategory* Container<BudgetCategory*>::Iterator::operator*();
