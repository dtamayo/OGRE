/*!
* @file Orbit.h
 @brief Header file for the Orbit class, which holds data for defining an orbit and functions to convert from osculating to cartesian descriptions.
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

#ifndef ORBIT_H
#define ORBIT_H

#include <vector>
#include <map>
#include <cmath>
#include "Point3d.h"
#include <Geometry>
#include "GLDrawingFunctions.h"

/*! @brief Takes a double in degrees and returns a double in radians */
inline double DegToRad(double deg) { return(deg*M_PI/180); }

/*! @brief Takes a double in radians and returns a double in degrees */
inline double RadToDeg(double rad) { return(rad*180/M_PI); }

class Orbit {
public:
    Orbit() : hasCoords(false), hasOrbEls(false), color(1.0, 1.0, 0.0, 1.0), particleSize(0.005) {}
    void calculatePosition(double* cosfs, double* sinfs);
    void calculateOrbit(double* cosNus, double* sinfs);
    void convertOrbElsToPos(Point3d& v, double* cosfs, double* sinfs, double f);
    void xyz2osc();
    void osc2xyz();
    void checkElements();

    double time, particleID, axis, e, i, Omega, w, f;
    double mu;      // G * mass of central objects (needed in order to convert from xyz to osc)

    Eigen::Vector3d r;
    Eigen::Vector3d v;
    Point3d posInPlane;
    std::vector<Point3d> orbitCoords;

    bool hasCoords;
    bool hasOrbEls;
    Color color;
    double particleSize;

};

typedef std::map<int,std::vector<Orbit> > OrbitData;

class StaticDisplayOrbit : public Orbit {
public:
    QString name;
    int frameStart;
    int frameEnd;
    int red, green, blue;
};

typedef std::vector<StaticDisplayOrbit> StaticDisplayOrbits;

#endif // ORBIT_H
