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

#include "../header/Event.h"

Frequency::Frequency(Rythm _rythm,
                     unsigned _frequency)
    : frequency(_frequency),
      rythm(_rythm)
{

}

Frequency::Rythm Frequency::getRythm() const
{
    return rythm;
}

unsigned Frequency::getFrequency() const
{
    return frequency;
}

unsigned Frequency::getNbWeek() const
{
    return (rythm == WEEKLY) ? frequency : 0;
}

unsigned Frequency::getNbMonth() const
{
    return (rythm == MONTHLY) ? frequency : 0;
}

unsigned Frequency::getNbYear() const
{
    return (rythm == YEARLY) ? frequency : 0;
}

QString const Frequency::toString() const
{
    return QString::number(getNbWeek()) + " " + QString::number(getNbMonth()) + " " + QString::number(getNbYear());
}

Frequency* Frequency::fromString(QString _text)
{
    unsigned __i = 0;
    int __j = 0;
    unsigned __f = 0;
    Rythm __r = YEARLY;
    for(;__i < 3 && __j<_text.size() && __f == 0; __i++)
    {
        QString __s = "";
        for(;__j<_text.size() && _text[__j]!=' ';__j++)
        {
            __s += _text[__j];
        }
        __j++;
        __f = __s.toInt();

        if(__f > 0){
            if(__i == 0){__r = WEEKLY;}
            else if(__i == 1){__r = MONTHLY;}
        }
    }

    return new Frequency(__r,__f);
}

Frequency Frequency::operator*(unsigned const& _factor) const
{
    return Frequency(rythm,_factor * frequency);
}

QString const operator<<(QString const& _s, Frequency const& _f)
{
    return _s + QString::number(_f.getNbWeek()) + " days, " + QString::number(_f.getNbMonth()) + " months, " + QString::number(_f.getNbYear()) + " years";
}

Date::Date(Day _day,
           Month _month,
           Year _year)
    : day(_day),
      month(_month),
      year(_year)
{

}

unsigned Date::getDay() const
{
    return day;
}

unsigned Date::getMonth() const
{
    return month;
}

unsigned Date::getYear() const
{
    return year;
}

Date const Date::getToday()
{
    QDate __d = QDate::currentDate();
    return Date(Day(__d.day()),Month(__d.month()),Year(__d.year()));
}

bool Date::operator<(Date const& _date) const
{
    unsigned __date_1 = year*10000 + month*100 + day;
    unsigned __date_2 = _date.year*10000 + _date.month*100 + _date.day;

    return __date_1 < __date_2;
}

bool Date::operator<(Date const* _date) const
{
    unsigned __date_1 = year*10000 + month*100 + day;
    unsigned __date_2 = _date->year*10000 + _date->month*100 + _date->day;

    return __date_1 < __date_2;
}

Date Date::operator+(Frequency const& _frequency) const
{
    if(_frequency.getRythm() == Frequency::YEARLY)
    {
        unsigned __year = year + _frequency.getFrequency();
        return Date(day,month,__year);
    }

    else if(_frequency.getRythm() == Frequency::MONTHLY)
    {
        unsigned __month = month + _frequency.getFrequency(),
                 __year = year,
                __day = day;

        while(__month > 12)
        {
            __year ++;
            __month -= 12;
        }

        unsigned __lim = 31;
        if(__month == february && ((__year%100 != 0 && __year%4 == 0) || (__year%400 == 0))){__lim = 29;}
        else if(__month == february){__lim = 28;}
        else if(__month == april){__lim = 30;}
        else if(__month == june){__lim = 30;}
        else if(__month == september){__lim = 30;}
        else if(__month == november){__lim = 30;}

        if(__day > __lim)
        {
            __day = __lim;
        }

        return Date(Day(__day),Month(__month),__year);
    }

    else if(_frequency.getRythm() == Frequency::WEEKLY)
    {
        unsigned __month = month,
                 __year = year,
                __day = day + 7 * _frequency.getFrequency();

        unsigned __lim;
        do
        {
            __lim = 31;
            if(__month == february && ((__year%100 != 0 && __year%4 == 0) || (__year%400 == 0))){__lim = 29;}
            else if(__month == february){__lim = 28;}
            else if(__month == april){__lim = 30;}
            else if(__month == june){__lim = 30;}
            else if(__month == september){__lim = 30;}
            else if(__month == november){__lim = 30;}

            if(__day > __lim)
            {
                __day -= __lim;
                __month ++;
            }

            while(__month > 12)
            {
                __year ++;
                __month -= 12;
            }

        }while(__day > __lim);

        return Date(Day(__day),Month(__month),__year);
    }

    return *this;
}

Date Date::operator-(Frequency const& _frequency) const
{
    if(_frequency.getRythm() == Frequency::YEARLY)
    {
        int __year = year - _frequency.getFrequency();
        return Date(day,month,Year(__year));
    }

    else if(_frequency.getRythm() == Frequency::MONTHLY)
    {
        int __month = month - _frequency.getFrequency(),
            __year = year;
        unsigned __day = day;

        while(__month <= 0)
        {
            __year --;
            __month += 12;
        }

        unsigned __lim = 31;
        if(__month == february && ((__year%100 != 0 && __year%4 == 0) || (__year%400 == 0))){__lim = 29;}
        else if(__month == february){__lim = 28;}
        else if(__month == april){__lim = 30;}
        else if(__month == june){__lim = 30;}
        else if(__month == september){__lim = 30;}
        else if(__month == november){__lim = 30;}

        if(__day > __lim)
        {
            __day = __lim;
        }

        return Date(Day(__day),Month(__month),Year(__year));
    }

    else if(_frequency.getRythm() == Frequency::WEEKLY)
    {
        int __month = month,
            __year = year,
            __day = day - 7 * _frequency.getFrequency();

        unsigned __lim;
        do
        {
            if(0 >= __day)
            {
                __month --;

                while(__month <= 0)
                {
                    __year --;
                    __month += 12;
                }

                __lim = 31;
                if(__month == february && ((__year%100 != 0 && __year%4 == 0) || (__year%400 == 0))){__lim = 29;}
                else if(__month == february){__lim = 28;}
                else if(__month == april){__lim = 30;}
                else if(__month == june){__lim = 30;}
                else if(__month == september){__lim = 30;}
                else if(__month == november){__lim = 30;}

                __day += __lim;
            }

        }while(0 >= __day);

        return Date(Day(__day),Month(__month),Year(__year));
    }

    return *this;
}

QString const Date::toString(QString const _format) const
{
    QString __date = QString::number(day);
    QString __month = "";

    if(_format.contains("mmmm"))
    {
        switch (month) {
        case january:
          __month = "janvier";
          break;
        case february:
          __month = "février";
          break;
        case march:
          __month = "mars";
          break;
        case april:
          __month = "avril";
          break;
        case may:
          __month = "mai";
          break;
        case june:
          __month = "juin";
          break;
        case july:
          __month = "juillet";
          break;
        case august:
          __month = "août";
          break;
        case september:
          __month = "septembre";
          break;
        case october:
          __month = "octobre";
          break;
        case november:
          __month = "novembre";
          break;
        case december:
          __month = "décembre";
          break;
        }
    }
    else if(_format.contains("mmm"))
    {
        switch (month) {
        case january:
          __month = "jan.";
          break;
        case february:
          __month = "fév.";
          break;
        case march:
          __month = "mars";
          break;
        case april:
          __month = "avr.";
          break;
        case may:
          __month = "mai";
          break;
        case june:
          __month = "juin";
          break;
        case july:
          __month = "jul.";
          break;
        case august:
          __month = "août";
          break;
        case september:
          __month = "sep.";
          break;
        case october:
          __month = "oct.";
          break;
        case november:
          __month = "nov.";
          break;
        case december:
          __month = "déc.";
          break;
        }
    }
    else
    {
        __month = QString::number(month);
        if(month < 10) __month = "0" + __month;
        if(day < 10) __date = "0" + __date;

        if(_format == "dd/mm/yyyy") __date += "/" + __month + "/" + QString::number(year);
        else if(_format == "yyyy/mm/dd") __date = QString::number(year) + "/" + __month + "/" + __date;
        else if(_format == "mm/dd/yyyy") __date = __month + "/" + __date + "/" + QString::number(year);

        return __date;
    }

    __date += " " + __month + " " + QString::number(year);

    return __date;
}

Date* Date::fromString(QString _text)
{
    unsigned __i = 0;
    int __j = 0;
    Day __d;
    Month __m;
    Year __y;
    for(;__i < 3 && __j<_text.size(); __i++)
    {
        QString __s = "";
        for(;__j<_text.size() && _text[__j]!=' ';__j++)
        {
            __s += _text[__j];
        }
        __j++;

        if(__i == 0){__d = Day(__s.toInt());}
        else if(__i == 1){
            if(__s == "janvier"){__m = january;}
            else if(__s == "février"){__m = february;}
            else if(__s == "mars"){__m = march;}
            else if(__s == "avril"){__m = april;}
            else if(__s == "mai"){__m = may;}
            else if(__s == "juin"){__m = june;}
            else if(__s == "juillet"){__m = july;}
            else if(__s == "août"){__m = august;}
            else if(__s == "septembre"){__m = september;}
            else if(__s == "octobre"){__m = october;}
            else if(__s == "novembre"){__m = november;}
            else if(__s == "décembre"){__m = december;}
        }
        else if(__i == 2){__y = __s.toInt();}
    }

    return new Date(__d,__m,__y);
}

QDate const Date::toQDate() const
{
    QDate __qdate;
    __qdate.setDate(year,month,day);
    return __qdate;
}

QString const operator<<(QString const& _s, Date const& _d)
{
    return _s + QString::number(_d.getDay()) + "/" + QString::number(_d.getMonth()) + "/" + QString::number(_d.getYear());
}



Event::Event(QString const _subject,
             Date* const _date,
             Frequency* const _frequency,
             unsigned _occurence)
    : date_start(_date),
      frequency(_frequency),
      occurence(_occurence),
      subject(_subject)
{

}

Date const Event::getDate(unsigned _occurence) const
{
    if(occurence != 0 && _occurence > occurence)
    {
        throw Exception("Débordement d'occurences de date");
    }

    return *date_start + *frequency * _occurence;
}

Frequency const& Event::getFrequency() const
{
    return *frequency;
}

unsigned Event::getOccurence() const
{
    return occurence;
}

QString const Event::getSubject() const
{
    return subject;
}

bool Event::operator<(Event const* _event) const
{
    return *date_start < _event->date_start;
}

bool Event::operator<(Event const& _event) const
{
    return *date_start < _event.date_start;
}

bool Event::operator>(Event const* _event) const
{
    return _event->date_start->operator<(*date_start);
}

bool Event::operator>(Event const& _event) const
{
    return _event.date_start->operator<(*date_start);
}



