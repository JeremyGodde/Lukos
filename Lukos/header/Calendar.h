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


#ifndef CALENDAR_H
#define CALENDAR_H

/**
  @file Calendar.h
  @author Jérémy Godde
  @version 1
  @date july 2020
  */

#include <QRgb>
#include <QtXml>
#include <QTextCodec>
#include "Container.h"
#include "Event.h"

/**
 * @brief class Operation
 * Represente une opération
 * @see Budget
 */
class Operation
{
public:
    friend class Budget;

protected:
    QString subject; //!< intitulé de l'opération
    QString reference; //!< référence de l'opération
    double value; //!< montant de l'opération
    Date* date; //!< date de l'opération
    QString description; //!< description de l'opération

    explicit Operation(); //!< constructeur sans argument pour être contenu par Containeur
    explicit Operation(QString const _subject,
                       QString const _reference,
                       Date* const _date,
                       double _value,
                       QString const _description); //!< constructeur

public:
    /**
     * @fn QString const Operation::getSubject() const
     * @return l'intitulé de l'opération
     */
    QString const getSubject() const;

    /**
     * @fn QString const Operation::getReference() const
     * @return la référence de l'opération
     */
    QString const getReference() const;

    /**
     * @fn double Operation::getValue() const
     * @return le montant de l'opération
     */
    double getValue() const;

    /**
     * @fn Date const& Operation::getDate() const
     * @see Date
     * @return la date de l'opération
     */
    Date const& getDate() const;

    /**
     * @fn QString const Operation::getDescription() const
     * @return la description de l'opération
     */
    QString const getDescription() const;

                                                                     /*=================*
                                                                      *                 *
                                                                      *  NON PERFORMANT *
                                                                      *                 *
                                                                      *=================*/
    void setEdit(QString const _subject,
                 QString const _reference,
                 Date* const _date,
                 double _value,
                 QString const _description);

    /**
     * @fn void Operation::load(QXmlStreamReader* _xml)
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief chargement au travers du flux xml
     */
    void load(QXmlStreamReader* _xml);

    /**
     * @fn void Operation::save(QXmlStreamWriter* _xml) const
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief sauvegarde au travers du flux xml
     */
    void save(QXmlStreamWriter* _xml) const;

    /**
     * @fn bool Operation::operator<(Operation const& _operation) const
     * @param _operation Operation const& : opération avec laquelle comparer
     * @see Date
     * @return vrai si la date de l'opération est antérieure à la date de l'opération passée en argument
     */
    bool operator<(Operation const& _operation) const;

    /**
     * @fn bool Operation::operator<(Operation const& _operation) const
     * @param _operation Operation const& : opération avec laquelle comparer
     * @see Date
     * @return vrai si la date de l'opération est antérieure à la date de l'opération passée en argument
     */
    bool operator<(Operation const* _operation) const;

    ~Operation() = default;
};

Q_DECLARE_METATYPE(Operation*);

/**
 * @brief class Budget
 * Represente un budget
 * @see Container
 * @see Operation
 * @see BudgetCategory
 */
class Budget : public Container<Operation*>
{
public:
    friend class BudgetCategory;

protected:
    double balance; //!< solde réel du budget
    double forecast_balance; //!< solde prévisionnel du budget
    Date* date_fin; //!< date de fin du budget

    explicit Budget(); //!< constructeur sans argument pour être contenu par Containeur
    explicit Budget(double _forecast_balance,
                    Date* const _date); //!< constructeur

    /**
     * @fn void Budget::addOperation(QString const _subject, QString const _reference, Date* const _date, double _value, QString const _description)
     * @param _subject QString const : intitulé de l'opération
     * @param _reference QString const : référence de l'opération
     * @param _date Date* const : date de l'opération
     * @param _value double : montant de l'opération
     * @param _description QString const : description de l'opération
     * @see Operation
     * @see Date
     * @brief ajoute une opération au budget
     */
    void addOperation(QString const _subject,
                      QString const _reference,
                      Date* const _date,
                      double _value,
                      QString const _description);

public:
    /**
     * @fn double const Budget::getBalance() const
     * @return le solde réel du budget
     */
    double getBalance() const;

    /**
     * @fn double const Budget::getForecastBalance() const
     * @return le solde prévisionnel du budget
     */
    double getForecastBalance() const;

    /**
     * @fn double const Budget::getDiffBtwnBalances() const
     * @return la différence entre les soldes prévisionnel et réel du budget
     */
    double getDiffBtwnBalances() const;

    /**
     * @fn void const Budget::setEdit(double _forecast_balance)
     * @param _forecast_balance double : nouveau solde prévisionnel
     * @brief modifie le solde prévisionnel
     */
    void setEdit(double _forecast_balance);

    /**
     * @fn void Budget::load(QXmlStreamReader* _xml)
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief chargement au travers du flux xml
     * @see Operation
     */
    void load(QXmlStreamReader* _xml);

    /**
     * @fn void Budget::save(QXmlStreamWriter* _xml) const
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief sauvegarde au travers du flux xml
     * @see Operation
     */
    void save(QXmlStreamWriter* _xml) const;

    /**
     * @fn void const Budget::show() override
     * @see Container
     * @brief affiche les informations du budget et de ses opérations
     */
    void show() override;

    /**
     * @fn bool Budget::operator<(Budget const& _budget) const
     * @param _budget Budget const& : budget avec laquelle comparer
     * @see Date
     * @return vrai si la date du budget est antérieure à la date du budget passé en argument
     */
    bool operator<(Budget const& _budget) const;

    /**
     * @fn bool Budget::operator<(Budget const* _budget) const
     * @param _budget Budget const* : budget avec laquelle comparer
     * @see Date
     * @return vrai si la date du budget est antérieure à la date du budget passé en argument
     */
    bool operator<(Budget const* _budget) const;

    virtual ~Budget() = default; //!< Desctructeur par défaut
};

/**
 * @brief class BudgetCategory
 * Represente une catégorie budgétaire contenant plusieurs budgets
 * @see Event
 * @see Container
 * @see Budget
 * @see Calendar
 */
class BudgetCategory : public Event, public Container<Budget*>
{
public:
    friend class Calendar;

    /**
     * @enum Type
     * @brief ensemble des types de budget
     */
    enum Type
    {
        /**
         * @var undefined
         * @brief non-défini
         */
        undefined = 0,
        /**
         * @var credit
         * @brief crédit
         */
        credit = 1,
        /**
         * @var debit
         * @brief débit
         */
        debit = 2
    };

protected:
    QRgb color = qRgb(0,0,0); //!< couleur associée à la catégorie
    Type type = undefined;//!< type de la catégorie

    explicit BudgetCategory(); //!< constructeur sans argument pour être contenu par Containeur
    explicit BudgetCategory(QString const _subject,
                            Date* const _date,
                            Frequency* const _frequency,
                            unsigned _occurence,
                            Type _type); //!< constructeur

    /**
     * @fn void BudgetCategory::draw()
     * @brief dessine une représentation graphique de la catégorie de budget : montants en fonction des budgets
     */
    void draw();

    /**
     * @fn double getSumBalances()
     * @return la somme de tous les soldes de la catégorie
     */
    double getSumBalances();

public:
    /**
     * @fn BudgetCategory::Type BudgetCategory::getType() const
     * @return le type de la catégorie
     */
    Type getType() const;

    /**
     * @fn QRgb BudgetCategory::getColor() const
     * @return la couleur associée à la catégorie
     */
    QRgb getColor() const;

    /**
     * @fn void BudgetCategory::setSubject(QString const _subject)
     * @param _subject QString const : nouvel intitulé
     * @brief modifie l'intitulé
     */
    void setSubject(QString const _subject);

    void setDate(Date* const _date);                                                    //non implémentée
    void setFrequency(Frequency* const _frequency);                                     //non implémentée

    /**
     * @fn void BudgetCategory::setOccurence(unsigned _occurence)
     * @param _occurence unsigned : nouveau occurence
     * @brief modifie l'occurence
     */
    void setOccurence(unsigned _occurence);

    /**
     * @fn void BudgetCategory::setType(Type _type)
     * @param _type BudgetCategory::Type : nouveau type
     * @brief modifie le type
     */
    void setType(Type _type);

    /**
     * @fn void BudgetCategory::setColor(QRgb _color)
     * @param _color QRgb : nouvelle couleur
     * @brief modifie la couleur
     */
    void setColor(QRgb _color);

    /**
     * @fn void BudgetCategory::addOperation(QString const _subject, QString const _reference, Date* const _date, double _value, QString const _description)
     * @param _subject QString const : intitulé de l'opération
     * @param _reference QString const : référence de l'opération
     * @param _date Date* const : date de l'opération
     * @param _value double : montant de l'opération
     * @param _description QString const : description de l'opération
     * @see Operation
     * @see Date
     * @see Budget
     * @brief ajoute une opération au budget
     */
    void addOperation(QString const _subject,
                      QString const _reference,
                      Date* const _date,
                      double _value,
                      QString const _description);

    /**
     * @fn void BudgetCategory::delOperation(Operation* _element)
     * @param _element Operation* : opération à supprimer
     * @brief supprime l'opération de la catégorie ou ne fais rien si l'opération ne fait pas partie de la catégorie
     */
    void delOperation(Operation* _element);

    /**
     * @fn void BudgetCategory::load(QXmlStreamReader* _xml)
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief chargement au travers du flux xml
     * @see Budget
     */
    void load(QXmlStreamReader* _xml);

    /**
     * @fn void BudgetCategory::save() override
     * @see Event
     * @brief effectue une sauvegarde des données de la catégorie
     */
    void save() override;

    /**
     * @fn void BudgetCategory::save(QXmlStreamWriter* _xml) const
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief sauvegarde au travers du flux xml
     * @see Budget
     */
    void save(QXmlStreamWriter* _xml) const;

    /**
     * @fn void BudgetCategory::show() override
     * @see Container
     * @brief affiche les informations de la catégorie et de ses budgets
     */
    void show() override;

    virtual ~BudgetCategory() = default; //!< Destructeur par défaut
};

Q_DECLARE_METATYPE(BudgetCategory*);

/**
 * @brief class Calendar
 * Represente un calendrier budgetaire contenant plusieurs catégories budgétaires
 * @see Container
 * @see BudgetCategory
 */
class Calendar : public Container<BudgetCategory*>
{
protected:
    explicit Calendar(); //!< constructeur

    static Calendar* calendar; //!< instance unique du calendrier

    double balance = 0; //!< solde réel du calendrier

public:
    /**
     * @fn Calendar& Calendar::getInstance()
     * @return l'instance unique du calendrier
     */
    static Calendar& getInstance();

    /**
     * @fn void Calendar::addBudgetCategory(QString const _subject, Date* const _date, Frequency* const _frequency, unsigned _occurence, BudgetCategory::Type _type, QRgb _color)
     * @param _subject QString const : intitulé de la catégorie
     * @param _date Date* const : date de départ de la catégorie
     * @param _frequency Frequency* const : fréquence de la catégorie
     * @param _occurence unsigned : occurance de la catégorie
     * @param _type BudgetCategory::Type : type de la catégorie
     * @param _color QRgb : couleur de la catégorie
     * @see Date
     * @see Frequency
     * @see BudgetCategory
     * @brief ajoute une catégorie au calendrier
     */
    void addBudgetCategory(QString const _subject,
                           Date* const _date,
                           Frequency* const _frequency,
                           unsigned _occurence,
                           BudgetCategory::Type _type,
                           QRgb _color);

    /**
     * @fn double Calendar::getBalance() const
     * @return le solde réel du calendrier
     */
    double getBalance() const;

    /**
     * @fn double Calendar::addBalance(const double _value)
     * @brief modifie le solde réel du calendrier
     */
    void addBalance(const double _value);

    /**
     * @fn void Container::remove(BudgetCategory* _element) override
     * @param _element BudgetCategory* : catégorie à supprimer
     * @brief supprime la catégorie indiquée et recalcul le solde
     */
    void remove(BudgetCategory* _element) override;

    /**
     * @fn void Calendar::load(QXmlStreamReader* _xml)
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief chargement au travers du flux xml
     * @see BudgetCategory
     */
    void load(QXmlStreamReader* _xml);

    /**
     * @fn void Calendar::save(QXmlStreamWriter* _xml) const
     * @param _xml QXmlStreamWriter* : flux xml
     * @brief sauvegarde au travers du flux xml
     * @see BudgetCategory
     */
    void save(QXmlStreamWriter* _xml) const;

    /**
     * @fn BudgetCategory* Calendar::operator>>(QString _subject)
     * @param _subject QString : l'intitulé de la catégorie à rechercher
     * @return la catégorie correspondant à l'intitulé ou nullptr si aucune catégorie ne correspond à l'intitulé
     */
    BudgetCategory* operator>>(QString _subject);

    /**
     * @fn void Calendar::show() override
     * @see Container
     * @brief affiche les informations du calendrier et de ses catégories budgétaires
     */
    void show() override;

    virtual ~Calendar() = default; //!< Destructeur par défaut
};



/**
 * @fn QString const& operator<<(QString const& _s1, QString const& _s2)
 * @param _s1 QString const& : chaîne à laquelle concaténer
 * @param _s2 QString const& : chaîne à concaténer
 * @return une chaîne correspondant à la concaténation de deux chaînes
 */
QString const operator<<(QString const& _s1, QString const& _s2);

/**
 * @fn std::ostream& operator<<(std::ostream& _f, QString const& _s)
 * @param _f std::ostream& : sortie standard à laquelle concaténer
 * @param _s QString const& : chaîne à concaténer
 * @return une sortie standard correspondant à la concaténation d'une sortie standard et d'une chaîne
 */
std::ostream& operator<<(std::ostream& _f, QString const& _s);

/**
 * @fn std::ostream& operator<<(std::ostream& _f, Date const& _d)
 * @param _f std::ostream& : sortie standard à laquelle concaténer
 * @param _d Date const& : date à concaténer
 * @see Date
 * @return une sortie standard correspondant à la concaténation d'une sortie standard et d'une date
 */
std::ostream& operator<<(std::ostream& _f, Date const& _d);

/**
 * @fn QString getAttributeFromXML(QXmlStreamReader* _xml,QString const _attr)
 * @param _xml QXmlStreamReader* : flux xml
 * @param _attr QString const& : nom de l'attribut à retourner
 * @return la valeur de l'attribut rechercher dans le flux xml
 */
QString getAttributeFromXML(QXmlStreamReader* _xml,QString const _attr);

#endif // CALENDAR_H
