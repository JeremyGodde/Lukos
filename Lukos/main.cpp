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

#include "principale.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QTranslator qtTranslator;
         qtTranslator.load("qt_" + QLocale::system().name(),
                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QApplication a(argc, argv);
    a.installTranslator(&qtTranslator);

    Parameters* parameter = new Parameters;
    Principale w(*parameter);
    w.show();

    return a.exec();
}
