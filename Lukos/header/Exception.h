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

#ifndef EXCEPTION_H
#define EXCEPTION_H

/**
  @file Exception.h
  @author Jérémy Godde
  @version 1
  @date july 2020
  */

#include <iostream>
#include <QString>

/**
 * @brief class Exception
 * Represente une exception
 */
class Exception
{
protected:
    QString message; //!< message explicatif de la cause de l'exception
public:
    explicit Exception(QString _message) : message(_message) {} //!< constructeur

    /**
     * @fn inline QString Exception::getString() const
     * @return le message comme un QString
     */
    inline QString getString() const {return message;}

    /**
     * @fn inline std::string Exception::getStdString() const
     * @return le message comme un std::string
     */
    inline std::string getStdString() const {return message.toStdString();}
};

#endif // EXCEPTION_H
