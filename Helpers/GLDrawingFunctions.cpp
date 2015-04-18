/*!
 @file GLDrawingFunctions.cpp
 @brief This file contains functions mainly used for drawing objects in OpenGL.
 It is used only by OrbitalAnimator.
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

#include "GLDrawingFunctions.h"

// Modified from http://www.gamedev.net/topic/537269-procedural-sphere-creation/
void drawsphere(int sectors, int rings, int radius)
{
	float theta, phi;
	int i, j, t;

	int nvec = (rings-2)* sectors+2;
	int ntri = (rings-2)*(sectors-1)*2;

	std::vector<float> dat(nvec * 3);
	std::vector<int> idx(ntri * 3);

	for (t=0, j=1; j < rings-1; ++j)
	{
		for (i=0; i < sectors; ++i)
		{
            theta = float(j)/(rings-1) * M_PI;
            phi   = float(i)/(sectors-1) * M_PI*2;
			dat[t++] =  sinf(theta) * cosf(phi);
			dat[t++] =  cosf(theta);
			dat[t++] = -sinf(theta) * sinf(phi);
		}
	}
	dat[t++]=0; dat[t++]= 1; dat[t++]=0;
	dat[t++]=0; dat[t++]=-1; dat[t++]=0;

	for( t=0, j=0; j<rings-3; j++ )
	{
		for(      i=0; i<sectors-1; i++ )
		{
			idx[t++] = (j  )*sectors + i  ;
			idx[t++] = (j+1)*sectors + i+1;
			idx[t++] = (j  )*sectors + i+1;
			idx[t++] = (j  )*sectors + i  ;
			idx[t++] = (j+1)*sectors + i  ;
			idx[t++] = (j+1)*sectors + i+1;
		}
	}
	for( i=0; i<sectors-1; i++ )
	{
		idx[t++] = (rings-2)*sectors;
		idx[t++] = i;
		idx[t++] = i+1;
		idx[t++] = (rings-2)*sectors+1;
		idx[t++] = (rings-3)*sectors + i+1;
		idx[t++] = (rings-3)*sectors + i;
	}

	glPushMatrix();
	glScalef(radius, radius, radius);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,&dat[0]);
	glNormalPointer(GL_FLOAT,0,&dat[0]);
	glDrawElements(GL_TRIANGLES, ntri*3, GL_UNSIGNED_INT, &idx[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glPopMatrix();
}

// Modified from http://www.gamedev.net/topic/537269-procedural-sphere-creation/
Sphere::Sphere(int sectors_, int rings_, int radius_)
	: sectors(sectors_)
	, rings(rings_)
{
	updateRadius(radius_);
}

// Modified from http://www.gamedev.net/topic/537269-procedural-sphere-creation/
void Sphere::updateRadius(int radius_)
{
	radius = radius_;
	float theta, phi;
	int i, j, t;

	int nvec = (rings-2)* sectors+2;
	ntri = (rings-2)*(sectors-1)*2;

	dat.resize(nvec * 3);
	idx.resize(ntri * 3);

	for (t=0, j=1; j < rings-1; ++j)
	{
		for (i=0; i < sectors; ++i)
		{
            theta = float(j)/(rings-1) * M_PI;
            phi   = float(i)/(sectors-1) * M_PI*2;
			dat[t++] =  sinf(theta) * cosf(phi);
			dat[t++] =  cosf(theta);
			dat[t++] = -sinf(theta) * sinf(phi);
		}
	}
	dat[t++]=0; dat[t++]= 1; dat[t++]=0;
	dat[t++]=0; dat[t++]=-1; dat[t++]=0;

	for( t=0, j=0; j<rings-3; j++ )
	{
        for( i=0; i<sectors-1; i++ )
		{
			idx[t++] = (j  )*sectors + i  ;
			idx[t++] = (j+1)*sectors + i+1;
			idx[t++] = (j  )*sectors + i+1;
			idx[t++] = (j  )*sectors + i  ;
			idx[t++] = (j+1)*sectors + i  ;
			idx[t++] = (j+1)*sectors + i+1;
		}
	}
	for( i=0; i<sectors-1; i++ )
	{
		idx[t++] = (rings-2)*sectors;
		idx[t++] = i;
		idx[t++] = i+1;
		idx[t++] = (rings-2)*sectors+1;
		idx[t++] = (rings-3)*sectors + i+1;
		idx[t++] = (rings-3)*sectors + i;
	}
}

// Modified from http://www.gamedev.net/topic/537269-procedural-sphere-creation/
void Sphere::draw()
{
	glPushMatrix();
	glScalef(radius, radius, radius);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,&dat[0]);
	glNormalPointer(GL_FLOAT,0,&dat[0]);
	glDrawElements(GL_TRIANGLES, ntri*3, GL_UNSIGNED_INT, &idx[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glPopMatrix();
}

// Based on http://www.freemancw.com/2012/06/opengl-cone-function/
const Point3d perp(const Point3d &v)
{
	double min = fabs(v.x);
    Point3d cardinalAxis(1.0, 0.0, 0.0);
    if (fabs(v.y) < min)
    {
        min = fabs(v.y);
        cardinalAxis = Point3d(0.0, 1.0, 0.0);
    }
    if (fabs(v.z) < min)
    {
        cardinalAxis = Point3d(0.0, 0.0, 1.0);
    }

    return crossProduct(v, cardinalAxis);
}

// Based on http://www.freemancw.com/2012/06/opengl-cone-function/
Cone::Cone(
    const Point3d &d,
    const Point3d &a,
	const double h,
	const double rd,
	const int n)
{
    Point3d c = a + (-d * h);
    Point3d e0 = perp(d);
    Point3d e1 = crossProduct(e0, d);
    double angInc = (360.0 / n) * M_PI / 180.;

    // draw cone top
	topVertices.push_back(a);
    for(int i = 0; i <= n; i++)
    {
        double rad = angInc * i;
        Point3d p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		topVertices.push_back(p);
    }

    // draw cone bottom
	fanVertices.push_back(c);
    for(int i = n-1; i >= 0; i--)
    {
        double rad = angInc * i;
        Point3d p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		fanVertices.push_back(p);
    }
}

// Based on http://www.freemancw.com/2012/06/opengl-cone-function/
void Cone::draw()
{
    glBegin(GL_TRIANGLE_FAN);
    for (std::vector<Point3d>::const_iterator it = topVertices.begin(), endIt = topVertices.end();
		it != endIt; ++it)
	{
		glVertex3f(it->x, it->y, it->z);
	}
	glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(250, 20, 20);
    for (std::vector<Point3d>::const_iterator it = fanVertices.begin(), endIt = fanVertices.end();
		it != endIt; ++it)
	{
		glVertex3f(it->x, it->y, it->z);
	}
	glEnd();
}

void drawVector(Point3d const& v) {
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(v.x, v.y, v.z);
    glEnd();
    Cone c(unitVectorFrom(v), v, magnitude(v) * .1, magnitude(v) * .03, 20);
    c.draw();
}

void drawVectorCoords(GLfloat extent) {
    glColor3f(1, 0, 0);
    Point3d x(extent, 0, 0);
    drawVector(x);

    glColor3f(0, 1, 0);
    Point3d y(0, extent, 0);
    drawVector(y);

    glColor3f(0, 0, 1);
    Point3d z(0, 0, extent);
    drawVector(z);
}

void drawCoords(GLfloat extent) {
    glColor4f(1, 0, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(extent, 0, 0);
    glEnd();

    glColor4f(0, 0, 1, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, extent, 0);
    glEnd();

    glColor4f(0, 1, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, extent);
    glEnd();
}

void drawOrbitalRing(std::vector<Point3d> const& vertices) {
    glBegin(GL_LINE_STRIP);
    for (int f = 0; f < 360; ++f) glVertex3f(vertices[f].x, vertices[f].y, vertices[f].z);
    glVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
    glEnd();
}

void drawPlane(QColor const& color, Point3d const& z, GLfloat extent) {
    glPushMatrix();
    glColor4f(color.red() / 255., color.green() / 255., color.blue() / 255., color.alpha() / 255.);

    // pass 0 vector for xnew cuz we don't care about the orientation of x within the plane
    RotationAngles angles = angularMapping(z, Point3d(0, 0, 0));

    glRotatef(-90, 1, 0, 0); // first rotate so that openGL coordinates become normal
    glRotatef(-90, 0, 0, 1);
    glRotatef(radsToDeg(angles.phi), 0, 0, 1); // now do the three rotations to line up the axes
    glRotatef(radsToDeg(angles.theta), 0, 1, 0);
    glRotatef(radsToDeg(angles.psi), 0, 0, 1);

    glBegin(GL_QUADS);
    glVertex3f(-extent, -extent,0);
    glVertex3f(-extent, extent,0);
    glVertex3f(extent, extent,0);
    glVertex3f(extent, -extent,0);
    glEnd();
    glPopMatrix();
}

void drawXZPlane(GLfloat extent) { drawPlane(QColor::fromRgb(0, 255, 0, 32), Point3d(0, 1, 0), extent); }
void drawYZPlane(GLfloat extent) { drawPlane(QColor::fromRgb(255, 0, 0, 32), Point3d(1, 0, 0), extent); }
void drawXYPlane(GLfloat extent) { drawPlane(QColor::fromRgb(255, 0, 0, 64), Point3d(0, 0, 1), extent); }
