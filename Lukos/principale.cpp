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

#include "principale.h"
#include "ui_principale.h"
#include <iostream>

Principale::Principale(Parameters& _parameters, QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principale)
    , parameters(_parameters)
{
    //Réglage de la fenêtre
    ui->setupUi(this);
    this->setWindowTitle(parameters.getAppName());
    ui->labelBudgetPrevision->setVisible(false);
    ui->doubleBudgetPrevision->setVisible(false);
    ui->lineBudgetPrevision->setVisible(false);
    ui->buttonBudgetPrevision->setVisible(false);
    ui->labelBudgetOccurences->setVisible(false);
    ui->groupBudgetOccurences->setVisible(false);

    //Réglage du tableau
    ui->tableauOperations->horizontalHeader()->setVisible(true);
    ui->tableauOperations->setColumnHidden(7,true);
    ui->tableauOperations->setToolTipDuration(10000);
    ui->tableBudgetCategory->horizontalHeader()->setVisible(true);
    ui->tableBudget->horizontalHeader()->setVisible(true);

    //Insertion de la date courante
    ui->editDate->setDate(QDate::currentDate());
    ui->editBudgetDate->setDate(QDate::currentDate());

    //Chargement de sauvegarde
    QFile fichier(parameters.getFileSave());
    if(fichier.open(QIODevice::ReadOnly)){
        try
        {
            QXmlStreamReader* xml = new QXmlStreamReader(&fichier);

            xml->readNextStartElement();
            if(xml->qualifiedName() != "Application")
                throw Exception("Fichier corrompu : Application était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");

            QString __version = getAttributeFromXML(xml,"version");   //variable non-utilisée

            //vérifier la compatibilité inter-version

            xml->readNextStartElement();
            calendar.load(xml);
        }
        catch(Exception& e)
        {
            QMessageBox::warning(this,"Processus de chargement de sauvegarde",e.getString());
        }
    }
    fichier.close();

    //réglage des paramètres
    ui->comboParDevise->setCurrentText(parameters.getDevise());
        ui->doubleBudgetPrevision->setSuffix(" "+parameters.getDevise());
        ui->editMontant->setSuffix(" "+parameters.getDevise());
        ui->tableBudget->horizontalHeaderItem(2)->setText("Prévision "+parameters.getDevise());
        ui->tableBudget->horizontalHeaderItem(3)->setText("Solde "+parameters.getDevise());
        ui->tableauOperations->horizontalHeaderItem(4)->setText("Débit "+parameters.getDevise());
        ui->tableauOperations->horizontalHeaderItem(5)->setText("Crédit "+parameters.getDevise());
        ui->tableauOperations->horizontalHeaderItem(6)->setText("Solde "+parameters.getDevise());
    ui->checkParDevise->setChecked(parameters.getDeviseVisible());
    ui->radioParFormatDate1->setChecked(true);                      // paramètre à ajouter
    ui->radioParFormatDate2->setChecked(false);                     // paramètre à ajouter
    ui->radioParFormatDate3->setChecked(false);                     // paramètre à ajouter
    ui->radioParFormatDate4->setChecked(false);                     // paramètre à ajouter
    ui->radioParFormatDate5->setChecked(false);                     // paramètre à ajouter
    ui->spinParMaxPoints->setValue(parameters.getGraphicsMaxPoints());
    ui->lineParSave->setText(parameters.getFileSave());
    ui->lineParArchive->setText(parameters.getDirRegistry());
    ui->lineParPDF->setText(parameters.getDirPDF());

    //réglage du graphics
    ui->frameBudget->setMaxPoints(parameters.getGraphicsMaxPoints());

    //Réglage des valeurs de fréquences
    ui->comboBudgetFrequence->setItemData(1,QVariant::fromValue<Frequency*>(nullptr));
    ui->comboBudgetFrequence->setItemData(1,QVariant::fromValue<Frequency*>(new Frequency(0,0,1)));
    ui->comboBudgetFrequence->setItemData(2,QVariant::fromValue<Frequency*>(new Frequency(0,6,0)));
    ui->comboBudgetFrequence->setItemData(3,QVariant::fromValue<Frequency*>(new Frequency(0,3,0)));
    ui->comboBudgetFrequence->setItemData(4,QVariant::fromValue<Frequency*>(new Frequency(0,2,0)));
    ui->comboBudgetFrequence->setItemData(5,QVariant::fromValue<Frequency*>(new Frequency(0,1,0)));
    ui->comboBudgetFrequence->setItemData(6,QVariant::fromValue<Frequency*>(new Frequency(2,0,0)));
    ui->comboBudgetFrequence->setItemData(7,QVariant::fromValue<Frequency*>(new Frequency(1,0,0)));

    ui->comboPeriode->setItemData(0,QVariant::fromValue<Frequency*>(new Frequency(0,0,100)));
    ui->comboPeriode->setItemData(1,QVariant::fromValue<Frequency*>(new Frequency(1,0,0)));
    ui->comboPeriode->setItemData(2,QVariant::fromValue<Frequency*>(new Frequency(0,1,0)));
    ui->comboPeriode->setItemData(3,QVariant::fromValue<Frequency*>(new Frequency(0,3,0)));
    ui->comboPeriode->setItemData(4,QVariant::fromValue<Frequency*>(new Frequency(0,6,0)));
    ui->comboPeriode->setItemData(5,QVariant::fromValue<Frequency*>(new Frequency(0,0,1)));

    //Rafraîchissements et réglage des recherches
    ui->comboNumeroRecherche->addItem("");
    ui->comboIntituleRecherche->addItem("");
    if(calendar.getSize() > 0)
    {
        refreshTableau(true);
        refreshTableauBudgetCategory();
        refreshTableauBudget(0);
        refreshListBudget();
    }

    //Connectique :
    // * Opérations
    connect(ui->editIntitule,SIGNAL(textChanged(QString)),this,SLOT(activeVerification()));
    connect(ui->editNumero,SIGNAL(textChanged(QString)),this,SLOT(activeVerification()));
    connect(ui->editMontant,SIGNAL(valueChanged(double)),this,SLOT(activeVerification()));
    connect(ui->boutonsValidation,SIGNAL(accepted()),this,SLOT(addOperation()));
    connect(ui->buttonSupprimerOperation,SIGNAL(clicked()),this,SLOT(delOperation()));
    // * Filtre opérations
    connect(ui->comboPeriode,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshTableau()));
    connect(ui->comboNumeroRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByNumero(QString const&)));
    connect(ui->comboIntituleRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByIntitule(QString const&)));
    // * Menu
    connect(ui->actionSauvegarder,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->action_propos,SIGNAL(triggered()),this,SLOT(apropos()));
    // * Budgets
    connect(ui->tableBudgetCategory,SIGNAL(cellClicked(int,int)),this,SLOT(refreshTableauBudget(int)));

    connect(ui->buttonBudgetCouleur,SIGNAL(clicked()),this,SLOT(chooseColor()));
    connect(ui->comboBudgetFrequence,SIGNAL(currentIndexChanged(int)),this,SLOT(activeOccurence()));
    connect(ui->editBudget,SIGNAL(textChanged(QString)),this,SLOT(activeVerificationBudget()));
    connect(ui->comboBudgetOccurence,SIGNAL(currentIndexChanged(int)),this,SLOT(activeVerificationBudget()));
    connect(ui->buttonBudgetAjouter,SIGNAL(clicked()),this,SLOT(addBudget()));

    connect(ui->tableBudget,SIGNAL(cellClicked(int,int)),ui->frameBudget,SLOT(focusOn(int)));
    connect(ui->tableauOperations,SIGNAL(cellClicked(int,int)),this,SLOT(activeButSuppOp()));
    connect(ui->tableBudget,SIGNAL(cellClicked(int,int)),this,SLOT(modPrevision(int)));
    connect(ui->buttonBudgetPrevision,SIGNAL(clicked()),this,SLOT(setPrevision()));

    connect(ui->buttonBudgetSupprimer,SIGNAL(clicked()),this,SLOT(supprimer()));
    // * Redimensionnement
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(resize(int)));
    // * Paramètres
    connect(ui->comboParDevise,SIGNAL(currentTextChanged(QString)),this,SLOT(changeDevise(QString)));
    connect(ui->checkParDevise,SIGNAL(stateChanged(int)),this,SLOT(changeDeviseVisible(int)));
    connect(ui->spinParMaxPoints,SIGNAL(valueChanged(int)),this,SLOT(changeMaxPoints(int)));
    connect(ui->buttonParSave,SIGNAL(clicked()),this,SLOT(selectWriteSave()));
    connect(ui->lineParSave,SIGNAL(returnPressed()),this,SLOT(changeWriteSave()));
    connect(ui->buttonParArchive,SIGNAL(clicked()),this,SLOT(selectWriteRegistry()));
    connect(ui->lineParArchive,SIGNAL(returnPressed()),this,SLOT(changeWriteRegistry()));
    connect(ui->buttonParPDF,SIGNAL(clicked()),this,SLOT(selectWritePDF()));
    connect(ui->lineParPDF,SIGNAL(returnPressed()),this,SLOT(changeWritePDF()));

    //taille maximale
    this->showMaximized();
}

// =SLOT=================================================================================

void Principale::activeVerification()
{
    ui->boutonsValidation->setDisabled(
        ui->editMontant->value() == 0. ||
        ui->editNumero->text().isEmpty() ||
        ui->editIntitule->text().isEmpty() ||
        ui->comboBudget->count() == 0
    );
}

void Principale::activeButSuppOp()
{
    ui->buttonSupprimerOperation->setEnabled(true);
}

void Principale::addOperation()
{
    //ajout de l'opération

    unsigned __i = ui->comboBudget->currentIndex();
    int __d, __m, __y;
    ui->editDate->date().getDate(&__y,&__m,&__d);
    calendar[__i]->addOperation(
                ui->editIntitule->text(),
                ui->editNumero->text(),
                new Date(Date::Day(__d),Date::Month(__m),__y),
                ui->editMontant->value(),
                ui->editDescription->text());

    //ajout dans les filtres

    ui->comboNumeroRecherche->addItem(ui->editNumero->text());
    ui->comboIntituleRecherche->addItem(ui->editIntitule->text());

    //nettoyage des champs

    ui->editIntitule->setText("");
    ui->editMontant->setValue(0.);
    ui->editNumero->setText("");
    ui->comboBudget->setCurrentIndex(0);
    ui->editDate->setDate(QDate::currentDate());
    ui->editDescription->setText("");

    //fonctions communes

    refreshTableau();
    refreshTableauBudget(__i);
    ui->frameBudget->update();
    setUnsaved();
}

void Principale::delOperation()
{
    int __row = ui->tableauOperations->currentRow();

    QString __date = ui->tableauOperations->item(__row,1)->text();
    QString __intitule = ui->tableauOperations->item(__row,3)->text();
    if(QMessageBox::warning(this,"Validation requise","Cette action nécessite une validation:\n\t- Supprimer l'opération \""+__intitule+"\" du "+__date, QMessageBox::StandardButton::Cancel | QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) != QMessageBox::StandardButton::Ok)
        return;

    ui->buttonSupprimerOperation->setDisabled(true);

    Operation* __op = ui->tableauOperations->item(__row,0)->data(1).value<Operation*>();
    if(__op == nullptr)
        throw Exception("Tentative de suppression d'une opération avec data d'opération nulle");
    BudgetCategory* __bc = ui->tableauOperations->item(__row,0)->data(0).value<BudgetCategory*>();
    if(__bc == nullptr)
        throw Exception("Tentative de suppression d'une opération avec data de budget nulle");

    double __montant = __op->getValue();
    if(__bc->getType() == BudgetCategory::Type::credit)
        __montant *= -1;
    calendar.addBalance(__montant);

    __bc->delOperation(__op);

    refreshTableau();
    ui->buttonSupprimerOperation->setDisabled(ui->tableauOperations->rowCount() == 0);
    refreshTableauBudget(ui->tableBudgetCategory->currentRow());
    ui->frameBudget->update();
    setUnsaved();
}

void Principale::searchByIntitule(QString const& _text)
{
    disconnect(ui->comboNumeroRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByNumero(QString const&)));
    ui->comboNumeroRecherche->setCurrentIndex(0);
    connect(ui->comboNumeroRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByNumero(QString const&)));

    int __i = 0;
    for(;__i < ui->tableauOperations->rowCount(); __i++)
    {
        ui->tableauOperations->setRowHidden(
                    __i,
                    !_text.isEmpty() && _text != ui->tableauOperations->item(__i,3)->text());
    }
}

void Principale::searchByNumero(QString const& _text)
{
    disconnect(ui->comboIntituleRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByIntitule(QString const&)));
    ui->comboIntituleRecherche->setCurrentIndex(0);
    connect(ui->comboIntituleRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByIntitule(QString const&)));

    int __i = 0;
    for(;__i < ui->tableauOperations->rowCount(); __i++)
    {
        ui->tableauOperations->setRowHidden(
                    __i,
                    !_text.isEmpty() && _text != ui->tableauOperations->item(__i,2)->text());
    }
}

void Principale::activeVerificationBudget()
{
    ui->buttonBudgetAjouter->setDisabled(
        ui->editBudget->text().isEmpty()||
        ui->buttonBudgetCouleur->icon().isNull() ||
        ui->comboBudgetFrequence->currentIndex() == 0 ||
        ui->comboBudgetOccurence->currentIndex() == 0 ||
        (calendar >> ui->editBudget->text()) != nullptr
    );
}

void Principale::activeOccurence()
{
    if(ui->comboBudgetFrequence->currentIndex() == 0)
    {
        ui->comboBudgetOccurence->setCurrentIndex(0);
        ui->comboBudgetOccurence->setDisabled(true);
        return;
    }

    ui->comboBudgetOccurence->clear();
    ui->comboBudgetOccurence->addItem("Choisissez d'abord une fréquence",QVariant(0));

    Frequency * __frequency = ui->comboBudgetFrequence->currentData().value<Frequency*>();

    if(__frequency->getNbWeek()>0)
    {
        unsigned __v = __frequency->getNbWeek();
        unsigned __i = 1;
        for(;__i <= 26;__i++)
        {
            ui->comboBudgetOccurence->addItem("sur "+QString::number(__i*__v)+" semaines",QVariant(__i*__v));
        }
    }
    else if(__frequency->getNbMonth()>0)
    {
        unsigned __v = __frequency->getNbMonth();
        unsigned __i = 1;
        for(;__i <= 24;__i++)
        {
            ui->comboBudgetOccurence->addItem("sur "+QString::number(__i*__v)+" mois",QVariant(__i*__v));
        }
    }
    else if(__frequency->getNbYear()>0)
    {
        unsigned __v = __frequency->getNbYear();
        unsigned __i = 1;
        for(;__i <= 15;__i++)
        {
            ui->comboBudgetOccurence->addItem("sur "+QString::number(__i*__v)+" ans",QVariant(__i*__v));
        }
    }

    ui->comboBudgetOccurence->setEnabled(true);
}

void Principale::addBudget()
{
    QDate __date = ui->editBudgetDate->date();

    Frequency* __frequency = new Frequency(*ui->comboBudgetFrequence->currentData().value<Frequency*>());
    unsigned __occurence = ui->comboBudgetOccurence->currentData().toUInt();

    BudgetCategory::Type __type = (ui->comboBudgetType->currentIndex() == 0) ? BudgetCategory::Type::credit : BudgetCategory::Type::debit;
    calendar.addBudgetCategory(
                                ui->editBudget->text(),
                                new Date(Date::Day(__date.day()),Date::Month(__date.month()),Date::Year(__date.year())),
                                __frequency,
                                __occurence,
                                __type,
                                lastSelectionColor.rgb());

    ui->editBudget->clear();
    ui->comboBudgetOccurence->setCurrentIndex(0);
    ui->comboBudgetOccurence->setDisabled(true);
    ui->comboBudgetFrequence->setCurrentIndex(0);
    ui->comboBudgetType->setCurrentIndex(0);
    lastSelectionColor = QColor("black");
    ui->buttonBudgetCouleur->setIcon(QIcon());
    ui->buttonBudgetCouleur->setText("Sélectionner...");
    ui->editBudgetDate->setDate(QDate::currentDate());

    ui->buttonBudgetSupprimer->setEnabled(calendar.getSize() > 0);

    refreshTableauBudgetCategory();
    refreshListBudget();
    setUnsaved();
}

void Principale::chooseColor()
{
    QColorDialog* dialogColor = new QColorDialog(lastSelectionColor,this);
    dialogColor->setLocale(QLocale(QLocale::Language::French,QLocale::Country::France));
    dialogColor->setModal(true);
    connect(dialogColor,SIGNAL(colorSelected(const QColor &)),this,SLOT(getChosedColor(QColor const&)));
    dialogColor->show();
}

void Principale::getChosedColor(QColor const& _color)
{
    lastSelectionColor = _color;
    QPixmap __pix(QSize(16,16));
    __pix.fill(lastSelectionColor);
    ui->buttonBudgetCouleur->setIcon(QIcon(__pix));
    ui->buttonBudgetCouleur->setText("");
    activeVerificationBudget();
}

void Principale::modPrevision(int _row)
{
    ui->doubleBudgetPrevision->setValue(ui->tableBudget->item(_row,2)->text().toDouble());

    ui->labelBudgetPrevision->setVisible(true);
    ui->doubleBudgetPrevision->setVisible(true);
    ui->lineBudgetPrevision->setVisible(true);
    ui->buttonBudgetPrevision->setVisible(true);
    ui->labelBudgetOccurences->setVisible(true);
    ui->groupBudgetOccurences->setVisible(true);

    ui->radioBudgetJusteOccurence->setChecked(true);
}

void Principale::setPrevision()
{
    BudgetCategory* __bc = calendar >> ui->tableBudgetCategory->item(ui->tableBudgetCategory->currentRow(),1)->text();

    if(__bc == nullptr)
    {
        QMessageBox::warning(this,"Processus de modification du budget prévisionnel","Il semblerait que le budget sélectionné ne corresponde à aucun budget en mémoire. Par conséquent aucune opération de modification ne sera menée.");
        return;
    }

    unsigned __i = ui->tableBudget->currentRow();
    do
    {
        (*__bc)[__i]->setEdit(ui->doubleBudgetPrevision->value());
        __i++;
    }while(__i < __bc->getSize() && ui->radioBudgetFuturesOccurences->isChecked());

    refreshTableauBudget(ui->tableBudgetCategory->currentRow());
    setUnsaved();
}

void Principale::supprimer()
{
    QString __str_bc = ui->tableBudgetCategory->item(ui->tableBudgetCategory->currentRow(),1)->text();
    if(QMessageBox::warning(this,"Validation requise","Cette action nécessite une validation:\n\t- Supprimer la catégorie de budget \""+__str_bc+"\"", QMessageBox::StandardButton::Cancel | QMessageBox::StandardButton::Ok, QMessageBox::StandardButton::Cancel) != QMessageBox::StandardButton::Ok)
        return;

    BudgetCategory* __bc = calendar >> __str_bc;
    if(__bc == nullptr){
        QMessageBox::warning(this,"Processus de suppression de budget","Il semblerait que le budget sélectionné ne corresponde à aucun budget en mémoire. Par conséquent aucune opération de suppression ne sera menée.");
        return;
    }
    calendar.remove(__bc);

    refreshTableauBudgetCategory();
    if(calendar.getSize() > 0)
    {
        ui->frameBudget->setBudgetCategory(calendar[0]);
        refreshTableauBudget(0);
    }
    else
    {
        ui->buttonBudgetSupprimer->setDisabled(true);
        ui->frameBudget->setBudgetCategory(nullptr);
        refreshTableauBudget(-1);
    }
    refreshListBudget();
    refreshTableau();
    ui->buttonSupprimerOperation->setDisabled(ui->tableauOperations->rowCount() == 0);
    setUnsaved();
}

void Principale::changeDevise(QString _devise)
{
    parameters.changeDevise(_devise);
    ui->doubleBudgetPrevision->setSuffix(" "+_devise);
    ui->editMontant->setSuffix(" "+_devise);
    ui->tableBudget->horizontalHeaderItem(2)->setText("Prévision "+_devise);
    ui->tableBudget->horizontalHeaderItem(3)->setText("Solde "+_devise);
    ui->tableauOperations->horizontalHeaderItem(4)->setText("Débit "+_devise);
    ui->tableauOperations->horizontalHeaderItem(5)->setText("Crédit "+_devise);
    ui->tableauOperations->horizontalHeaderItem(6)->setText("Solde "+_devise);
    if(parameters.getDeviseVisible())
    {
        refreshTableau();
        if(calendar.getSize() > 0)
        {
            ui->tableBudgetCategory->setCurrentCell(0,0);
            refreshTableauBudget(0);
        }
    }
    setUnsaved();
}

void Principale::changeDeviseVisible(int _visible)
{
    parameters.changeDeviseVisible(_visible > 0);
    refreshTableau();
    if(calendar.getSize() > 0)
    {
        ui->tableBudgetCategory->setCurrentCell(0,0);
        refreshTableauBudget(0);
    }
    setUnsaved();
}

void Principale::changeMaxPoints(int _max_points)
{
    unsigned __max_points = abs(_max_points);
    parameters.changeMaxPoints(__max_points);
    ui->frameBudget->setMaxPoints(__max_points);
    setUnsaved();
}

void Principale::changeWritePDF(QString const& _dir)
{
    parameters.changeWritePDF(_dir);
    ui->lineParPDF->setText(_dir);
    setUnsaved();
}

void Principale::changeWritePDF()
{
    parameters.changeWriteRegistry(ui->lineParPDF->text());
    setUnsaved();
}

void Principale::selectWritePDF()
{
    QString const __current_dir = ui->lineParPDF->text();
    QFileDialog *__fd = new QFileDialog(this,"Selectionner le dossier d'export des PDF",__current_dir);
    __fd->setFileMode(QFileDialog::FileMode::Directory);
    connect(__fd,SIGNAL(fileSelected(const QString)),this,SLOT(changeWritePDF(const QString)));
    __fd->show();
}

void Principale::changeWriteRegistry(QString const& _dir)
{
    parameters.changeWriteRegistry(_dir);
    ui->lineParArchive->setText(_dir);
    setUnsaved();
}

void Principale::changeWriteRegistry()
{
    parameters.changeWriteRegistry(ui->lineParArchive->text());
    setUnsaved();
}

void Principale::selectWriteRegistry()
{
    QString const __current_dir = ui->lineParArchive->text();
    QFileDialog *__fd = new QFileDialog(this,"Selectionner le dossier d'export des archives",__current_dir);
    __fd->setFileMode(QFileDialog::FileMode::Directory);
    connect(__fd,SIGNAL(fileSelected(const QString)),this,SLOT(changeWriteRegistry(const QString)));
    __fd->show();
}

void Principale::changeWriteSave(QString const& _file)
{
    parameters.changeWriteSave(_file);
    ui->lineParSave->setText(_file);
    setUnsaved();
}

void Principale::changeWriteSave()
{
    parameters.changeWriteSave(ui->lineParSave->text());
    setUnsaved();
}

void Principale::selectWriteSave()
{
    QString const __current_file = ui->lineParSave->text();
    QFileDialog *__fd = new QFileDialog(this,"Selectionner le fichier d'enregistrement","./","xml (*.xml)");
    __fd->setFileMode(QFileDialog::FileMode::AnyFile);
    connect(__fd,SIGNAL(fileSelected(const QString)),this,SLOT(changeWriteSave(const QString)));
    __fd->show();
}

// =REFRESH==============================================================================

void Principale::refreshListBudget()
{
    ui->comboBudget->clear();
    ui->comboBudget->setIconSize(QSize(16,16));
    unsigned __i = 0;
    for(;__i < calendar.getSize();__i++)
    {
        QPixmap __pix(QSize(16,16));
        __pix.fill(QColor(calendar[__i]->getColor()));

        QString __type = ", crédit";
        if(calendar[__i]->getType() == BudgetCategory::Type::debit)
            __type = ", débit";

        ui->comboBudget->addItem(QIcon(__pix),calendar[__i]->getSubject() + __type);
    }
}

void Principale::refreshTableau(bool _init_recherches)
{
    disconnect(ui->comboNumeroRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByNumero(QString const&)));
    disconnect(ui->comboIntituleRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByIntitule(QString const&)));
    ui->comboIntituleRecherche->setCurrentIndex(0);
    ui->comboNumeroRecherche->setCurrentIndex(0);
    connect(ui->comboIntituleRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByIntitule(QString const&)));
    connect(ui->comboNumeroRecherche,SIGNAL(currentTextChanged(QString const&)),this,SLOT(searchByNumero(QString const&)));

    ui->tableauOperations->clearContents();
    ui->tableauOperations->setRowCount(0);

    ui->tableauOperations->sortByColumn(-1,Qt::SortOrder::AscendingOrder);

    Frequency* __f_affichage = ui->comboPeriode->currentData().value<Frequency*>();
    Date __aujdh = Date::getToday();
    Date __debut = __aujdh - *__f_affichage;

    unsigned __row = 0,
             __base = 0;
    auto __i_categorie = calendar.begin();
    for(;!__i_categorie.isTerminated();__i_categorie++)
    {
        auto __i_budget = (*__i_categorie)->begin();
        for(;!__i_budget.isTerminated();__i_budget++)
        {
            Budget* __b = *__i_budget;

            unsigned __i_operation = 0, __nb_affiches = 0;
            for(;__i_operation < __b->getSize();__i_operation++)
            {

                Operation* __o = (*__b)[__i_operation];

                if(!(__o->getDate().operator<(__debut)))
                {
                     __row ++;
                    ui->tableauOperations->setRowCount(__row);

                    //couleur de catégorie de budget

                    QTableWidgetItem* __twi_color = new QTableWidgetItem();
                        __twi_color->setBackground(QBrush(QColor((*__i_categorie)->getColor())));
                        __twi_color->setFlags(__twi_color->flags()^ Qt::ItemIsEditable);
                        __twi_color->setToolTip(__o->getDescription());
                        __twi_color->setData(0,QVariant::fromValue<BudgetCategory*>(*__i_categorie));
                        __twi_color->setData(1,QVariant::fromValue<Operation*>(__o));
                        ui->tableauOperations->setItem(__base+__i_operation,0,__twi_color);

                    //date

                    QTableWidgetItem* __twi_date = new QTableWidgetItem(__o->getDate().toString());
                        __twi_date->setTextAlignment(Qt::AlignCenter);
                        __twi_date->setFlags(__twi_date->flags()^ Qt::ItemIsEditable);
                        __twi_date->setToolTip(__o->getDescription());
                        ui->tableauOperations->setItem(__base+__i_operation,1,__twi_date);

                        QTableWidgetItem* __twi_date_tri = new QTableWidgetItem(__o->getDate().toString("yyyy/mm/dd"));
                            __twi_date_tri->setTextAlignment(Qt::AlignCenter);
                            __twi_date_tri->setFlags(__twi_date_tri->flags()^ Qt::ItemIsEditable);
                            __twi_date_tri->setToolTip(__o->getDescription());
                            ui->tableauOperations->setItem(__base+__i_operation,7,__twi_date_tri);

                    //référence

                    QTableWidgetItem* __twi_num = new QTableWidgetItem(__o->getReference());
                        __twi_num->setTextAlignment(Qt::AlignCenter);
                        __twi_num->setFlags(__twi_num->flags()^ Qt::ItemIsEditable);
                        __twi_num->setToolTip(__o->getDescription());
                        ui->tableauOperations->setItem(__base+__i_operation,2,__twi_num);
                        if(_init_recherches) ui->comboNumeroRecherche->addItem(__o->getReference());

                    //intitulé

                    QTableWidgetItem* __twi_int = new QTableWidgetItem(__o->getSubject());
                        __twi_int->setTextAlignment(Qt::AlignCenter);
                        __twi_int->setFlags(__twi_int->flags()^ Qt::ItemIsEditable);
                        __twi_int->setToolTip(__o->getDescription());
                        ui->tableauOperations->setItem(__base+__i_operation,3,__twi_int);
                        if(_init_recherches) ui->comboIntituleRecherche->addItem(__o->getSubject());

                    //débit et crédit

                    unsigned __colonne_m = 4, __colonne_nulle = 5;
                    if((*__i_categorie)->getType() == BudgetCategory::Type::credit)
                    {
                        __colonne_m = 5;
                        __colonne_nulle = 4;
                    }
                    QTableWidgetItem* __twi_montant = new QTableWidgetItem(QString::number(__o->getValue(),'f',2));
                        if(parameters.getDeviseVisible())
                        {
                            __twi_montant->setText(__twi_montant->text()+" "+parameters.getDevise());
                        }
                        __twi_montant->setTextAlignment(Qt::AlignCenter);
                        __twi_montant->setFlags(__twi_montant->flags()^ Qt::ItemIsEditable);
                        __twi_montant->setToolTip(__o->getDescription());
                        ui->tableauOperations->setItem(__base+__i_operation,__colonne_m,__twi_montant);

                    QTableWidgetItem* __twi_nulle = new QTableWidgetItem("-");
                        __twi_nulle->setTextAlignment(Qt::AlignCenter);
                        __twi_nulle->setFlags(__twi_nulle->flags()^ Qt::ItemIsEditable);
                        __twi_nulle->setToolTip(__o->getDescription());
                        ui->tableauOperations->setItem(__base+__i_operation,__colonne_nulle,__twi_nulle);
                    __nb_affiches++;
                }
            }
            __base += __nb_affiches;
        }
    }

    //tri par date pour calculer le solde

    ui->tableauOperations->sortByColumn(7,Qt::SortOrder::AscendingOrder);

    //calcul du solde opération par opération

    unsigned __i = 0;
    double solde = 0.;
    for(;__i<__row;__i++)
    {
        QString __m;
        if((__m = ui->tableauOperations->item(__i,4)->text()) != '-')
        {
            solde -= __m.toDouble();
        }
        else if((__m = ui->tableauOperations->item(__i,5)->text())!= '-')
        {
            solde += __m.toDouble();
        }

        QTableWidgetItem* __twi_solde = new QTableWidgetItem(QString::number(solde,'f',2));
            if(parameters.getDeviseVisible())
            {
                __twi_solde->setText(__twi_solde->text()+" "+parameters.getDevise());
            }
            __twi_solde->setTextAlignment(Qt::AlignCenter);
            __twi_solde->setFlags(__twi_solde->flags()^ Qt::ItemIsEditable);
            ui->tableauOperations->setItem(__i,6,__twi_solde);
    }

    //affichage du solde

    if(ui->comboPeriode->currentIndex() == 0)
    {
        ui->titre->setText("Toutes les opérations\nSolde actuel : "+QString::number(calendar.getBalance(),'f',2)+" "+parameters.getDevise());
    }
    else
    {
        ui->titre->setText("Du "+__debut.toString()+" au "+__aujdh.toString()+"\nSolde en entrée : "+QString::number(calendar.getBalance() - solde,'f',2)+" "+parameters.getDevise()+" | Solde sur la période : "+QString::number(solde,'f',2)+" "+parameters.getDevise());
    }


    if(solde < 0)
    {
        ui->titre->setStyleSheet("color : red;");
    }
    else{
        ui->titre->setStyleSheet("color : initial;");
    }

    //tri selon les paramètres

    ui->tableauOperations->sortByColumn(7,Qt::SortOrder::AscendingOrder);

    //refresh schéma
    this->update(ui->frameBudget->rect());
}

void Principale::refreshTableauBudgetCategory()
{
    ui->tableBudgetCategory->clearContents();

    ui->tableBudgetCategory->setRowCount(calendar.getSize());

    unsigned __i = 0;
    for(;__i < calendar.getSize();__i++)
    {
        //couleur

        QTableWidgetItem* __twi_color = new QTableWidgetItem();
            __twi_color->setBackground(QBrush(QColor(calendar[__i]->getColor())));
            __twi_color->setFlags(__twi_color->flags()^ Qt::ItemIsEditable);
            ui->tableBudgetCategory->setItem(__i,0,__twi_color);

        //catégorie

        QTableWidgetItem* __twi_nom = new QTableWidgetItem(calendar[__i]->getSubject());
            __twi_nom->setTextAlignment(Qt::AlignCenter);
            __twi_nom->setFlags(__twi_nom->flags()^ Qt::ItemIsEditable);
            ui->tableBudgetCategory->setItem(__i,1,__twi_nom);

        //type
        QString __type = "inconnu";

        if(calendar[__i]->getType() == BudgetCategory::Type::credit)
            __type = "Crédit";
        else if(calendar[__i]->getType() == BudgetCategory::Type::debit)
            __type = "Débit";

        QTableWidgetItem* __twi_type = new QTableWidgetItem(__type);
            __twi_type->setTextAlignment(Qt::AlignCenter);
            __twi_type->setFlags(__twi_type->flags()^ Qt::ItemIsEditable);
            ui->tableBudgetCategory->setItem(__i,2,__twi_type);
    }

    //tri par ordre alphabétique

    ui->tableBudgetCategory->sortByColumn(1,Qt::SortOrder::AscendingOrder);

    // selection par défaut
    if(calendar.getSize()>0)
        ui->tableBudgetCategory->setCurrentCell(0,1);
}

void Principale::refreshTableauBudget(int _category)
{
    if(_category < 0)
    {
        ui->tableBudget->clearContents();
        ui->tableBudget->setRowCount(0);
        return;
    }

    unsigned __category = _category;
    if(__category >= calendar.getSize())
        return;

    BudgetCategory* __ptrbc = calendar >> ui->tableBudgetCategory->item(__category,1)->text();
    if(__ptrbc == nullptr)
    {
        QMessageBox::warning(this,"Processus d'affichage de budget","Il semblerait que le budget sélectionné ne corresponde à aucun budget en mémoire. Par conséquent aucune opération d'affichage ne sera menée.");
        return;
    }

    ui->frameBudget->setBudgetCategory(__ptrbc); //pour générer le schéma correspondant

    BudgetCategory& __bc = *__ptrbc;
    ui->labelTitreBudget->setText("Historique du budget "+__bc.getSubject());

    ui->tableBudget->clearContents();

    ui->tableBudget->setRowCount(__bc.getSize());

    unsigned __i = 0;
    for(;__i < __bc.getSize();__i++)
    {
        //date de début

        QString __date;
        if(__i > 0) __date = __bc.getDate(__i-1).toString();
        else __date = (__bc.getDate() - __bc.getFrequency()).toString();
        QTableWidgetItem* __twi_dd = new QTableWidgetItem(__date);
            __twi_dd->setTextAlignment(Qt::AlignCenter);
            __twi_dd->setFlags(__twi_dd->flags()^ Qt::ItemIsEditable);
            ui->tableBudget->setItem(__i,0,__twi_dd);

        //date de fin

        QTableWidgetItem* __twi_df = new QTableWidgetItem(__bc.getDate(__i).toString());
            __twi_df->setTextAlignment(Qt::AlignCenter);
            __twi_df->setFlags(__twi_df->flags()^ Qt::ItemIsEditable);
            ui->tableBudget->setItem(__i,1,__twi_df);

        //prévision

        QTableWidgetItem* __twi_pre = new QTableWidgetItem(QString::number(__bc[__i]->getForecastBalance(),'f',2));
            if(parameters.getDeviseVisible())
            {
                __twi_pre->setText(__twi_pre->text()+" "+parameters.getDevise());
            }
            __twi_pre->setTextAlignment(Qt::AlignCenter);
            __twi_pre->setFlags(__twi_pre->flags()^ Qt::ItemIsEditable);
            ui->tableBudget->setItem(__i,2,__twi_pre);

        //solde

        QTableWidgetItem* __twi_solde = new QTableWidgetItem(QString::number(__bc[__i]->getBalance(),'f',2));
            if(parameters.getDeviseVisible())
            {
                __twi_solde->setText(__twi_solde->text()+" "+parameters.getDevise());
            }
            __twi_solde->setTextAlignment(Qt::AlignCenter);
            __twi_solde->setFlags(__twi_solde->flags()^ Qt::ItemIsEditable);
            ui->tableBudget->setItem(__i,3,__twi_solde);
    }

    ui->labelBudgetPrevision->setVisible(false);
    ui->doubleBudgetPrevision->setVisible(false);
    ui->lineBudgetPrevision->setVisible(false);
    ui->buttonBudgetPrevision->setVisible(false);
    ui->labelBudgetOccurences->setVisible(false);
    ui->groupBudgetOccurences->setVisible(false);

    if(ui->tableBudgetCategory->currentItem() != nullptr)
        ui->buttonBudgetSupprimer->setEnabled(true);

    ui->frameBudget->focusOn(0);
}

// =SAVE=================================================================================

void Principale::save()
{
    //sauvegarde des paramètres

    parameters.save();

    //sauvegarde des données

    QFile fichier(parameters.getFileSave());
    if(!fichier.open(QIODevice::WriteOnly)){
        fichier.close();
        throw Exception("impossible d'ouvrir le fichier");
    }
    QXmlStreamWriter* xml = new QXmlStreamWriter(&fichier);

    xml->setCodec("UTF-8");
    xml->setAutoFormatting(true);

    xml->writeStartDocument();
    xml->writeComment("\n\t@file\n\t\n\t@brief Saved datas of "+parameters.getAppLongName()+"\n\t@version "+parameters.getAppVersion()+"\n\t@author Jérémy Godde\n\t@copyright GNU GPL 3.0\n");
    xml->writeDTD("<!DOCTYPE xml>");
    xml->writeStartElement("Application");
    xml->writeAttribute("version",parameters.getAppVersion());

    calendar.save(xml);

    xml->writeEndElement();
    xml->writeEndDocument();

    delete xml;

    this->setWindowTitle(parameters.getAppName());
    ui->actionSauvegarder->setDisabled(true);
}

void Principale::setUnsaved()
{
    if(this->windowTitle() != parameters.getAppName() + " [unsaved]")
    {
       this->setWindowTitle(parameters.getAppName() + " [unsaved]");
       ui->actionSauvegarder->setEnabled(true);
    }
}

// =MENU=================================================================================

void Principale::apropos()
{
    QMessageBox::information(this,"À propos",
        parameters.getAppLongName()+"\n\nVersion : "+parameters.getAppVersion()+"\nAuthor: Jérémy Godde\nMade with Qt Creator 4.12.3 based on Qt 5.15\nLicense : GNU GPL 3 ; This is free software, and you are welcome to redistribute it under certain conditions\n\nCopyright © 2020 • Regulation by European and French law");
}

// ======================================================================================
// ======================================================================================
// ======================================================================================

void Principale::closeEvent(QCloseEvent *event)
{
    if(this->windowTitle() != parameters.getAppName() &&
      QMessageBox::StandardButton::Yes == QMessageBox::question(
           this,"Quitter","Voulez-vous enregistrer votre travail ?")
      )
    {
        save();
        event->accept();
    }
    else
    {
        event->accept();
    }
}

void Principale::resize(int _index)
{
    if(_index == 0)
    {
        //page principale

        ui->tableauOperations->setColumnWidth(0,10);
        double width_unity= (ui->tableauOperations->width() - 12)/8.;
        ui->tableauOperations->setColumnWidth(1,width_unity);
        ui->tableauOperations->setColumnWidth(2,width_unity);
        ui->tableauOperations->setColumnWidth(3,3*width_unity);
        ui->tableauOperations->setColumnWidth(4,width_unity);
        ui->tableauOperations->setColumnWidth(5,width_unity);
        ui->tableauOperations->setColumnWidth(6,width_unity);
    }
    else if(_index == 1)
    {
        //page budget

        double width_unity = ui->tableBudget->width()/10.;
        ui->tableBudget->setColumnWidth(0,3*width_unity);
        ui->tableBudget->setColumnWidth(1,3*width_unity);
        ui->tableBudget->setColumnWidth(2,2*width_unity);
        ui->tableBudget->setColumnWidth(3,2*width_unity);

        ui->tableBudgetCategory->setColumnWidth(0,10);
        width_unity = (ui->tableBudgetCategory->width() - 12)/4.;
        ui->tableBudgetCategory->setColumnWidth(1,3*width_unity);
        ui->tableBudgetCategory->setColumnWidth(2,width_unity);
    }
}

void Principale::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    resize(0);
    resize(1);
}

Principale::~Principale()
{
    delete ui;
}
