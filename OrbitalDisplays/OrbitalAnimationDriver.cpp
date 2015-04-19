/*!
 @file OrbitalAnimationDriver.cpp
 @brief Implementation of the Disp::OrbitalAnimationDriver class, which acts as a go-between for the Disp::MainWindow and Disp::OrbitalAnimator classes.
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

#include "OrbitalAnimationDriver.h"

namespace Disp
{
    /*! @brief Sets up the UI for the Disp::OrbitalAnimator as well as the Disp::SettingsDialog.

        Called by Disp::MainWindow::MainWindow().
        Returns a QWidget* containing the Disp::SettingsDialog for Disp::MainWindow and initializes the Disp::OrbitalAnimator.
        Also sets the rate at which frames are displayed on the screen.

        @sa @ref Disp::OrbitalAnimationDriver::layoutControls(), Disp::OrbitalAnimationDriver::makeConections(),
        Disp::OrbitalAnimator::OrbitalAnimator(), Disp::OrbitalAnimator::setupUI()
      */
    QWidget* OrbitalAnimationDriver::setupUI()
    {
        animationTimer.setInterval(int(1000/FPS)); // sets how many milliseconds between frames
        orbitalAnimator = new OrbitalAnimator(animatorSettings);
        controlsWidget = orbitalAnimator->setupUI(animatorSettings);
        layoutControls();
        makeConnections();
        return controlsWidget;
    }

    /*! @brief Reads in an ecliptic data file.

        Calls Disp::OrbitalDataCSVReader::OrbitalDataCSVReader(), passing it the ecliptic data file name, which it gets when called by
        Disp::MainWindow::openEcliptic().

        @sa @ref Disp::OrbitalAnimator::setEclipticData(), Disp::OrbitalDataCSVReader::OrbitalDataCSVReader()
      */
    void OrbitalAnimationDriver::setEclipticData(QString eclipticFName) {
        orbitalAnimator->setLoading(true);
        orbitalAnimator->updateGL(); // makes display show the "Loading" message after the loading flag is set on previous line
        OrbitalDataCSVReader eclipticData(eclipticFName);
        orbitalAnimator->updateEclipticCache(eclipticData.getOrbits());
        orbitalAnimator->eclipticDataLoaded = true;
    }

    /*! @brief Reads in an equatorial data file.

        Calls Disp::OrbitalDataCSVReader::OrbitalDataCSVReader(), passing it the equatorial data file name, which it gets when called by
        Disp::MainWindow::openEquatorial().

        @sa @ref Disp::OrbitalAnimator::setEquatorialData(), Disp::OrbitalDataCSVReader::OrbitalDataCSVReader()
      */
    void OrbitalAnimationDriver::setEquatorialData(QString equatorialFName) {
        orbitalAnimator->setLoading(true);
        orbitalAnimator->updateGL(); // makes display show the "Loading" message after the loading flag is set on previous line
        OrbitalDataCSVReader equatorialData(equatorialFName);
        orbitalAnimator->updateEquatorialCache(equatorialData.getOrbits());
        orbitalAnimator->equatorialDataLoaded = true;
    }

    /*! @brief Reads in an input file.

        Calls the appropriate <integrator>File() function, passing it the simulation data file name, which it gets when called by
        Disp::MainWindow::openSimulation().  It also sets the maximum on the slider in the settings dialog that lets the user slide through the
        simulation frames to the last frame in the simulation.

        @sa @ref Disp::dIFile::reboundFile()
      */
    void OrbitalAnimationDriver::setSimulationData(QString filename, QString fileType, QString dataType, bool b) {
        orbitalAnimator->setLoading(true);
        orbitalAnimator->updateGL(); // makes display show the "Loading" message after the loading flag is set on previous line
        orbitalAnimator->setFullOrbit(b);
        if (QString::compare(fileType,QString("Rebound"),Qt::CaseInsensitive) == 0) {
            ReboundReader reboundFile(filename);
            orbitalAnimator->updateSimulationCache(reboundFile.getData());
        }
        else if (QString::compare(fileType,QString("SWIFT"),Qt::CaseInsensitive) == 0) {
            SwiftReader swiftFile(filename);
            orbitalAnimator->updateSimulationCache(swiftFile.getData());
        }
        else if (QString::compare(fileType,QString("dI"),Qt::CaseInsensitive) == 0) {
            DIReader dIFile(filename);
            orbitalAnimator->updateSimulationCache(dIFile.getData());
        }
        orbitalAnimator->simulationDataLoaded = true;
        orbitalAnimator->updateGL();
    }

    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::clearEquatorialData() { orbitalAnimator->clearEquatorialData(); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::clearSimulationData() { orbitalAnimator->clearSimulationData(); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::clearEclipticData() { orbitalAnimator->clearEclipticData(); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::clearAllData() { orbitalAnimator->clearAllData(); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::playbackQueue(QTableWidget* queue) { orbitalAnimator->playbackQueue(queue); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::record(QTableWidget* queue) { orbitalAnimator->record(queue); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::setViewableArea(int w, int h) { orbitalAnimator->resize(w, h); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::showFrame(int frameNumber) { orbitalAnimator->settingsDialog->setFrame(frameNumber); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */
    void OrbitalAnimationDriver::setZoom(double zoomPercent) { orbitalAnimator->setZoom(zoomPercent); }
    /*! @brief Called by Disp::MainWindow simply to pass the command onto Disp::OrbitalAnimator or Disp::SettingsDialog.

        @sa
    */

    std::vector<double> OrbitalAnimationDriver::getState() {
        std::vector<double> st (5);
        st[0] = orbitalAnimator->getXRotation();
        st[1] = orbitalAnimator->getYRotation();
        st[2] = orbitalAnimator->getZRotation();
        st[3] = orbitalAnimator->getZoomScale();
        st[4] = orbitalAnimator->getCurrentFrame();
        return st;
    }

    int OrbitalAnimationDriver::getSimulationSize() { return orbitalAnimator->getSimulationSize(); }

    void OrbitalAnimationDriver::performAction(QTableWidgetItem* a) {
        if (a->column() == 0) orbitalAnimator->checkAndPerformAction(a);
    }

    void OrbitalAnimationDriver::layoutControls()
    {
        orbitalAnimator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addWidget(orbitalAnimator);
        mainLayout->setMargin(0);
        setLayout(mainLayout);
    }

    void OrbitalAnimationDriver::makeConnections()
    {
        connect(orbitalAnimator->settingsDialog, SIGNAL(setCurrentIndex(int)), orbitalAnimator, SLOT(setCurrentIndex(int)));
        connect(orbitalAnimator->settingsDialog, SIGNAL(setXRot(double)), orbitalAnimator, SLOT(setXRot(double)));
        connect(orbitalAnimator->settingsDialog, SIGNAL(setYRot(double)), orbitalAnimator, SLOT(setYRot(double)));
        connect(orbitalAnimator->settingsDialog, SIGNAL(setZRot(double)), orbitalAnimator, SLOT(setZRot(double)));
        connect(orbitalAnimator->settingsDialog, SIGNAL(setZoomFactor(double)), orbitalAnimator, SLOT(setZoomFactor(double)));
        connect(orbitalAnimator->settingsDialog, SIGNAL(rotate()), orbitalAnimator, SLOT(rotate()));
        connect(orbitalAnimator->settingsDialog, SIGNAL(zoom()), orbitalAnimator, SLOT(zoom()));
        connect(orbitalAnimator->settingsDialog, SIGNAL(simulate()), orbitalAnimator, SLOT(simulate()));
        connect(orbitalAnimator->settingsDialog, SIGNAL(handleAnimateChecked(bool)), this, SLOT(handleAnimateChecked(bool)));

        connect(&animationTimer, SIGNAL(timeout()), orbitalAnimator->settingsDialog, SLOT(advanceTime()));
        //connect(orbitalAnimator->savePicsButton, SIGNAL(clicked()), this, SLOT(savePics()));
    }

    void OrbitalAnimationDriver::handleAnimateChecked(bool val)
    {
        if (val) animationTimer.start();
        else animationTimer.stop();
    }

} // namespace RobD
