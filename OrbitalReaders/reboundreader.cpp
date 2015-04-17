/*!
 @file ReboundReader.cpp
 @brief Reads in output from REBOUND. Called by OrbitalAnimationDriver. Does not depend on any other classes.
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

#include "reboundreader.h"

#define SEPARATOR "\\s*"
#define DECIMAL_FIELD_REXP "([0-9\\.eE\\-\\+]*)"

ReboundReader::ReboundReader(QString filename)
    : lineParser(
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP
        SEPARATOR DECIMAL_FIELD_REXP)
{
    if(filename.length() > 0)
    {
        QFile file(filename);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream stream(&file);
            readResults(stream);
            file.close();
        }
    }
}

void ReboundReader::readResults(QTextStream& stream)
{
    QString line;
    int lineNum = 0;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        ++lineNum;
        if (lineParser.exactMatch(line))
        {
            QStringList decimals = lineParser.capturedTexts();
            Orbit d;
            bool ok = true;
            #define HANDLE_ERROR(index) \
                if (!ok) { \
                std::ostringstream os; \
                os << "Could not decode decimal " << decimals.at(index).toLatin1().data(); \
                throw std::runtime_error(os.str()); \
                }
            d.particleID = decimals.at(1).toDouble(&ok); HANDLE_ERROR(2);
            d.time = decimals.at(2).toDouble(&ok); HANDLE_ERROR(1);
            d.axis = decimals.at(3).toDouble(&ok); HANDLE_ERROR(3);
            d.e = decimals.at(4).toDouble(&ok); HANDLE_ERROR(4);
            d.i = 180./M_PI*decimals.at(5).toDouble(&ok); HANDLE_ERROR(5);
            d.Omega = 180./M_PI*decimals.at(6).toDouble(&ok); HANDLE_ERROR(6);
            d.w = 180./M_PI*decimals.at(7).toDouble(&ok); HANDLE_ERROR(7);
            d.l = 180./M_PI*decimals.at(8).toDouble(&ok); HANDLE_ERROR(8);
            d.P = decimals.at(9).toDouble(&ok); HANDLE_ERROR(9);
            d.f = 180./M_PI*decimals.at(10).toDouble(&ok); HANDLE_ERROR(10);
            d.hasOrbEls = true;
            data[d.particleID].push_back(d);
        }
    }
}

