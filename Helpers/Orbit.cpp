/*!
* @file Orbit.cpp
 @brief Implementation for the Orbit class, which holds data for defining an orbit and functions to convert from osculating to cartesian descriptions.
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

#include "Orbit.h"
#include <QtCore/QDebug>
#include <Geometry>
#include <Dense>
#include <math.h>

#define PRECISION 1e-14

void Orbit::convertOrbElsToPos(Point3d& v, double* cosfs, double* sinfs, double f) {
    double radius = axis * (1 - e * e) / (1 + e * cosfs[(size_t)f]);
    v.x = radius * cosfs[(size_t)f];
    v.y = radius * sinfs[(size_t)f];
    v.z = 0;
}

void Orbit::calculatePosition(double* cosfs, double* sinfs) {
    convertOrbElsToPos(posInPlane, cosfs, sinfs, f); hasCoords = true;
}

void Orbit::calculateOrbit(double* cosfs, double* sinfs) {
    orbitCoords.resize(360);
    for (size_t i = 0; i < orbitCoords.size(); ++i) convertOrbElsToPos(orbitCoords[i], cosfs, sinfs, i);
    posInPlane.x = orbitCoords[f].x;
    posInPlane.y = orbitCoords[f].y;
    posInPlane.z = orbitCoords[f].z;
}

void Orbit::checkElements()
{
    if(mu < 0)
    {
        qDebug() << "mu must be >= 0";
        exit(1);
    }

    if(axis < 0)
    {
        qDebug() << "a must be >= 0";
        exit(1);
    }
    if(e < 0 || e >= 1)
    {
        qDebug() << "e must be between 0 and 1 (and can't equal 1)";
        exit(1);
    }

    if(i < 0 || i > 180)
    {
        qDebug() << "i must be within 0 and 180 degrees.";
        exit(1);
    }

    return;
}

void Orbit::osc2xyz()
{
    // Do checks to make sure elements are valid

    checkElements();

    // Change angles to radians

    f = DegToRad(f);
    w = DegToRad(w);
    Omega = DegToRad(Omega);
    i = DegToRad(i);

    // Now find r

    double rmag = axis*(1 - pow(e, 2))/(1 + e*cos(f));

    // and rhat

    Eigen::Vector3d rhat;

    rhat(0) = cos(w + f)*cos(Omega) - cos(i)*sin(Omega)*sin(w + f);
    rhat(1) = cos(w + f)*sin(Omega) + cos(i)*cos(Omega)*sin(w + f);
    rhat(2) = sin(i)*sin(w + f);

    // We can now find x, y, z by multiplying the scalar r by rhat

    r = rmag*rhat;

    // find the components of the angular momentum unit vector

    Eigen::Vector3d hhat(sin(Omega)*sin(i), -cos(Omega)*sin(i), cos(i));

    // and the total angular momentum

    double h = sqrt(mu*axis*(1 - pow(e, 2)));

    // Calculate components of theta unit vector from thetahat = h hat cross r hat

    Eigen::Vector3d thetahat = hhat.cross(rhat);

    // Finally calculate rdot and thetadot in order to calculate v's

    double thetadot = h/pow(rmag, 2); // theta is azimuthal angle in plane of orbit
    double rdot = e*mu*sin(f)/h;	// rdot in plane of orbit

    // v = rthetadot thetahat + rdot rhat.  Just need the components though:
    v = rmag*thetadot*thetahat + rdot*rhat;

    return;
}

void Orbit::xyz2osc()
{
	Eigen::Vector3d h = r.cross(v);

	//DOESN'T CONSIDER CASE OF RADIAL ORBIT i.e. h = 0

	// Now calculate components of P = (v^2r - mu)rhat - (v dot r)v
	// First calculate A = (v^2r - mu)/r = v^2 - mu/r and B = v dot r

	double A = v.squaredNorm() - mu / r.norm();

	double B = v.dot(r);

	Eigen::Vector3d P = A*r - B*v;

	e = P.norm()/mu;

	axis = h.squaredNorm() / ( mu * (1 - pow(e, 2)));

	i = acos(h(2)/h.norm());

	i = RadToDeg(i);// convert to degrees

	if(fabs(i) < PRECISION)
	{
		Omega = 0;
    }
    else
    {
        Omega = atan2(h(1), h(0));
        Omega += M_PI/2;			// atan2(hy,hx) = phi = spherical angle for h
                                    // Omega = phi + 90 deg.
    }

    if(Omega < 0)
    {
        Omega += 2*M_PI;
    }

    double C = cos(Omega)*P(0) + sin(Omega)*P(1);

    Omega = RadToDeg(Omega);

    double temp = C/(mu*e);
    temp = std::max(temp, -1.);
    temp = std::min(temp, 1.);

    w = acos(temp);

    if(P(2) < 0)
    {
        w = -w;
    }

    if(fabs(i) < PRECISION)
    {
        w = atan2(P(1), P(0));
    }

    else
    {
        if(fabs(P(2)) < PRECISION)
        {
            w = acos(temp);
        }
    }

    if(w < 0)
    {
        w += 2*M_PI;
    }

    w = RadToDeg(w);

    temp = (axis*(1-pow(e, 2))/r.norm() - 1)/e;
    temp = std::max(temp, -1.);
    temp = std::min(temp, 1.);

    f = acos(temp);
    f = RadToDeg(f);

    // We can disambiguate angles by seeing whether v dot r (=B) is positive or negative

    if(B < 0)
    {
        f = -f;
    }

    if(f < 0)
    {
        f += 360;
    }

    if(e < PRECISION)
    {
        e = 0;
    }

    return;
}

