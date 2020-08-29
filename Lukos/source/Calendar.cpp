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

#include "header/Calendar.h"

Calendar* Calendar::calendar = new Calendar();

//======================================

QString getAttributeFromXML(QXmlStreamReader* _xml,QString const _attr)
{
    if(!_xml->attributes().hasAttribute(_attr))
        throw Exception("fichier corrompu : l'attribut "+_attr+" était attendu, mais n'a pas été rencontré");

    auto __list = _xml->attributes();
    auto __it_attr = __list.begin();
    for(;__it_attr != __list.end(); __it_attr++)
    {
        if((*__it_attr).qualifiedName() == _attr)
        {
            return (*__it_attr).value().toString();
        }
    }

    throw Exception("fichier corrompu : l'attribut "+_attr+" était attendu, mais n'a pas été rencontré");
}

//======================================

QString const operator<<(QString const& _s1, QString const& _s2)
{
    return _s1 + _s2;
}

std::ostream& operator<<(std::ostream& _f, QString const& _s)
{
    _f << _s.toStdString();
    return _f;
}

std::ostream& operator<<(std::ostream& _f, Date const& _d)
{
    _f << ("" << _d);
    return _f;
}


Operation::Operation()
    : subject("default Operation"),
      reference("default Reference"),
      value(0.),
      date(nullptr)
{
}

Operation::Operation(QString const _subject,
                     QString const _reference,
                     Date* const _date,
                     double _value,
                     QString const _description)
    : subject(_subject),
      reference(_reference),
      value(_value),
      date(_date),
      description(_description)
{
}

QString const Operation::getSubject() const
{
    return subject;
}

QString const Operation::getReference() const
{
    return reference;
}

double Operation::getValue() const
{
    return value;
}

Date const& Operation::getDate() const
{
    return *date;
}

QString const Operation::getDescription() const
{
    return description;
}

void Operation::setEdit(QString const _subject,
                        QString const _reference,
                        Date* const _date,
                        double _value,
                        QString const _description)
{
    subject = _subject;
    reference = _reference;
    date = _date;
    value = _value;
    description = _description;
}

void Operation::load(QXmlStreamReader* _xml)
{
    if(_xml->qualifiedName() != "Operation")
        throw Exception("fichier corrompu : Operation était attendu, mais '"+_xml->qualifiedName()+"' a été rencontré");

    subject = getAttributeFromXML(_xml,"subject");
    reference = getAttributeFromXML(_xml,"reference");
    value = getAttributeFromXML(_xml,"value").toDouble();
    date = Date::fromString(getAttributeFromXML(_xml,"date"));
    description = getAttributeFromXML(_xml,"description");

    _xml->readNextStartElement(); // lis la fin de opération
    _xml->readNextStartElement(); // lis la balise suivante
}

void Operation::save(QXmlStreamWriter* _xml) const
{
    _xml->writeStartElement("Operation");

    _xml->writeAttribute("subject",subject);
    _xml->writeAttribute("reference",reference);
    _xml->writeAttribute("value", QString::number(value,'f',2));
    _xml->writeAttribute("date", date->toString());
    _xml->writeAttribute("description",description);

    _xml->writeEndElement();
}

bool Operation::operator<(Operation const& _operation) const
{
    return date->operator<(_operation.date);
}

bool Operation::operator<(Operation const* _operation) const
{
    return date->operator<(_operation->date);
}


Budget::Budget()
    : Container<Operation *>(),
      balance(0.),
      forecast_balance(0.),
      date_fin(nullptr)
{
}

Budget::Budget(double _forecast_balance, Date* const _date)
    : Container<Operation *>(),
      balance(0.),
      forecast_balance(_forecast_balance),
      date_fin(_date)
{
}

double Budget::getBalance() const
{
    return balance;
}

double Budget::getForecastBalance() const
{
    return forecast_balance;
}

double Budget::getDiffBtwnBalances() const
{
    return forecast_balance - balance;
}

void Budget::setEdit(double _forecast_balance)
{
    forecast_balance = _forecast_balance;
}

void Budget::addOperation(QString const _subject,
                          QString const _reference,
                          Date* const _date,
                          double _value,
                          QString const _description)
{
    insert(new Operation(_subject,
                         _reference,
                         _date,
                         _value,
                         _description)
           );

    balance += _value;
}

bool Budget::operator<(Budget const& _budget) const
{
    return date_fin->operator<(_budget.date_fin);
}

bool Budget::operator<(Budget const* _budget) const
{
    return date_fin->operator<(_budget->date_fin);
}

void Budget::load(QXmlStreamReader* _xml)
{
    if(_xml->qualifiedName() != "Budget")
        throw Exception("fichier corrompu : Budget était attendu, mais '"+_xml->qualifiedName()+"' a été rencontré");

    balance = getAttributeFromXML(_xml,"balance").toDouble();
    forecast_balance = getAttributeFromXML(_xml,"forecast_balance").toDouble();
    date_fin = Date::fromString(getAttributeFromXML(_xml,"date"));

    _xml->readNextStartElement();
    while(!_xml->isEndElement())
    {
        Operation* __o = new Operation();
        __o->load(_xml);
        insert(__o);
    }

    _xml->readNextStartElement();
}

void Budget::save(QXmlStreamWriter* _xml) const
{
    _xml->writeStartElement("Budget");

    _xml->writeAttribute("balance", QString::number(balance,'f',2));
    _xml->writeAttribute("forecast_balance", QString::number(forecast_balance,'f',2));
    _xml->writeAttribute("date", date_fin->toString());

    unsigned __i = 0;
    for(;__i < size; __i++)
    {
        array_elements[__i]->save(_xml);
    }

    _xml->writeEndElement();
}

void Budget::show()
{
    if(size == 0) return;

    std::cout << "\tBudget au " << *date_fin
              << " : " << QString::number(balance,'f',2) << " Eur. sur "
              << QString::number(forecast_balance,'f',2) << " Eur. prevu";

    unsigned __i = 0;
    for(;__i<size; __i++)
    {
        std::cout << "\n\t\t* " << array_elements[__i]->getDate() << " : "
                  << array_elements[__i]->getSubject() << " (" << array_elements[__i]->getReference() << ") = "
                  << QString::number(array_elements[__i]->getValue(),'f',2) << "\n\t\t    -> "
                  << array_elements[__i]->getDescription();
    }

    std::cout << std::endl;
}

BudgetCategory::BudgetCategory()
    : Event("default Subject",
            nullptr,
            nullptr,
            0),
      Container<Budget *>()
{
}

BudgetCategory::BudgetCategory(QString const _subject,
                               Date* const _date,
                               Frequency* const _frequency,
                               unsigned _occurence,
                               Type _type)
    : Event(_subject,
            _date,
            _frequency,
            _occurence),
      Container<Budget *>(),
      type(_type)
{
    unsigned __i = 0;
    for(;__i<occurence;__i++)
    {
        Date __d = *date_start + *frequency * __i;
        insert(new Budget(0,new Date(__d)));
    }
}

void BudgetCategory::draw()
{
    std::cout << "function Draw will be implemented soon" << std::endl;
}

void BudgetCategory::setSubject(QString const _subject)
{
    subject = _subject;
}

void BudgetCategory::setDate(Date* const _date)
{
    std::cout << "setDate have not been implemented yet\n";
    delete _date;
}

void BudgetCategory::setFrequency(Frequency* const _frequency)
{
    std::cout << "setFrequency have not been implemented yet\n";
    delete _frequency;
}

void BudgetCategory::setOccurence(unsigned _occurence)
{
    if(occurence < _occurence)
    {

        for(;occurence<_occurence;occurence++)
        {
            insert(new Budget(0,new Date(*date_start + *frequency * occurence)));
        }
    }
    else if(occurence > _occurence)
    {
        for(;occurence>_occurence;occurence--)
        {
            while(array_elements[occurence-1]->getSize() > 0)
            {
                Operation* __o = (*(array_elements[occurence-1]))[0];
                array_elements[occurence-1]->remove(__o);
            }
            remove(occurence-1);
        }
    }
}

void BudgetCategory::setType(BudgetCategory::Type _type)
{
    if(type != _type && _type != undefined)
    {
        type = _type;
        unsigned __i = 0;
        for(;__i < size; __i++)
        {
            if(type == credit)
                Calendar::getInstance().addBalance(2* array_elements[__i]->getBalance());
            else if(type == debit)
                Calendar::getInstance().addBalance(-2* array_elements[__i]->getBalance());
        }
    }
}


void BudgetCategory::setColor(QRgb _color)
{
    color = _color;
}

QRgb BudgetCategory::getColor() const
{
    return color;
}

BudgetCategory::Type BudgetCategory::getType() const
{
    return type;
}

void BudgetCategory::addOperation(QString const _subject,
                                  QString const _reference,
                                  Date* const _date,
                                  double _value,
                                  QString const _description)
{
    if(type == undefined)
        throw Exception("Tentative d'ajout d'operation a un budget de categorie non-definie");

    unsigned __i = 0;
    for(;__i < size && *(array_elements[__i]->date_fin) < _date; __i++)
    {

    }

    if(__i < size && !(__i == 0 && *_date + *frequency < *(array_elements[__i]->date_fin) ))
    {
        array_elements[__i]->addOperation(_subject,
                                          _reference,
                                          _date,
                                          _value,
                                          _description);
        if(type == credit)
            Calendar::getInstance().addBalance(_value);
        else if(type == debit)
            Calendar::getInstance().addBalance(-1*_value);
        else
            throw Exception("Erreur impossible");
    }
    else
    {
        delete _date;
    }
}

void BudgetCategory::delOperation(Operation* _element)
{
    unsigned __i = 0;
    for(;__i < size && *(array_elements[__i]->date_fin) < _element->getDate(); __i++)
    {

    }

    if(__i < size)
    {
        array_elements[__i]->balance -= _element->getValue();
        array_elements[__i]->remove(_element);
    }
}

double BudgetCategory::getSumBalances()
{
    double __sum = 0.;
    unsigned __i = 0;
    for(; __i < size; __i++)
    {
        __sum += array_elements[__i]->balance;
    }

    if(type == debit)
        __sum *= -1;

    return __sum;
}

void BudgetCategory::load(QXmlStreamReader* _xml)
{
    if(_xml->qualifiedName() != "BudgetCategory")
        throw Exception("fichier corrompu : BudgetCategory était attendu, mais '"+_xml->qualifiedName()+"' a été rencontré");

    type = Type(getAttributeFromXML(_xml,"type").toInt());
    color = QRgb(getAttributeFromXML(_xml,"color").toUInt());
    subject = getAttributeFromXML(_xml,"subject");
    date_start = Date::fromString(getAttributeFromXML(_xml,"date"));
    frequency = Frequency::fromString(getAttributeFromXML(_xml,"frequency"));
    occurence = getAttributeFromXML(_xml,"occurence").toInt();

    _xml->readNextStartElement();
    while(!_xml->isEndElement())
    {
        Budget* __b = new Budget();
        __b->load(_xml);
        insert(__b);
    }

    _xml->readNextStartElement();
}

void BudgetCategory::save()
{
    //...
}

void BudgetCategory::save(QXmlStreamWriter* _xml) const
{
    _xml->writeStartElement("BudgetCategory");

    _xml->writeAttribute("type", QString::number(type));
    _xml->writeAttribute("color", QString::number(color));
    _xml->writeAttribute("subject", subject);
    _xml->writeAttribute("date", date_start->toString());
    _xml->writeAttribute("frequency", frequency->toString());
    _xml->writeAttribute("occurence", QString::number(occurence));

    unsigned __i = 0;
    for(;__i < size; __i++)
    {
        array_elements[__i]->save(_xml);
    }

    _xml->writeEndElement();
}

void BudgetCategory::show()
{
    if(type == undefined) return;

    std::cout << "+ " << subject << " {" << ((type == credit) ? "Crt" : "Dbt") << "}\t couleur : " << color << std::endl;

    unsigned __i = 0;
    for(;__i<size;__i++)
    {
        array_elements[__i]->show();
    }
}

Calendar::Calendar()
    : Container<BudgetCategory *>()
{
}

Calendar& Calendar::getInstance()
{
    return *calendar;
}

void Calendar::addBudgetCategory(QString const _subject,
                                 Date* const _date,
                                 Frequency* const _frequency,
                                 unsigned _occurence,
                                 BudgetCategory::Type _type,
                                 QRgb _color)
{
    BudgetCategory * __bc = new BudgetCategory(_subject,
                                  _date,
                                  _frequency,
                                  _occurence,
                                  _type);
    __bc->setColor(_color);
    insert(__bc);
}

double Calendar::getBalance() const
{
    return balance;
}

void Calendar::addBalance(double const _value)
{
    balance += _value;
}

void Calendar::remove(BudgetCategory* _element)
{
    balance -= _element->getSumBalances();
    Container::remove(_element);
}

void Calendar::load(QXmlStreamReader* _xml)
{
    if(_xml->qualifiedName() != "Calendar")
        throw Exception("fichier corrompu : Calendar était attendu, mais '"+_xml->qualifiedName()+"' a été rencontré");

    balance = getAttributeFromXML(_xml,"balance").toDouble();

    _xml->readNextStartElement();
    while(!_xml->isEndElement())
    {
        BudgetCategory* __bc = new BudgetCategory();
        __bc->load(_xml);
        insert(__bc);
    }
}

void Calendar::save(QXmlStreamWriter* _xml) const
{
    _xml->writeStartElement("Calendar");

    _xml->writeAttribute("balance", QString::number(balance,'f',2));

    unsigned __i = 0;
    for(;__i < size; __i++)
    {
        array_elements[__i]->save(_xml);
    }

    _xml->writeEndElement();
}

void Calendar::show()
{
    std::cout << "Compte : " << QString::number(balance,'f',2) << " Eur.\n";
    unsigned __i = 0;
    for(;__i<size;__i++)
    {
        array_elements[__i]->show();
    }
}

BudgetCategory* Calendar::operator>>(QString _subject)
{
    unsigned __i = 0;
    for(;__i<size;__i++)
    {
        if(array_elements[__i]->getSubject() == _subject)
            return array_elements[__i];
    }

    return nullptr;
}





















