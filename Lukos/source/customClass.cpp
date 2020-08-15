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

#include "../header/customClass.h"
#include <iostream>

QCustomFrame::QCustomFrame(QWidget* parent)
    : QFrame(parent)
{}

void QCustomFrame::setBudgetCategory(BudgetCategory * _category)
{
    category = _category;
}

void QCustomFrame::setMaxPoints(unsigned const _max_points)
{
    max_points = _max_points ? _max_points : max_points;
}


void QCustomFrame::focusOn(int _point)
{
    if(_point < 0) return;
    unsigned __point = static_cast<unsigned>(_point);
    if(__point >= category->getSize()) return;
    point = __point;
    this->update();
}

void QCustomFrame::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    try
    {

    if(category != nullptr)
    {
        QPainter __paint(this);

        int __height_graduation_abs = 30,
            __width_graduation_ord = margin_x+50;

       // Calculs des dimensions

        unsigned __i = 0, __i_memo = 0;
        int __x_previous = 0,
            __y_previous = 0,
            __y_previous_forecast = 0;
        double __path = (width() - margin_x - __width_graduation_ord)/(max_points+1.);

        if(category->getSize() < max_points || point <= max_points/2) __i_memo = __i = 0;
        else if(point >= category->getSize() - max_points/2) __i_memo = __i = category->getSize() - max_points;
        else __i_memo = __i = point - max_points/2;

        double __max = (*category)[__i]->getBalance(), __min = __max;

        for(; __i < category->getSize(); __i++)
        {
            if(__max < (*category)[__i]->getBalance())
                __max = (*category)[__i]->getBalance();
            else if(__min > (*category)[__i]->getBalance())
                __min = (*category)[__i]->getBalance();

            if(__max < (*category)[__i]->getForecastBalance())
                __max = (*category)[__i]->getForecastBalance();
            else if(__min > (*category)[__i]->getForecastBalance())
                __min = (*category)[__i]->getForecastBalance();
        }

        __i = __i_memo;

        // Axe des abscisses (ligne temporelle)

        __paint.setPen(QPen(QBrush(QColor("silver")),3));
        __paint.drawLine(__width_graduation_ord,height()-margin_y-__height_graduation_abs,width()-2*__path-10,height()-margin_y-__height_graduation_abs);

        // Axe des ordonnées (ligne budgetaire)

        __paint.setPen(QPen(QBrush(QColor("silver")),3));
        __paint.drawLine(__width_graduation_ord,margin_y,__width_graduation_ord,height()-margin_y - __height_graduation_abs);


        // Légende ordonnée

        unsigned __j = 0, __j_max = 10.;
        for(; __j<=__j_max;__j++)
        {
            double __y_ord = margin_y + (((height()-2*margin_y - __height_graduation_abs)/__j_max) * __j);

            __paint.setPen(QPen(QBrush(QColor("black")),3));
            __paint.drawText(5,__y_ord-7,__width_graduation_ord-10,14,
                             Qt::TextFlag::TextSingleLine | Qt::AlignmentFlag::AlignRight,
                             QString::number(__max - (__max-__min)/__j_max * __j,'f',2));

            __paint.setPen(QPen(QBrush(QColor("silver")),1,Qt::PenStyle::DashLine));
            __paint.drawLine(__width_graduation_ord,__y_ord,width()-2*__path-10,__y_ord);
        }

        // Légende tracé

        __paint.setPen(QPen(QBrush(QColor("black")),1));
        __paint.drawRect(width()-2*__path,0,width(),41);

        __paint.setPen(QPen(QBrush(QColor(category->getColor())),1,Qt::PenStyle::DashLine));
        __paint.drawLine(width()-2*__path + 5,12,width() - 1.5*__path - 5,12);
        __paint.setPen(QPen(QBrush(QColor(category->getColor())),2));
        __paint.drawLine(width()-2*__path + 5,29,width() - 1.5*__path - 5,29);

        __paint.setPen(QPen(QBrush(QColor("black")),3));
        __paint.drawText(width() - 1.5*__path,5,1.5*__path - 5,14,Qt::TextFlag::TextSingleLine,"Budget prévu");
        __paint.drawText(width() - 1.5*__path,22,1.5*__path - 5,14,Qt::TextFlag::TextSingleLine,"Solde réel");

        // Contenu

        for(; __i < category->getSize() && __i < __i_memo + max_points; __i++)
        {
            int __x = __width_graduation_ord + __path * (__i - __i_memo);
            int __y = margin_y, __y_forecast = margin_y;
            if(__max == __min)
            {
                __y += (height() - 2 * margin_y - __height_graduation_abs)*(1. - (*category)[__i]->getBalance()/(*category)[__i]->getForecastBalance());
            }
            else
            {
                __y += (height() - 2 * margin_y - __height_graduation_abs)*(1. - (*category)[__i]->getBalance()/(__max - __min));
                __y_forecast += (height() - 2 * margin_y - __height_graduation_abs)*(1. - (*category)[__i]->getForecastBalance()/(__max - __min));
            }

            //légende de l'axe

            __paint.setPen(QPen(QBrush(QColor("black")),3));
            __paint.translate(__x-13,height()-5);
            __paint.rotate(-90);
            __paint.drawText(0,0,__height_graduation_abs,28,Qt::TextFlag::TextWrapAnywhere,category->getDate(__i).toString("dd/mm/yyyy"));
            __paint.rotate(90);
            __paint.translate(13-__x,5-height());

            // Point

            int __epais = 3;
            if(__i == point) __epais = 7;

            __paint.setPen(QPen(QBrush(QColor(category->getColor())),__epais));
            __paint.drawPoint(__x,__y_forecast);
            __paint.setPen(QPen(QBrush(QColor(category->getColor())),__epais+2));
            __paint.drawPoint(__x,__y);

            if(__i>__i_memo){

                // Ligne avec le point précédent

                __paint.setPen(QPen(QBrush(QColor(category->getColor())),1,Qt::PenStyle::DashLine));
                __paint.drawLine(__x_previous,__y_previous_forecast,__x,__y_forecast);
                __paint.setPen(QPen(QBrush(QColor(category->getColor())),2));
                __paint.drawLine(__x_previous,__y_previous,__x,__y);
            }

            __x_previous = __x;
            __y_previous = __y;
            __y_previous_forecast = __y_forecast;
        }

        __paint.end();
    }

    }
    catch(Exception& e)
    {
        std::cerr << e.getStdString() << std::endl;
    }
}
