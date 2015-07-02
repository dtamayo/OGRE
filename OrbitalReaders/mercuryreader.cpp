/*!
 @file MercuryReader.cpp
 @brief Reads in output from Mercury. Called by OrbitalAnimationDriver. Does not depend on any other classes.
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

#include "MercuryReader.h"

#define SEPARATOR "\\s*"
#define DECIMAL_FIELD_REXP "([0-9\\.eE\\-\\+]*)"

MercuryReader::MercuryReader(QString filename, QString dataType)
    : lineParser(
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
            if(QString::compare(dataType,QString("xyz"),Qt::CaseInsensitive) == 0){
                readXYZ(stream, 0);
            }
            else{
                readOsc(stream, 0);
            }
            file.close();
        }
    }
}

void MercuryReader::readOsc(QTextStream& stream, int ID)
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
            d.time = decimals.at(1).toDouble(&ok); HANDLE_ERROR(1);
            d.axis = decimals.at(2).toDouble(&ok); HANDLE_ERROR(3);
            d.e = decimals.at(3).toDouble(&ok); HANDLE_ERROR(4);
            d.i = decimals.at(4).toDouble(&ok); HANDLE_ERROR(5);
            d.Omega = decimals.at(5).toDouble(&ok); HANDLE_ERROR(6);
            d.w = decimals.at(6).toDouble(&ok); HANDLE_ERROR(7);
            d.f = decimals.at(7).toDouble(&ok); HANDLE_ERROR(10);
            d.hasOrbEls = true;
            data[ID].push_back(d);
        }
    }
}

void MercuryReader::readXYZ(QTextStream& stream, int ID)
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
            d.time = decimals.at(1).toDouble(&ok); HANDLE_ERROR(1);
            d.r[0] = decimals.at(2).toDouble(&ok); HANDLE_ERROR(3);
            d.r[1] = decimals.at(3).toDouble(&ok); HANDLE_ERROR(4);
            d.r[2] = decimals.at(4).toDouble(&ok); HANDLE_ERROR(5);
            d.v[0] = decimals.at(5).toDouble(&ok); HANDLE_ERROR(6);
            d.v[1] = decimals.at(6).toDouble(&ok); HANDLE_ERROR(7);
            d.v[2] = decimals.at(7).toDouble(&ok); HANDLE_ERROR(8);
            d.hasOrbEls = false;
            d.posInPlane.x = d.r[0];
            d.posInPlane.y = d.r[1];
            d.posInPlane.z = d.r[2];
            data[ID].push_back(d);
        }
    }
}


