/*!
 @file SwiftReader.cpp
 @brief Reads in SWIFT files. Called by OrbitalAnimationDriver. Does not depend on any other classes.
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

#include "SwiftReader.h"

#define SEPARATOR "\\s*"
#define DECIMAL_FIELD_REXP "([0-9\\.eE\\-\\+]*)"

SwiftReader::SwiftReader(QString filename)
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

void SwiftReader::readResults(QTextStream& stream)
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
                os << "Could not decode decimal " << decimals.at(index).toAscii().data(); \
                throw std::runtime_error(os.str()); \
                }
            d.time = decimals.at(1).toDouble(&ok); HANDLE_ERROR(1);
            d.particleID = decimals.at(2).toDouble(&ok); HANDLE_ERROR(2);
            d.axis = decimals.at(3).toDouble(&ok) * 25559; HANDLE_ERROR(3);
            d.e = decimals.at(4).toDouble(&ok); HANDLE_ERROR(4);
            d.i = decimals.at(5).toDouble(&ok); HANDLE_ERROR(5);
            d.Omega = decimals.at(6).toDouble(&ok); HANDLE_ERROR(6);
            d.w = decimals.at(7).toDouble(&ok); HANDLE_ERROR(7);
            d.f = decimals.at(8).toDouble(&ok); HANDLE_ERROR(8);
            d.hasOrbEls = true;
            data[d.particleID].push_back(d);
        }
    }
}
