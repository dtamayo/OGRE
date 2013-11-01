/*!
 @file OrbitalAnimationDriver.h
 @brief Class definition for Disp::OrbitalAnimationDriver, which acts as a go-between for the Disp::MainWindow and Disp::OrbitalAnimator classes.

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

#ifndef ORBITAL_ANIMATION_DRIVER_H
#define ORBITAL_ANIMATION_DRIVER_H

#include <QtGui/QWidget>

#include <QtCore/QTimer>

#include "OrbitalAnimator.h"
#include "OrbitalReaders/DIReader.h"
#include "OrbitalReaders/OrbitalDataCSVReader.h"
#include "OrbitalReaders/SwiftReader.h"
#include "Settings.h"
#include "SettingsDialog.h"

#include <QtGui/QApplication>
#include <QtGui/QCheckBox>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtOpenGL/QGLFramebufferObject>

#define FPS 24 // frames per second to be displayed in the application

namespace Disp
{
    class OrbitalAnimator;
    class SliderSpinBoxPair; // should be removed
    /*!
        @brief Acts as a go-between for Disp::MainWindow and Disp::OrbitalAnimator that displays the orbits.

        This interfacing class makes it easier to do automatic tests on Disp::OrbitalAnimator (not implemented) independent of the rest of the program.
        It also makes Disp::OrbitalAnimator and Disp::Mainwindow more compartamentalized and therefore more easily reusable for other applications.

        The class instantiates a Disp::OrbitalAnimator (private variable orbitalAnimator) that displays the orbits.  Almost all of
        Disp::OrbitalAnimationDriver 's member functions are public so that Disp::MainWindow can call them.  Disp::OrbitalAnimationDriver then passes on
        those commands to orbitalAnimator.  This class is also used to call the various readers to read in the appropriate files. It also makes connections
        between Disp::SettingsDialog (the private variable controlsWidget) and orbitalAnimator so that the user's rotation, zoom etc. choices can be passed
        to the display.  Finally, it has a public member variable of type Disp::OrbitalAnimatorSettings (animatorSettings) defined in Settings.h.  This way
        Disp::MainWindow can directly alter the settings when the user changes them.

        The application's Disp::OrbitalAnimationDriver is instantiated in Disp::MainWindow::MainWindow().
    */
    class OrbitalAnimationDriver : public QWidget
    {
        Q_OBJECT
    public:
        void setViewableArea(int w, int h);
        void setRotation(int xRot, int yRot, int zRot);
        void showFrame(int frameNumber);
        void setZoom(double zoomPercent);
        QWidget* setupUI();
        void layoutControls();
        void makeConnections();
        void setSimulationData(QString filename, QString fileType, QString dataType, bool b);
        void setEquatorialData(QString equatorialFName);
        void setEclipticData(QString eclipticFName);
        void clearEquatorialData();
        void clearEclipticData();
        void clearSimulationData();
        void clearAllData();
        void playbackQueue(QTableWidget* queue);
        void record(QTableWidget* queue);
        std::vector<double> getState();
        int getSimulationSize();

        OrbitalAnimatorSettings animatorSettings;

    public slots:
        void performAction(QTableWidgetItem* a);

    private slots:
        void handleAnimateChecked(bool val);
        //void savePics(); not used

    private:
        OrbitalAnimator* orbitalAnimator;
        QWidget* controlsWidget;
        QTimer animationTimer;
    };
} // namespace RobD

#endif
