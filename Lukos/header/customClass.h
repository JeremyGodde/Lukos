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

#ifndef CUSTOMCLASS_H
#define CUSTOMCLASS_H

#include "Calendar.h"
#include <QtWidgets>
#include <QPainter>

/**
 * @brief class QCustomFrame
 * Represente un QFrame personnalisé permettant l'affichage d'un graphique
 */
class QCustomFrame : public QFrame
{
    Q_OBJECT

protected:
    /**
     * @fn virtual void QCustomFrame::paintEvent(QPaintEvent *) override
     * @brief événement personnalisé dessinant le graphique
     */
    virtual void paintEvent(QPaintEvent *) override;

    BudgetCategory const* category = nullptr; //!< pointeur sur la catégorie contenant les données à afficher sous forme de graphique

    int margin_x = 20; //!< dimension des marges horizontales en px
    int margin_y = 20; //!< dimension des marges verticales en px
    unsigned point = 0; //!< indice de la donnée sur laquelle s'effectue le centrage du graphique
    unsigned max_points = 6; //!< nombre de données à afficher sur le graphique

public:
    explicit QCustomFrame(QWidget* parent); //!< constructeur
    ~QCustomFrame() = default;  //!< destructeur par défaut

    /**
     * @fn void QCustomFrame::setBudgetCategory(BudgetCategory const* const _category)
     * @param _category BudgetCategory const* const : pointeur sur la catégorie
     * @brief modifie le ponteur sur la catégorie contenant les données à afficher sous forme de graphique
     */
    void setBudgetCategory(BudgetCategory const* const _category);

    /**
     * @fn void QCustomFrame::setMaxPoints(unsigned const _max_points)
     * @param _max_points unsigned const : nombre de données
     * @brief modifie le nombre de données à afficher sur le graphique
     */
    void setMaxPoints(unsigned const _max_points);

public slots:
    /**
     * @fn void QCustomFrame::focusOn(int _point)
     * @param _point int : indice de la donnée
     * @brief modifie l'indice de la donnée sur laquelle s'effectue le centrage du graphique
     */
    void focusOn(int _point);
};

#endif // CUSTOMCLASS_H
