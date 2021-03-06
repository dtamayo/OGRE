/*!
 @file DIReader.h
 @brief Header for the class DIFile. This class parses and stores data from .dI files.
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

#ifndef DIREADER_H
#define DIREADER_H

#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "Helpers/Point3d.h"
#include "Helpers/Orbit.h"

class DIReader
{
public:
    DIReader(QString filename);
    OrbitData const& getData() const { return data; }

private:
    void skipToResults(QTextStream& stream);
    void readResults(QTextStream& stream);

    QRegExp dataStart;
    QRegExp lineParser;
    OrbitData data;
};

#endif // DIREADER_H
