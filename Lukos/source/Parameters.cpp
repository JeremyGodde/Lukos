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

#include "../header/Parameters.h"

Parameters::Parameters()
    : QObject()
{
    QFile fichier(cfg_save);
    if(!fichier.open(QIODevice::ReadOnly)){

        QWidget* m = new QWidget;
        if( QMessageBox::warning(m,"Chargement de la configuration",
            "Impossible de trouver le fichier de configuration. Voulez-vous quand même lancer une session avec les paramètres par défaut ?",
            QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No) == QMessageBox::StandardButton::No)
        {
            QMessageBox::critical(m,"Chargement de la configuration","Vous avez refusé d'utiliser les paramètres par défaut, l'application va par conséquent s'arrêter");
            delete m;
            abort();
        }
        delete m;

        devise = "€";
        devise_visible = false;
        graphics_max_points = 6;
        format_date = "dd mmmm yyyy";

        QString const REGISTRY = "registry",
                      SAVE = "save",
                      PDF = "export";

        if(!QDir(PDF).exists())
        {
            QDir().mkdir(PDF);
        }

        if(!QDir(REGISTRY).exists())
        {
            QDir().mkdir(REGISTRY);
        }

        if(!QDir(SAVE).exists())
        {
            QDir().mkdir(SAVE);
        }

        dir_pdf = PDF + "/";
        dir_registry = REGISTRY + "/";
        file_save = SAVE + "/save.xml";

        save();

        return;
    }
    QXmlStreamReader* xml = new QXmlStreamReader(&fichier);

    try{
        xml->readNextStartElement();
        if(xml->qualifiedName() != "Configuration")
            throw Exception("Fichier corrompu : Configuration était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");

        // version

        xml->readNextStartElement();
        if(xml->qualifiedName() != "Version")
            throw Exception("Fichier corrompu : Version était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        QString __version = xml->readElementText();
        if(__version < app_version &&
            QMessageBox::warning(nullptr,"Chargement de la configuration",
            "La version du fichier de configuration est antérieure à la vôtre. Voulez-vous quand même continuer ?",
            QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No) == QMessageBox::StandardButton::No)
        {
            throw Exception("Refus d'utilisation une version antérieure");
        }
        else if(__version > app_version)
        {
            QMessageBox::critical(nullptr,"Chargement de la configuration",
                                  "La version du fichier de configuration est postérieure à la vôtre. Nous ne pouvons la charger.");
            throw Exception("Version incompatible");
        }

        // file_save

        xml->readNextStartElement();
        if(xml->qualifiedName() != "Filesave")
            throw Exception("Fichier corrompu : Filesave était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        file_save = xml->readElementText();

        // dir_registry

        xml->readNextStartElement();
        if(xml->qualifiedName() != "DirectoryOfRegistry")
            throw Exception("Fichier corrompu : DirectoryOfRegistry était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        dir_registry = xml->readElementText();

        // dir_pdf

        xml->readNextStartElement();
        if(xml->qualifiedName() != "DirectoryOfExport")
            throw Exception("Fichier corrompu : DirectoryOfExport était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        dir_pdf = xml->readElementText();

        // devise

        xml->readNextStartElement();
        if(xml->qualifiedName() != "Devise")
            throw Exception("Fichier corrompu : Devise était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        devise = xml->readElementText();

        // devise_visible

        xml->readNextStartElement();
        if(xml->qualifiedName() != "DeviseVisible")
            throw Exception("Fichier corrompu : DeviseVisible était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        devise_visible = xml->readElementText().toShort(nullptr,2);

        // graphics_max_points

        xml->readNextStartElement();
        if(xml->qualifiedName() != "GraphicsMaxPoints")
            throw Exception("Fichier corrompu : GraphicsMaxPoints était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
        graphics_max_points = xml->readElementText().toUInt();

        if(__version >= "Abricot (0.2.2)"){
            // format

            xml->readNextStartElement();
            if(xml->qualifiedName() != "FormatDate")
                throw Exception("Fichier corrompu : FormatDate était attendu, mais '"+xml->qualifiedName()+"' a été rencontré");
            format_date = xml->readElementText();
        }// version Abricot 0.2.2
    }
    catch(Exception &e)
    {
        QWidget* m = new QWidget;
        QMessageBox::critical(m,"Chargement de la sauvegarde","Le fichier de sauvegarde est corrompu :\n"+e.getString());
        delete m;
        abort();
    }
    fichier.close();
}

bool Parameters::openHelp() const
{
    return QDesktopServices::openUrl(url_help);
}

QString const& Parameters::getAppName() const
{
    return app_name;
}

QString const& Parameters::getAppLongName() const
{
    return app_long_name;
}

QString const& Parameters::getAppVersion() const
{
    return app_version;
}

QString const& Parameters::getDevise() const
{
    return devise;
}

bool Parameters::getDeviseVisible() const
{
    return devise_visible;
}

QString const& Parameters::getDirPDF() const
{
    return dir_pdf;
}

QString const& Parameters::getDirRegistry() const
{
    return dir_registry;
}

QString const& Parameters::getFileSave() const
{
    return file_save;
}

QString const& Parameters::getFormatDate() const
{
    return format_date;
}

unsigned Parameters::getGraphicsMaxPoints() const
{
    return graphics_max_points;
}

void Parameters::save()
{
    QFile fichier(cfg_save);
    if(!fichier.open(QIODevice::WriteOnly)){
        fichier.close();
        throw Exception("impossible d'ouvrir le fichier");
    }
    QXmlStreamWriter* xml = new QXmlStreamWriter(&fichier);

    xml->setCodec("UTF-8");
    xml->setAutoFormatting(true);

    xml->writeStartDocument();
    xml->writeComment("\n\t@file\n\t\n\t@brief Configuration of "+app_name+" software\n\t@version "+app_version+"\n\t@author Jérémy Godde\n\t@copyright GNU GPL 3.0\n");
    xml->writeDTD("<!DOCTYPE xml>");
    xml->writeStartElement("Configuration");

    xml->writeTextElement("Version",app_version);
    xml->writeTextElement("Filesave",file_save);
    xml->writeTextElement("DirectoryOfRegistry",dir_registry);
    xml->writeTextElement("DirectoryOfExport",dir_pdf);
    xml->writeTextElement("Devise",devise);
    xml->writeTextElement("DeviseVisible",QString::number(devise_visible));
    xml->writeTextElement("GraphicsMaxPoints",QString::number(graphics_max_points));
    xml->writeTextElement("FormatDate",format_date);

    xml->writeEndElement();
    xml->writeEndDocument();

    fichier.close();
}
