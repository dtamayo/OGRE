/*!
 @file OrbitalDataCSVReader.cpp
 @brief Reads in .csv files. Called by OrbitalAnimationDriver. Does not depend on any other classes.
 Parsing is done using QRegExp.

 @section LICENSE

 Copyright (c) 2013 Robert Douglas, Heming Ge, Daniel Tamayo
 Copyright (c) 2012 Robert Douglas

 This file is part of OGRE.

 OGRE is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 OGRE is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with OGRE.  If not, see <http://www.gnu.org/licenses/>.

 The original code for this project was developed by Robert Douglas.
 This version is derived from Robert Douglas's
 repository at https://www.assembla.com/profile/rwdougla revision 29.
 The copyright notice from the original code is given below:

 Copyright (c) 2012 Robert Douglas
 Distributed under the accompanying Software License, Version 1.0.
 (See accompanying file LICENSE_ORIGINAL.txt or copy at
 https://subversion.assembla.com/svn/rob_douglas_sandbox/trunk/license.txt)
*/

#include "OrbitalDataCSVReader.h"

#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

OrbitalDataCSVReader::OrbitalDataCSVReader(QString filename)
{
    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);

        QString line;
        int lineNum = 0;
        while (!stream.atEnd())
        {
            line = stream.readLine();
            ++lineNum;
            QStringList parts = line.split(',');
            if (parts.size() >= 11)
            {
                StaticDisplayOrbit newOrbit;
                newOrbit.name = parts[0];
                newOrbit.axis = parts[1].toDouble();
                newOrbit.e = parts[2].toDouble();
                newOrbit.i = parts[3].toDouble();
                newOrbit.Omega = parts[4].toDouble();
                newOrbit.w = parts[5].toDouble();
                newOrbit.frameStart = parts[6].toInt();
                newOrbit.frameEnd = parts[7].toInt();
                newOrbit.red = parts[8].toInt();
                newOrbit.green = parts[9].toInt();
                newOrbit.blue = parts[10].toInt();
                newOrbit.f = 0;
                newOrbit.time = 0;
                orbits.push_back(newOrbit);
            }
        }

        file.close();
    }
}
