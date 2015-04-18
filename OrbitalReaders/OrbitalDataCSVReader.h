/*!
 @file OrbitalDataCSVReader.h
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

#ifndef ORBITAL_DATA_CSV_READER_H
#define ORBITAL_DATA_CSV_READER_H

#include "Helpers/Orbit.h"

#include <QtCore/QString>

#include <vector>

class OrbitalDataCSVReader
{
public:
    OrbitalDataCSVReader(QString filename);
    StaticDisplayOrbits const& getOrbits() const { return orbits; }

private:
    StaticDisplayOrbits orbits;
};

#endif
