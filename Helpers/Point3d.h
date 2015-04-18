/*!
 @file Point3d.h
 @brief Header file for the vector class Point3d.  Provides functions for various vector operations.
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

#ifndef ORBITAL_INFO_H
#define ORBITAL_INFO_H

#ifdef WIN32
typedef unsigned long long uint64_t;
#else
typedef unsigned long long uint64_t;
#endif
#include <vector>
#include <map>
#include <Geometry>
#include <string>
#include <QtCore/QString>
#include <cmath>
#include "Transforms.hpp"

// Could be used as point with a time index, or if time = 1, is convenient
// definition for 3D point with ability to apply matrix transforms to it.
struct Point3d
{
    Point3d();
    Point3d(double x_, double y_, double z_);

    double x, y, z;

    static Point3d maxPoint();
    static Point3d minPoint();
    static Point3d centerOf(Point3d l, Point3d r);

    Point3d operator*(double scalar) const { return Point3d(x * scalar, y * scalar, z * scalar); }
    Point3d operator/(double scalar) const { return Point3d(x / scalar, y / scalar, z / scalar); }
    Point3d operator+(Point3d that) const { return Point3d(x + that.x, y + that.y, z + that.z); }
    Point3d operator-(Point3d that) const { return Point3d(x - that.x, y - that.y, z - that.z); }
    Point3d operator-() const { return Point3d(-x, -y, -z); }
};

struct Color
{
    Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char alpha_);
    Color();
    unsigned char r, g, b, alpha;
};

Point3d unitVectorFrom(Point3d const& v);
Point3d findMin(Point3d s, Point3d t);
Point3d findMax(Point3d s, Point3d t);

double magnitude(Point3d const& p);
Point3d crossProduct(Point3d const& l, Point3d const& r);
Point3d toUnitVector(Point3d const& orig);
double dotProduct(Point3d const& l, Point3d const& r);

struct RotationAngles { double theta; double phi; double psi; };
RotationAngles angularMapping(Point3d const& znew, Point3d const& xnew);
Math::Matrix3D rotationMatrixTo(Point3d const& origin, Point3d const& dest);

#endif
