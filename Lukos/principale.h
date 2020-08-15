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

#ifndef PRINCIPALE_H
#define PRINCIPALE_H

#include <QtWidgets>
#include <QPainter>
#include "header/Calendar.h"
#include "header/Parameters.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class Principale;
}
QT_END_NAMESPACE

class Principale : public QMainWindow
{
    Q_OBJECT

public:
    Principale(Parameters& _parameters,QMainWindow *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;
    ~Principale();

protected slots:
    void resize(int _index);

    //-----------------------------------------------------------menu
    void apropos();
    void save();

    //-----------------------------------------------------------onglet 1
    void activeVerification();
    void activeButSuppOp();
    void addOperation();
    void delOperation();
    void refreshTableau(bool _init_recherches = false);
    void refreshTableauBudget(int _category);
    void searchByNumero(QString const&);
    void searchByIntitule(QString const&);

    //-----------------------------------------------------------onglet 2
    void activeVerificationBudget();
    void activeOccurence();
    void addBudget();
    void chooseColor();
    void getChosedColor(QColor const&);
    void modPrevision(int _row);
    void setPrevision();
    void supprimer();

    //----------------------------------------------------------paramètres
    void changeDevise(QString);
    void changeDeviseVisible(int _visible);
    void changeMaxPoints(int);
    void changeWritePDF(const QString&);
    void changeWritePDF();
    void selectWritePDF();
    void changeWriteRegistry(const QString&);
    void changeWriteRegistry();
    void selectWriteRegistry();
    void changeWriteSave(const QString&);
    void changeWriteSave();
    void selectWriteSave();

protected:
    void closeEvent(QCloseEvent *event) override;
    void refreshListBudget();
    void refreshTableauBudgetCategory();
    void setUnsaved();

    Ui::Principale *ui;
    Calendar& calendar = Calendar::getInstance();
    Parameters& parameters;

    QColor lastSelectionColor = QColor("black");

    Frequency* f = nullptr;
};

#endif // PRINCIPALE_H
