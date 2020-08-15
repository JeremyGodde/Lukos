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

#ifndef CUSTOMCLASS_H
#define CUSTOMCLASS_H

#include "Calendar.h"
#include <QtWidgets>
#include <QPainter>

class QCustomFrame : public QFrame
{
    Q_OBJECT
public:
    QCustomFrame(QWidget* parent);
    ~QCustomFrame() = default;

    void setBudgetCategory(BudgetCategory * _category);
    void setMaxPoints(unsigned const _max_points);

public slots:
    void focusOn(int _point);

protected:
    virtual void paintEvent(QPaintEvent *) override;
    BudgetCategory *category = nullptr;

    int margin_x = 20;
    int margin_y = 20;
    unsigned point = 0;
    unsigned max_points = 6;
};

#endif // CUSTOMCLASS_H
