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

#ifndef EVENT_H
#define EVENT_H

/**
  @file Event.h
  @author Jérémy Godde
  @version 1
  @date july 2020
  */

#include "Exception.h"
#include <QVariant>
#include <QDate>

/**
 * @brief class Frequency
 * Représente une fréquence de répétition.
 */

class Frequency
{
protected:
    unsigned nb_week; //!< nombre de semaines de fréquence
    unsigned nb_month; //!< nombre de mois de fréquence
    unsigned nb_year; //!< nombre d'années de fréquence

public:
    explicit Frequency(unsigned _nb_week,
                       unsigned _nb_month,
                       unsigned _nb_year); //!< constructeur

    /**
     * @fn unsigned Frequency::getNbWeek() const
     * @return la part de la fréquence correspondant aux semaines
     */
    unsigned getNbWeek() const;

    /**
     * @fn unsigned Frequency::getNbMonth() const
     * @return la part de la fréquence correspondant aux mois
     */
    unsigned getNbMonth() const;

    /**
     * @fn unsigned Frequency::getNbYear() const
     * @return la part de la fréquence correspondant aux années
     */
    unsigned getNbYear() const;

    /**
     * @fn QString const Frequency::toString() const
     * @return la fréquence au format dd mm yyyy
     */
    QString const toString() const;

    /**
     * @fn Frequency Frequency::operator*(unsigned const& _factor) const
     * @param _factor unsigned const& : occurence choisie
     * @return la frequence de l'occurence choisie
     */
    Frequency operator*(unsigned const& _factor) const;

    /**
     * @fn static Frequency* Frequency::fromString(QString _text)
     * @param _text QString : texte à convertir
     * @return un pointeur sur un object fréquence correspondant au texte
     */
    static Frequency* fromString(QString _text);

};

Q_DECLARE_METATYPE(Frequency*);

/**
 * @brief class Date
 * Représente une date.
 */

//AJOUT de operator<(Date const*)

class Date
{
public:
    /**
     * @enum Day
     * @brief ensemble des jours
     */
    enum Day{
        /**
         * @var d_1
         * @brief 1er jour du mois
         */
        d_1 = 1,
        /**
         * @var d_2
         * @brief 2eme jour du mois
         */
        d_2 = 2,
        /**
         * @var d_3
         * @brief 3eme jour du mois
         */
        d_3 = 3,
        /**
         * @var d_4
         * @brief 4eme jour du mois
         */
        d_4 = 4,
        /**
         * @var d_5
         * @brief 5eme jour du mois
         */
        d_5 = 5,
        /**
         * @var d_6
         * @brief 6eme jour du mois
         */
        d_6 = 6,
        /**
         * @var d_7
         * @brief 7eme jour du mois
         */
        d_7 = 7,
        /**
         * @var d_8
         * @brief 8eme jour du mois
         */
        d_8 = 8,
        /**
         * @var d_9
         * @brief 9eme jour du mois
         */
        d_9 = 9,
        /**
         * @var d_10
         * @brief 10eme jour du mois
         */
        d_10 = 10,
        /**
         * @var d_11
         * @brief 11eme jour du mois
         */
        d_11 = 11,
        /**
         * @var d_12
         * @brief 12eme jour du mois
         */
        d_12 = 12,
        /**
         * @var d_13
         * @brief 13eme jour du mois
         */
        d_13 = 13,
        /**
         * @var d_14
         * @brief 14eme jour du mois
         */
        d_14 = 14,
        /**
         * @var d_15
         * @brief 15eme jour du mois
         */
        d_15 = 15,
        /**
         * @var d_16
         * @brief 16eme jour du mois
         */
        d_16 = 16,
        /**
         * @var d_17
         * @brief 17eme jour du mois
         */
        d_17 = 17,
        /**
         * @var d_18
         * @brief 18eme jour du mois
         */
        d_18 = 18,
        /**
         * @var d_19
         * @brief 19eme jour du mois
         */
        d_19 = 19,
        /**
         * @var d_20
         * @brief 20eme jour du mois
         */
        d_20 = 20,
        /**
         * @var d_21
         * @brief 21eme jour du mois
         */
        d_21 = 21,
        /**
         * @var d_22
         * @brief 22eme jour du mois
         */
        d_22 = 22,
        /**
         * @var d_23
         * @brief 23eme jour du mois
         */
        d_23 = 23,
        /**
         * @var d_24
         * @brief 24eme jour du mois
         */
        d_24 = 24,
        /**
         * @var d_25
         * @brief 25eme jour du mois
         */
        d_25 = 25,
        /**
         * @var d_26
         * @brief 26eme jour du mois
         */
        d_26 = 26,
        /**
         * @var d_27
         * @brief 27eme jour du mois
         */
        d_27 = 27,
        /**
         * @var d_28
         * @brief 28eme jour du mois
         */
        d_28 = 28,
        /**
         * @var d_29
         * @brief 29eme jour du mois
         */
        d_29 = 29,
        /**
         * @var d_30
         * @brief 30eme jour du mois
         */
        d_30 = 30,
        /**
         * @var d_31
         * @brief 31eme jour du mois
         */
        d_31 = 31
    };

    /**
     * @enum Month
     * @brief ensemble des mois
     */
    enum Month{
        /**
         * @var january
         * @brief mois de janvier
         */
        january = 1,
        /**
         * @var february
         * @brief mois de février
         */
        february = 2,
        /**
         * @var march
         * @brief mois de mars
         */
        march = 3,
        /**
         * @var april
         * @brief mois de avril
         */
        april = 4,
        /**
         * @var may
         * @brief mois de mai
         */
        may = 5,
        /**
         * @var june
         * @brief mois de juin
         */
        june = 6,
        /**
         * @var july
         * @brief mois de juillet
         */
        july = 7,
        /**
         * @var august
         * @brief mois de août
         */
        august = 8,
        /**
         * @var september
         * @brief mois de septembre
         */
        september = 9,
        /**
         * @var october
         * @brief mois de octobre
         */
        october = 10,
        /**
         * @var november
         * @brief mois de novembre
         */
        november = 11,
        /**
         * @var december
         * @brief mois de décembre
         */
        december = 12
    };

    /**
     * @typedef unsigned Year
     * @brief représente l'année
     */
    typedef unsigned Year;

protected:
    Day day; //!< jour
    Month month; //!< mois
    Year year; //!< année

public:
    explicit Date(Day _day,Month _month,Year _year); //!< constructeur

    /**
     * @fn unsigned Date::getDay() const
     * @return le jour de la date
     */
    unsigned getDay() const;

    /**
     * @fn unsigned Date::getMonth() const
     * @return le mois de la date
     */
    unsigned getMonth() const;

    /**
     * @fn unsigned Date::getYear() const
     * @return l'année de la date
     */
    unsigned getYear() const;

    /**
     * @fn static Date const Date::getToday()
     * @return la date du jour
     */
    static Date const getToday();

    /**
     * @fn QString const Date::toString(QString const _format = "dd mmmm yyyy") const
     * @param _format QString const : format de la date
     * @return la date au format indiqué
     */
    QString const toString(QString const _format = "dd mmmm yyyy") const;

    /**
     * @fn bool Date::operator<(Date const& _date) const
     * @param _date Date const& : date avec laquelle comparer
     * @return vrai si la date est antérieure à la date passée en argument
     */
    bool operator<(Date const& _date) const;

    /**
     * @fn bool Date::operator<(Date const* _date) const
     * @param _date Date const* : date avec laquelle comparer
     * @return vrai si la date est antérieure à la date passée en argument
     */
    bool operator<(Date const* _date) const;

    /**
     * @fn bool Date::operator+(Frequency const& _frequency) const
     * @param _frequency Frequency const* : fréquence à ajouter
     * @see Frequency
     * @return la date correspondant à la somme de la date et de la fréquence
     */
    Date operator+(Frequency const& _frequency) const;

    /**
     * @fn bool Date::operator-(Frequency const& _frequency) const
     * @param _frequency Frequency const* : fréquence à soustraire
     * @see Frequency
     * @return la date correspondant à la différence de la date et de la fréquence
     */
    Date operator-(Frequency const& _frequency) const;

    Date& operator=(Date const& _date) = default; //!< operateur de recopie par défaut

    /**
     * @fn static Date* Date::fromString(QString _text)
     * @param _text QString : texte à convertir
     * @return un pointeur sur un object date correspondant au texte
     */
    static Date* fromString(QString _text);
};

/**
 * @brief class Event
 * Représente un événement.
 */

// AJOUT DE _occurence
// AJOUT DE getOccurence()
// MODIFICATION DE getDate(...)

class Event
{
protected:
    Date* date_start; //!< date de début de l'événement
    Frequency* frequency; //!< fréquence de l'événement
    unsigned occurence; //!< occurence de l'événement, si égale à 0 alors le nombre d'occurences est indéterminé

    QString subject; //!< sujet de l'événement

    explicit Event(QString const _subject,
                   Date* const _date,
                   Frequency* const _frequency,
                   unsigned _occurence); //!< constructeur

    ~Event() = default;
public:

    /**
     * @fn Date const Event::getDate(unsigned _occurence = 0) const
     * @param _occurence unsigned : occurence de l'événement pour laquelle retourner la date
     * @see Date
     * @return la date correspondant à l'occurence choisie de l'événement
     */
    Date const getDate(unsigned _occurence = 0) const;

    /**
     * @fn Frequency const& Event::getFrequency() const
     * @see Frequency
     * @return la fréquence de l'événement
     */
    Frequency const& getFrequency() const;

    /**
     * @fn unsigned Event::getOccurence() const
     * @return le nombre d'occurence
     */
    unsigned getOccurence() const;

    /**
     * @fn QString const Event::getSubject() const
     * @return le sujet de l'événement
     */
    QString const getSubject() const;

    /**
     * @fn bool Event::operator<(Event const* _event) const
     * @param _event Event const* : événement avec lequel comparer
     * @see Date
     * @return vrai si la date de l'événement est antérieure à la date de l'événement passé en argument
     */
    bool operator<(Event const* _event) const;

    /**
     * @fn bool Event::operator<(Event const& _event) const
     * @param _event Event const& : événement avec lequel comparer
     * @see Date
     * @return vrai si la date de l'événement est antérieure à la date de l'événement passé en argument
     */
    bool operator<(Event const& _event) const;

    /**
     * @fn bool Event::operator>(Event const* _event) const
     * @param _event Event const* : événement avec lequel comparer
     * @see Date
     * @return vrai si la date de l'événement est postérieure à la date de l'événement passé en argument
     */
    bool operator>(Event const* _event) const;

    /**
     * @fn bool Event::operator>(Event const& _event) const
     * @param _event Event const& : événement avec lequel comparer
     * @see Date
     * @return vrai si la date de l'événement est postérieure à la date de l'événement passé en argument
     */
    bool operator>(Event const& _event) const;

    Event& operator=(Event const& _event) = default; //!< operateur de recopie par défaut

    /**
     * @fn virtual void Event::save() = 0
     * @brief effectue une sauvegarde des données de l'événement
     */
    virtual void save() = 0;
};

/**
 * @fn QString const operator<<(QString const& _s, Frequency const& _f)
 * @param _s QString const& : chaîne à laquelle concaténer
 * @param _f Frequency const& : fréquence à concaténer
 * @see Frequency
 * @return une chaîne correspondant à la concaténation d'une chaîne et d'une fréquence
 */
QString const operator<<(QString const& _s, Frequency const& _f);

/**
 * @fn QString const operator<<(QString const& _s, Date const& _d)
 * @param _s QString const& : chaîne à laquelle concaténer
 * @param _d Date const& : date à concaténer
 * @see Date
 * @return une chaîne correspondant à la concaténation d'une chaîne et d'une date
 */
QString const operator<<(QString const& _s, Date const& _d);

#endif // EVENT_H
