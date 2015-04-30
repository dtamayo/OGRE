/*!
 @file Settings.h
 @brief Defines all the settings for the OrbitalAnimator

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

 @see SettingsDialog
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QObject>
#include <QtGui/QColor>

namespace Disp
{
    class OrbitalAnimatorSettings : public QObject
    {
        Q_OBJECT
    public:
        OrbitalAnimatorSettings(QObject* parent = 0)
            : QObject(parent)
            , mDisplayOverlays(false)
            , mDisplayCoords(true)
            , mDisplayMainOrbit(true)
            , mDisplaySpinAxis(true)
            , mDisplayMouseTracking(true)
            , mDisplayCentralBody(true)
            , mDisplayFrameNumber(false)
            , mDisplayVecX(true)
            , mCentralBodyColor(0x8A, 0x41, 0x17, 0xFF)
            , mOrbitalPlaneColor(0x56, 0xA5, 0xEC, 0x80)
            , mOrbitColor(0x00, 0xFF, 0x00, 0xFF)//0x4A, 0xA0, 0x2C, 0xFF)
        {}

        bool displayOverlays() const { return mDisplayOverlays; }
        bool displayCoords() const { return mDisplayCoords; }
        bool displayMainOrbit() const { return mDisplayMainOrbit; }
        bool displaySpinAxis() const { return mDisplaySpinAxis; }
        bool displayMouseTracking() const { return mDisplayMouseTracking; }
        bool displayCentralBody() const { return mDisplayCentralBody; }
        bool displayFrameNumber() const { return mDisplayFrameNumber; }
        bool displayVecX() const { return mDisplayVecX; }

        QColor centralBodyColor() const { return mCentralBodyColor; }
        QColor orbitalPlaneColor() const { return mOrbitalPlaneColor; }
        QColor orbitColor() const { return mOrbitColor; }

    public slots:
        void setDisplayOverlays(bool val) { mDisplayOverlays = val; changed(); }
        void setDisplayCoords(bool val) { mDisplayCoords = val; changed(); }
        void setDisplayMainOrbit(bool val) { mDisplayMainOrbit = val; changed(); }
        void setDisplaySpinAxis(bool val) { mDisplaySpinAxis = val; changed(); }
        void setDisplayMouseTracking(bool val) { mDisplayMouseTracking = val; changed(); }
        void setDisplayCentralBody(bool val) { mDisplayCentralBody = val; changed(); }
        void setDisplayFrameNumber(bool val) { mDisplayFrameNumber = val; changed(); }
        void setDisplayVecX(bool val){ mDisplayVecX = val; changed(); }
        void setCentralBodyColor(const QColor& val) { mCentralBodyColor = val; changed(); }
        void setOrbitalPlaneColor(const QColor& val) { mOrbitalPlaneColor = val; changed(); }
        void setOrbitColor(const QColor& val) { mOrbitColor = val; changed(); }

    signals:
        void changed();

    private:
        bool mDisplayOverlays;
        bool mDisplayCoords;
        bool mDisplayMainOrbit;
        bool mDisplaySpinAxis;
        bool mDisplayMouseTracking;
        bool mDisplayCentralBody;
        bool mDisplayFrameNumber;
        bool mDisplayVecX;
        QColor mCentralBodyColor;
        QColor mOrbitalPlaneColor;
        QColor mOrbitColor;
        int xrot;
        int yrot;
        int zrot;
    };
} // namespace RobD

#endif
