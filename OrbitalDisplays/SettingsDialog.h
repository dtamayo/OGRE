/*!
 @file SettingsDialog.h
 @brief class definition for Settings Dialog, the column of boxes and sliders on the right of the window.

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

#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QtWidgets/QWidget>

#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>

#include "Settings.h"

/*! @brief main namespace for the project
*/
namespace Disp
{
    /*! @brief Class for the column of boxes and sliders on the right of the main window

        The class has public member widgets (of type QDoubleSpinBox and QSlider) for each of the items appearing in the settings dialog
        on the right of the application window.

        For an example of how to add/edit a new button, see @ref modsetdiag.
    */
    class SettingsDialog : public QWidget
    {
        Q_OBJECT
    public:
        SettingsDialog(OrbitalAnimatorSettings &animatorSettings_, QWidget *parent = 0);

        /*! @brief widget to rotate the view around the x axis
        */
        QDoubleSpinBox* xRotationBox;
        /*! @brief widget to rotate the view around the y axis
        */
        QDoubleSpinBox* yRotationBox;
        /*! @brief widget to rotate the view around the z axis
        */
        QDoubleSpinBox* zRotationBox;

        QSlider* zoomScaleBox;

        //QSlider* scrollZoom;

        QDoubleSpinBox* rotateAmountX;
        QDoubleSpinBox* rotateAmountY;
        QDoubleSpinBox* rotateAmountZ;

        QSpinBox* rotateSpeed;

        QDoubleSpinBox* zoomAmount;

        QSpinBox* zoomSpeed;

        QSpinBox* simulateAmount;

        QSpinBox* simulateSpeed;

        QSlider* scrollTimeIndex;

        QSpinBox* timeIndex;

        int getTimeStep() const { return timeStep->value(); }
        void setFrame(int fn); 
        void setFrameRange(int fr);

    signals:

        void setCurrentIndex(int);

        void setXRot(double);

        void setYRot(double);

        void setZRot(double);

        void setZoomFactor(int);

        void rotate();

        void zoom();

        void simulate();

        void handleAnimateChecked(bool);

    public slots:

        void advanceTime();

    private slots:

        void selectCentralBodyColor();

        void selectOrbitalPlaneColor();

        void selectOrbitColor();

        void setSliderValue(int val) { scrollTimeIndex->setValue(val); }

        void setBoxValue(int val) { timeIndex->setValue(val); }

    private:

        OrbitalAnimatorSettings& animatorSettings;

        QCheckBox* animate;

        QSpinBox* timeStep;

        QPushButton* centralBodyColorSelector;

        QPushButton* orbitalPlaneColorSelector;

        QPushButton* orbitColorSelector;

        QPushButton* rotator;

        QPushButton* zoomer;

        QPushButton* simulator;

        void setupUI();
        void layoutControls();
        void makeConnections();
    };
} // namespace RobD

#endif
