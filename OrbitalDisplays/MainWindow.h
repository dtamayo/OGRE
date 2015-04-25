/*!
 @file MainWindow.h
 @brief Header file for the MainWindow class, which displays the application's main window.

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QTextEdit>
#include "OrbitalAnimationDriver.h"
#include "QueueActionDialog.h"
#include "Queue.h"
#include "OpenSimulationDialog.h"
#include <QMenuBar>
#include <QtGui/QFileDialog>

namespace Disp
{
    /*! @brief Class for the application's main window.  Defines the widgets to be displayed in it and how
    they should be laid out.

    The main window contains the QGLWidget where orbits are displayed
    and the settingsDialog which holds the sliders and boxes on the right side of the screen.
    This class makes use of the menuBar() function in QMainWindow, and defines what happens when the actions
    in the menus are activated. Any changes to the menu bar can be made here.
    A MainWindow object is instantiated in main.cpp. This class instantiates an OrbitalAnimationDriver
    object and calls Disp::OrbitalAnimationDriver::setupUI(). It does not use any other classes.
    */
    class MainWindow : public QMainWindow
    {
    Q_OBJECT
    public:
        MainWindow(QString filename, QString integrator);
        void setupUI();

    private slots:
        void openSimulationDialog();
        void openSimulation(QString filename, QString filetype, QString datatype, bool fullorbit);
        void openEquatorial();
        void openEcliptic();
        void removeSimulation();
        void removeEquatorial();
        void removeEcliptic();
        void removeAllOrbits();
        void displayCentralBody();
        void chooseCentralBodyColor();
        void displayCoords();
        void displayMainOrbit();
        void displaySpinAxis();
        void launchAddActionDialog();
        void playbackQueue();
        void record();

    private:
        void createMenuOptions();
        void setupMenuOptions();
        void setupMenus();
        void setupActionSelector();
        void setupDriver();
        void setupButtons();
        void createUIElements();
        void setupUIElements();
        void makeConnections();
        void simulationLoaded();
        void equatorialLoaded();
        void eclipticLoaded();
        void simulationRemoved();
        void equatorialRemoved();
        void eclipticRemoved();

        QMenu* fileMenu;
        QMenu* optionsMenu;

        QAction* dispCentralBody;
        QAction* centralBodyColor;
        QAction* dispCoords;
        QAction* dispMainOrbit;
        QAction* dispSpinAxis;

        bool centralBodyShowing;
        bool coordsShowing;
        bool mainOrbitShowing;
        bool spinAxisShowing;

        QAction* openSimulationFile;
        QAction* openEclipticFile;
        QAction* openEquatorialFile;
        QAction* separator;
        QAction* removeSimulationFile;
        QAction* removeEquatorialFile;
        QAction* removeEclipticFile;
        QAction* removeAll;

        Queue* queue;
        QComboBox* actionSelectorButton;
        QPushButton* playbackButton;
        QPushButton* recordButton;

        OrbitalAnimationDriver* driver;
        QWidget* settingsDialog;

        QFileDialog* dlg;

        QVBoxLayout* mainLayout;
        QHBoxLayout* visualizerLayout;
        QHBoxLayout* visualizerTitleLayout;
        QVBoxLayout* queueBoxLayout;
        QHBoxLayout* queueTitleLayout;
        QHBoxLayout* queueLayout;
        QHBoxLayout* actionSelectorLayout;
        QHBoxLayout* playbackButtonLayout;

        QWidget* main;
        QWidget* queueBox;
        QWidget* queueBoxUpper;
        QWidget* queueBoxLower;
        QWidget* actionSelector;
        QWidget* visualizer;
        QWidget* visualizerTitle;
        QWidget* playback;

        int rowToRemove;
    };
}
#endif // MAINWINDOW_H
