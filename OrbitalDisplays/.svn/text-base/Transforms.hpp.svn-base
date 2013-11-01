//          Copyright Robert Douglas 2012
// Distributed under the accompanying Software License, Version 1.0.
//    (See accompanying file license.txt or copy at
//          https://subversion.assembla.com/svn/rob_douglas_sandbox/trunk/license.txt)
#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "Matrix.hpp"

#include <cmath>

// based on write-up from:
// http://cse.taylor.edu/~btoll/s99/424/res/mtu/Notes/geometry/geo-tran.htm
namespace Math
{
	typedef Matrix<4, 4> Matrix3D;

	Matrix3D rotateAboutX(double radians);
	Matrix3D rotateAboutY(double radians);
	Matrix3D rotateAboutZ(double radians);
	Matrix3D translate(double dx, double dy, double dz);
	Matrix3D scale(double pctX, double pctY, double pctZ);

	inline void setToIdentity(Matrix3D& m)
	{
		for (unsigned int i = 0; i < Matrix3D::NRows; ++i)
		{
			for (unsigned int j = 0; j < Matrix3D::NColumns; ++j)
			{
				m[i * Matrix3D::NColumns + j]
					= i == j
					? 1
					: 0;
			}
		}
	}
	inline Matrix3D rotateAboutX(double radians)
	{
		Matrix3D m;
		setToIdentity(m);
		m[5] = cos(radians);
		m[6] = -sin(radians);
		m[9] = sin(radians);
		m[10] = cos(radians);
		return m;
	}
	inline Matrix3D rotateAboutY(double radians)
	{
		Matrix3D m;
		setToIdentity(m);
		m[0] = cos(radians);
		m[2] = sin(radians);
		m[8] = -sin(radians);
		m[10] = cos(radians);
		return m;
	}
	inline Matrix3D rotateAboutZ(double radians)
	{
		Matrix3D m;
		setToIdentity(m);
		m[0] = cos(radians);
		m[1] = -sin(radians);
		m[4] = sin(radians);
		m[5] = cos(radians);
		return m;
	}
	inline Matrix3D translate(double dx, double dy, double dz)
	{
		Matrix3D m;
		setToIdentity(m);
		m[3] = dx;
		m[7] = dy;
		m[11] = dz;
		return m;
	}
	inline Matrix3D scale(double pctX, double pctY, double pctZ)
	{
		Matrix3D m;
		setToIdentity(m);
		m[0] = pctX;
		m[5] = pctY;
		m[10] = pctZ;
		return m;
	}
} // namespace Math

#endif
