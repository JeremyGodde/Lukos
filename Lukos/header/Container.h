/* =======================================================================
    Lukos Budgeting Application
    Copyright (C) 2020 Jérémy Godde

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
======================================================================= */

#ifndef CONTAINER_H
#define CONTAINER_H

/**
  @file Container.h
  @author Jérémy Godde
  @version 1
  @date july 2020
  */

#include "Exception.h"

/**
 * @brief class Container<T>
 * Représente un contenant d'objets T
 * @remark nécessite pour T un constructeur sans argument
 */

//MODIFICATION DE constructeur Iterator
//AJOUT DE bool Iterator::termine

template <typename T>
class Container
{
public:
    class Iterator;

protected:
    T* array_elements; //!< tableau d'éléments
    unsigned size; //!< taille du tableau

public:
    explicit Container(); //!< constructeur
    ~Container() = default;

    /**
     * @fn Iterator Container::begin()
     * @return un Iterator positionné au début
     * @see Iterator
     */
    Iterator begin();

    /**
     * @fn unsigned Container::getSize()
     * @return la taille du tableau
     */
    unsigned getSize() const;

    /**
     * @fn void Container::sort()
     * @brief trie le tableau
     */
    void sort();

    /**
     * @fn virtual void Container::remove(unsigned _position)
     * @param _position unsigned : position de l'élément à supprimer
     * @brief supprime l'élément à la position indiquée
     */
    virtual void remove(unsigned _position);

    /**
     * @fn virtual void Container::remove(T _element)
     * @param _element T : élément à supprimer
     * @brief supprime l'élément indiqué
     */
    virtual void remove(T _element);

    /**
     * @fn void Container::insert(T _element)
     * @param _element T : élément à insérer
     * @brief insére l'élément dans le tableau
     */
    void insert(T _element);

    /**
     * @fn T Container::operator[](unsigned _position)
     * @param _position unsigned : position de l'élément à retourner
     * @return l'élément à la position indiquée
     */
    T operator[](unsigned _position);

    /**
     * @fn virtual void Container::show() = 0
     * @brief affiche les informations du conteneur et de ses éléments
     */
    virtual void show() = 0;

    /**
     * @brief class Iterator
     * Représente l'itérateur de la classe Container
     * @see Container
     */
    class Iterator
    {
    protected:
        unsigned position; //!< position actuelle de l'itérator
        Container<T>& container; //!< position actuelle de l'itérator
        bool termine; //!< marqueur indiquant si l'itérator est arrivé à la fin

    public:
        explicit Iterator(Container<T>& _container, unsigned _position); //!< constructeur

        /**
         * @fn bool typename Container::Iterator::isTerminated() const
         * @return vrai si l'itérateur a atteint une extrémité
         */
        bool isTerminated() const;

        /**
         * @fn Container::Iterator& Container::Iterator::operator++(int)
         * @brief incrémente l'itérateur avant de le retourner
         * @return retourne l'itérateur
         */
        Iterator& operator++(int);

        /**
         * @fn Container::Iterator::operator*()
         * @return l'élément sur lequel pointe l'itérateur
         */
        T operator*();

        Iterator& operator=(Iterator const& _iterator) = default; //!< operateur de recopie par défaut
    };
};

#endif // CONTAINER_H
