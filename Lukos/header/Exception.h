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

class Exception
{
protected:
    QString message;
public:
    explicit Exception(QString _message) : message(_message) {}
    QString getString() const {return message;}
    std::string getStdString() const {return message.toStdString();}
};

#endif // EXCEPTION_H
