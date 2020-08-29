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

#ifndef PARAMETERS_H
#define PARAMETERS_H

/**
  @file Parameters.h
  @author Jérémy Godde
  @version 1
  @date july 2020
  */


#include "Exception.h"
#include <QtXml>
#include <QTextCodec>
#include <QMessageBox>
#include <QDesktopServices>

/**
 * @brief class Parameters
 * Represente le gestionnaire des paramètres de l'application
 */
class Parameters : public QObject
{
    Q_OBJECT
protected:
    QString const cfg_save = "config.xml"; //!< emplacement du fichier de configuration
    QUrl const url_help = QUrl("https://github.com/JeremyGodde/Lukos/wiki/Aide"); //!< url de la page d'aide

    QString app_name = "Lukos"; //!< nom courrant de l'application
    QString app_long_name = app_name + " Budgeting Application"; //!< nom complet de l'application
    QString app_version = "Abricot (0.3)";  //!< version actuelle de l'application

    QString devise;  //!< devise des valeurs
    bool devise_visible; //!< affiche la devise dans les tableaux si true, sinon n'affiche pas la devise dans les tableaux
    QString dir_registry; //!< emplacement du dossier où doivent être exportées les archives
    QString dir_pdf; //!< emplacement du dossier où doivent être exportées les pdf
    QString file_save; //!< emplacement du fichier de sauvegarde
    QString format_date; //!< format d'affichage de la date
    unsigned graphics_max_points; //!< nombre de point d'afficher sur le graphique

public:

    Parameters(); //!< constructeur sans arguments
    ~Parameters() = default; //!< destructeur par défaut

    /**
     * @fn bool Parameters::openHelp() const
     * @brief ouvre dans le navigateur par défaut la page d'aide de l'application
     * @return true si l'ouverture est un succès, false sinon
     */
    bool openHelp() const;

    /**
     * @fn QString const& Parameters::getAppName() const
     * @return le nom courrant de l'application
     */
    QString const& getAppName() const;

    /**
     * @fn QString const& Parameters::getAppLongName() const
     * @return le nom complet de l'application
     */
    QString const& getAppLongName() const;

    /**
     * @fn QString const& Parameters::getAppVersion() const
     * @return la version actuelle de l'application
     */
    QString const& getAppVersion() const;

    /**
     * @fn QString const& Parameters::getDevise() const
     * @return la devise des valeurs
     */
    QString const& getDevise() const;

    /**
     * @fn bool Parameters::getDeviseVisible() const
     * @return si la devise doit ou non être affichée dans les tableaux
     */
    bool getDeviseVisible() const;

    /**
     * @fn QString const& Parameters::getDirRegistry() const
     * @return l'emplacement du dossier d'archivage
     */
    QString const& getDirRegistry() const;

    /**
     * @fn QString const& Parameters::getDirPDF() const
     * @return l'emplacement du dossier d'export de pdf
     */
    QString const& getDirPDF() const;

    /**
     * @fn QString const& Parameters::getFileSave() const
     * @return l'emplacement du ficher de sauvegarde
     */
    QString const& getFileSave() const;

    /**
     * @fn QString const& Parameters::getFormatDate() const
     * @return le format d'affichage de date
     */
    QString const& getFormatDate() const;

    /**
     * @fn QString const& Parameters::getGraphicsMaxPoints() const
     * @return le nombre maximal de points qu'affiche le graphique
     */
    unsigned getGraphicsMaxPoints() const;

    /**
     * @fn void Parameters::save()
     * @brief effectue une sauvegarde des paramètres
     */
    void save();

public slots:
    /**
     * @fn inline void Parameters::changeDevise(QString const _devise)
     * @param _devise QString const : la devise
     * @brief modifie la devise
     */
    inline void changeDevise(QString const _devise) {devise = _devise;}

    /**
     * @fn inline void Parameters::changeDeviseVisible(bool _visible)
     * @param _visible bool : état d'affichage de la devise dans les tableaux
     * @brief modifie l'état d'affichage de la devise dans les tableaux
     */
    inline void changeDeviseVisible(bool _visible){devise_visible = _visible;}

    /**
     * @fn inline void Parameters::changeFormatDate(QString const _format)
     * @param _format QString const : le format d'affichage des dates
     * @brief modifie le format d'affichage des dates
     */
    inline void changeFormatDate(QString const _format){format_date = _format;}

    /**
     * @fn inline void Parameters::changeMaxPoints(unsigned _max_points)
     * @param _max_points unsigned : le nombre maximal de point à afficher sur le graphique
     * @brief modifie le nombre maximal de point à afficher sur le graphique
     */
    inline void changeMaxPoints(unsigned _max_points) {graphics_max_points = _max_points;}

    /**
     * @fn inline void Parameters::changeWriteSave(QString const _file)
     * @param _file QString const : l'emplacement du fichier de sauvegarde
     * @brief modifie l'emplacement du fichier
     */
    inline void changeWriteSave(QString const _file){file_save = _file;}

    /**
     * @fn inline void Parameters::changeWriteRegistry(QString const _dir)
     * @param _dir QString const : l'emplacement du dossier d'archivage
     * @brief modifie l'emplacement du dossier d'archivage
     */
    inline void changeWriteRegistry(QString const _dir){dir_registry = _dir;}

    /**
     * @fn inline void Parameters::changeWritePDF(QString const _dir)
     * @param _dir QString const : l'emplacement du dossier d'export de pdf
     * @brief modifie l'emplacement du dossier d'export de pdf
     */
    inline void changeWritePDF(QString const _dir){dir_pdf = _dir;}
};

#endif // PARAMETERS_H
