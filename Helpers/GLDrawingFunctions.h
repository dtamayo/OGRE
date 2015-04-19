/*!
 @file GLDrawingFunctions.h
 @brief Provides support for drawing objects in OpenGL.  It is used only by OrbitalAnimator.
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

#ifndef EXTERNAL_FUNCTIONS_H
#define EXTERNAL_FUNCTIONS_H

#include <vector>
#include "Point3d.h"
//#include "Transforms.hpp"
#include <cmath>
#include <QColor>
#include <Geometry>
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#include <OpenGl/gl.h>
#endif

#define X_ 0
#define Y_ 1
#define Z_ 2

inline double degToRads(double deg) { return deg * M_PI / 180.; }
inline double radsToDeg(double rads) { return rads * 180. / M_PI; }

void drawsphere(int sectors, int rings, int radius);

class Sphere
{
public:
	Sphere(int sectors, int rings, int radius);
	void updateRadius(int radius);
	void draw();

private:
	std::vector<float> dat;
	std::vector<int> idx;
	int ntri;
	int sectors;
	int rings;
	int radius;
};

class Cone
{
public:
	Cone(
        const Point3d &d,
        const Point3d &a,
		const double h,
		const double rd,
		const int n);

	void draw();

private:
    std::vector<Point3d> topVertices;
    std::vector<Point3d> fanVertices;
};

void drawCoords(GLfloat extent);
void drawVectorCoords(GLfloat extent);
void drawVector(Point3d const& v);
void drawOrbitalRing(std::vector<Point3d> const& vertices);
void drawPlane(QColor const& color, Point3d const& z, GLfloat extent);
void drawXYPlane(GLfloat extent);
void drawXZPlane(GLfloat extent);
void drawYZPlane(GLfloat extent);

#endif
