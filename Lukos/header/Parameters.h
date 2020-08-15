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

class Parameters : public QObject
{
    Q_OBJECT
protected:
    QString const cfg_save = "config.xml";

    QString app_name;
    QString app_long_name;
    QString app_version;
    QString devise;
    bool devise_visible;
    QString dir_registry;
    QString dir_pdf;
    QString file_save;
    unsigned graphics_max_points;

public:

    Parameters();
    ~Parameters() = default;

    QString const& getAppName() const;
    QString const& getAppLongName() const;
    QString const& getAppVersion() const;
    QString const& getDevise() const;
    bool getDeviseVisible() const;
    QString const& getDirRegistry() const;
    QString const& getDirPDF() const;
    QString const& getFileSave() const;
    unsigned getGraphicsMaxPoints() const;

    void save();

public slots:
    void changeDevise(QString _devise) {devise = _devise;}
    void changeDeviseVisible(bool _visible){devise_visible = _visible;}
    void changeMaxPoints(unsigned _max_points) {graphics_max_points = _max_points;}
    void changeWriteSave(const QString _file){file_save = _file;}
    void changeWriteRegistry(const QString _dir){dir_registry = _dir;}
    void changeWritePDF(const QString _dir){dir_pdf = _dir;}
};

#endif // PARAMETERS_H
