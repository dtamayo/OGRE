/*!
 @file MainWindow.cpp
 @brief Implementation of the RobD::MainWindow class, which displays the application's main window.
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

#include "MainWindow.h"

namespace Disp {

/*! @brief Constructor for MainWindow.

  Initializes, lays out and connects the signals and slots of the widgets in MainWindow (this includes the
  OrbitalAnimationDriver that displays the orbits, and the SettingsDialog, which are the sliders and boxes
  on the right of the screen. It also creates and sets up the queue on the bottom (a QTableWidget).
  See individual methods for function and use.  MainWindow inherits from Qt's class QMainWindow.
  See @ref add2ndorb, modsetdiag, modqueue
*/
    MainWindow::MainWindow(QString filename, QString integrator, QString type) : QMainWindow()
    {
        queue = new Queue(0, 7, this);
        driver = new OrbitalAnimationDriver;
        settingsDialog = driver->setupUI();

        createMenuOptions();
        setupMenuOptions();
        setupMenus();
        setupButtons();
        createUIElements();
        setupUIElements();
        makeConnections();

        setCentralWidget(main);
        setMinimumSize(800, 800);
        setWindowTitle("Orbit Simulator");

        if(filename != ""){
            openSimulation(filename, integrator, type, true);
        }
    }

    /*! @brief Opens a file dialog that asks the user to open an input file.

        Called from the file menu in the menu bar--File -> Open.  Clicking that button corresponds to the QAction
        openSimulationFile defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections() (see @ref sigslots).
        It pops up a file dialog that lets the user select the file (see
        @ref adddialog).  You select the QString to the file's path with dlg->selectedFiles().first().
        This is passed to the RobD::OrbitalAnimationDriver through RobD::OrbitalAnimationDriver::setSimulationData().
        Once done, it calls RobD::MainWindow::simulationLoaded() to initialize the appropriate settings.

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::openSimulationDialog() {
        OpenSimulationDialog dialog;
        if (dialog.exec() == QDialog::Accepted) {
            openSimulation(dialog.getFileName(),dialog.getFileType(),"",dialog.getDrawFullOrbit());
            simulationLoaded();
        }
    }

    void MainWindow::openSimulation(QString filename, QString filetype, QString datatype, bool fullorbit) {
        driver->setSimulationData(filename, filetype, datatype, fullorbit);
        simulationLoaded();
    }


    /*! @brief Opens a file dialog that asks the user to open a .csv file.

        The selected file will be loaded into the OrbitalAnimator.
        Called from the file menu in the menu bar--File -> Open Equatorial Data File .  Clicking that button corresponds to the QAction
        openEquatorial defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections() (see @ref sigslots).
        See a more detailed analogous description in RobD::MainWindow::openSimulation().

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::openEquatorial() {
        dlg = new QFileDialog();
        if (dlg->exec() == QDialog::Accepted) {
            driver->setEquatorialData(dlg->selectedFiles().first());
            equatorialLoaded();
        }
    }

    /*! @brief Opens a file dialog that asks the user to open a .csv file.

        The selected file will be loaded into the OrbitalAnimator.
        Called from the file menu in the menu bar--File -> Open Equatorial Data File .  Clicking that button corresponds to the QAction
        openEcliptic defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections() (see @ref sigslots).
        See a more detailed analogous description in RobD::MainWindow::openSimulation().

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::openEcliptic() {
        dlg = new QFileDialog();
        if (dlg->exec() == QDialog::Accepted) {
            driver->setEclipticData(dlg->selectedFiles().first());
            eclipticLoaded();
        }
    }

    /*! @brief Removes the displayed ecliptic orbits from the OrbitalAnimator.

        Called from the file menu in the menu bar.  File -> Remove Ecliptic Orbits
        Connected to the removeEclipticFile action defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Disabled if ecliptic orbits have not been loaded.  See the way that's done in RobD::MainWindow::eclipticRemoved().

         @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::removeEcliptic() {
        driver->clearEclipticData();
        eclipticRemoved();
    }

    /*! @brief Removes the displayed equatorial orbits from the OrbitalAnimator.

        Called from the file menu in the menu bar.  File -> Remove Equatorial Orbits
        Connected to the removeEquatorialFile action defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots)..  Disabled if equatorial orbits have not been loaded.  See the way that's
        done in RobD::MainWindow::equatorialRemoved().

         @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::removeEquatorial() {
        driver->clearEquatorialData();
        equatorialRemoved();
    }

    /*! @brief Removes the simulation from the OrbitalAnimator.

        Called from the file menu in the menu bar.  File -> Remove Simulation Orbit
        Connected to the removeSimulationFile action defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Disabled if simulation has not been loaded.  See the way that's
        done in RobD::MainWindow::simulationRemoved().

         @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::removeSimulation() {
        driver->clearSimulationData();
        simulationRemoved();
    }

    /*! @brief Removes all data from the OrbitalAnimator.

        Called from the file menu in the menu bar.  File -> Remove All Orbits
        Connected to the removeAll action defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Disabled if nothing has been loaded.  See the way that's
        done in RobD::MainWindow::simulationRemoved().

         @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::removeAllOrbits() {
        driver->clearAllData();
        simulationRemoved();
        equatorialRemoved();
        eclipticRemoved();
    }

    /*!
     * @brief Toggles the display for the central body.

        Disabled if nothing has been loaded.
        Called from the options menu in the menu bar. Options -> Show/Hide Central Body
        Connected to the dispCentralBody action defined in MainWindow.h, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Updates the boolean centralBodyShowing for use within RobD::MainWindow, and updates the value in the
        RobD::OrbitalAnimationDriver 's member class RobD::AnimatorSettings, so that the change can get implemented in the RobD::OrbitalAnimator.
        Finally, it changes the text that's displayed in the menu item dispCentralBody (that the user sees) to Show/Hide Central Body so that the
        appropriate action is shown in the future.

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
     */
    void MainWindow::displayCentralBody() {
        if (centralBodyShowing) {
            driver->animatorSettings.setDisplayCentralBody(false);
            centralBodyShowing = false;
            dispCentralBody->setText(tr("&Show Central Body"));
        }
        else {
            driver->animatorSettings.setDisplayCentralBody(true);
            centralBodyShowing = true;
            dispCentralBody->setText(tr("&Hide Central Body"));
        }
    }

    /*!
     * @brief Brings up a dialog for the user to choose the color of the central body.

        Disabled if nothing has been loaded.
        Called from the options menu in the menu bar. Options -> Change Central Body Color
        Connected to the centralBodyColor action, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  For a description of how dialogs work, see @ref adddialog.

        This dialog is implemented differently from the other dialogs because it is more convenient for the user to have the colors
        continually update as he or she scrolls through the different options.
        The connect() call is made here so that every time the user clicks on a different color in the dialog, the OrbitalAnimator
        updates the centralBody with that color. There is no need to wait for exec() to return, because the central body will just
        remain the last color selected before the dialog closed.

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::createMenuOptions() and RobD::MainWindow::setupMenuOptions()
     */
    void MainWindow::chooseCentralBodyColor() {
        QColorDialog dlg(driver->animatorSettings.centralBodyColor(), this);
        dlg.setOption(QColorDialog::ShowAlphaChannel, true);
        driver->connect(&dlg, SIGNAL(currentColorChanged(const QColor&)),
                        &(driver->animatorSettings), SLOT(setCentralBodyColor(const QColor&)));
        dlg.exec();
    }

    /*!
     * @brief Toggles the display for the coordinate axes.

        Disabled if nothing has been loaded.
        Called from the options menu in the menu bar. Options -> Show/Hide Coordinate Axes
        Connected to the dispCoords action, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Also see RobD::MainWindow::displayCentralBody() for an analogous description of the function body.
     */
    void MainWindow::displayCoords() {
        if (coordsShowing) {
            driver->animatorSettings.setDisplayCoords(false);
            coordsShowing = false;
            dispCoords->setText(tr("&Show Coordinate Axes"));
        }
        else {
            driver->animatorSettings.setDisplayCoords(true);
            coordsShowing = true;
            dispCoords->setText(tr("&Hide Coordinate Axes"));
        }
    }

    /*!
     * @brief Toggles the display for the main orbit.

        Disabled if the simulation has not been loaded.
        Called from the options menu in the menu bar. Options -> Show/Hide Main Orbit
        Connected to the dispMainOrbit action, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Also see RobD::MainWindow::displayCentralBody() for an analogous description of the function body.

     */
    void MainWindow::displayMainOrbit() {
        if (mainOrbitShowing) {
            driver->animatorSettings.setDisplayMainOrbit(false);
            mainOrbitShowing = false;
            dispMainOrbit->setText(tr("&Show Main Orbit"));
        }
        else {
            driver->animatorSettings.setDisplayMainOrbit(true);
            mainOrbitShowing = true;
            dispMainOrbit->setText(tr("&Hide Main Orbit"));
        }
    }

    /*!
     * @brief Toggles the display for the spin axis.

        Disabled if the simulation has not been loaded.
        Called from the options menu in the menu bar. Options -> Show/Hide Spin Axis
        Connected to the dispSpinAxis action, which is connected to this function in RobD::MainWindow::makeConnections()
        (see @ref sigslots).  Also see RobD::MainWindow::displayCentralBody() for an analogous description of the function body.
     */
    void MainWindow::displaySpinAxis() {
        if (spinAxisShowing) {
            driver->animatorSettings.setDisplaySpinAxis(false);
            spinAxisShowing = false;
            dispSpinAxis->setText(tr("&Show Spin Axis"));
        }
        else {
            driver->animatorSettings.setDisplaySpinAxis(true);
            spinAxisShowing = true;
            dispSpinAxis->setText(tr("&Hide Spin Axis"));
        }
    }

    /*!
     * @brief Launches a dialog used for adding an action to the queue.

        Once the user selects a type of action to add to the queue from the dropdown menu on the top right of the queue (the actionSelectorButton
        defined in MainWindow.h), a SIGNAL (connected in RobD::MainWindow::makeConnections()) triggers this function (SLOT), which pops up a dialog for the
        corresponding action.  The class for all the different possible dialogs is RobD::QueueActionDialog.  This function creates a
        RobD::QueueActionDialog instance, passing it the type of action to be added to the queue (an integer value with cases defined in
        QueueActionDialog.cpp), the current number of actions in the queue, and the current state of the RobD::OrbitalAnimator (with the rotation
        angles, zoom etc. to set the default values in the dialog to the current values if desired).

        If the dialog.exec() (the function that pops up the dialog) returns QDialog::Accepted, a new action is added to the queue.
        For more information on how dialogs work, see @ref modqueue.
        For more information on how actions are added to the queue, see Queue::addActionToQueue().

        See @ref QueueActionDialog.h
     */
    void MainWindow::launchAddActionDialog() {
        if(actionSelectorButton->currentIndex() != 0) // if an action other than the blank one is selected, pop up a dialog
        {
            QueueActionDialog dialog(actionSelectorButton->currentIndex(),
                                     queue->rowCount(),
                                     driver->getState(),
                                     driver->getSimulationSize());
            if (dialog.exec() == QDialog::Accepted) {
                queue->addActionToQueue(dialog.act);
            }
        }
    }

    /*! @brief Calls RobD::OrbitalAnimationDriver::playbackQueue(), passing it the queue.

        Called when the playbackButton is pressed.  Corresponds to the playbackButton QPushButton defined in MainWindow.h, that is connected to this
        function (SLOT) in RobD::MainWindow::MakeConnections() (see @ref sigslots).

        @sa @ref RobD::OrbitalAnimationDriver::playbackQueue(), RobD::MainWindow::makeConnections()
    */
    void MainWindow::playbackQueue() { driver->playbackQueue(queue); }

    /*! @brief Calls RobD::OrbitalAnimationDriver::record(), passing it the queue.

        Called when the recordButton is pressed.  Corresponds to the recordButton QPushButton defined in MainWindow.h, that is connected to this
        function (SLOT) in RobD::MainWindow::MakeConnections() (see @ref sigslots).

        @sa @ref RobD::OrbitalAnimationDriver::record(), RobD::MainWindow::makeConnections()
    */
    void MainWindow::record() { driver->record(queue); }

    /*! @brief Initializes all of the actions to be put into the menu bar.

        Note: the tr() function used here makes it so that the text passed to it will be appropriately translated
        in a different language environment.

        @sa @ref addmenuitem, RobD::MainWindow::makeConnections(), RobD::MainWindow::setupMenus(),
        and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::createMenuOptions() {
        openSimulationFile = new QAction(tr("&Open Simulation File"), this);
        openEquatorialFile = new QAction(tr("&Open Equatorial Data File"), this);
        openEclipticFile = new QAction(tr("&Open Ecliptic Data File"), this);
        removeSimulationFile = new QAction(tr("&Remove Simulation Orbit"), this);
        removeEquatorialFile = new QAction(tr("&Remove Equatorial Orbits"), this);
        removeEclipticFile = new QAction(tr("&Remove Ecliptic Orbits"), this);
        removeAll = new QAction(tr("&Remove All Orbits"), this);
        dispCentralBody = new QAction(tr("&Hide Central Body"), this);
        centralBodyColor = new QAction(tr("&Change Central Body Color"), this);
        dispCoords = new QAction(tr("&Hide Coordinate Axes"), this);
        dispMainOrbit = new QAction(tr("&Hide Main Orbit"), this);
        dispSpinAxis = new QAction(tr("&Hide Spin Axis"), this);
        separator = new QAction(this);
    }

    /*! @brief Sets initial conditions for actions as well as shortcuts

        Shortcuts get set to the operating system's default, e.g., open = open apple-O on mac, ctrl-o on Windows).

        @sa @ref addmenuitem, RobD::MainWindow::createMenuOptions(), RobD::MainWindow::setupMenus(),
        and RobD::MainWindow::makeConnections()
    */
    void MainWindow::setupMenuOptions() {
        openSimulationFile->setShortcuts(QKeySequence::Open);
        removeSimulationFile->setDisabled(true);
        removeEquatorialFile->setDisabled(true);
        removeEclipticFile->setDisabled(true);
        removeAll->setDisabled(true);
        dispCentralBody->setDisabled(true);
        centralBodyColor->setDisabled(true);
        dispCoords->setDisabled(true);
        dispMainOrbit->setDisabled(true);
        dispSpinAxis->setDisabled(true);
        separator->setSeparator(true); // a horizontal line to be displayed in the file menu below the different open options
    }

    /*! @brief Adds all of the actions into their corresponding menu bar.

        @sa @ref addmenuitem, RobD::MainWindow::createMenuOptions(), RobD::MainWindow::makeConnections()
        and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::setupMenus() {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(openSimulationFile);
        fileMenu->addAction(openEquatorialFile);
        fileMenu->addAction(openEclipticFile);
        fileMenu->addAction(separator);
        fileMenu->addAction(removeSimulationFile);
        fileMenu->addAction(removeEquatorialFile);
        fileMenu->addAction(removeEclipticFile);
        fileMenu->addAction(removeAll);
        optionsMenu = menuBar()->addMenu(tr("&Options"));
        optionsMenu->addAction(dispCentralBody);
        optionsMenu->addAction(centralBodyColor);
        optionsMenu->addAction(dispCoords);
        optionsMenu->addAction(dispMainOrbit);
        optionsMenu->addAction(dispSpinAxis);
    }

    /*! @brief Initializes the actionSelectorButton (a QComboBox) that's used to add actions to the queue at the bottom.

        Adds all the different options for the queue to the dropdown menu widget on the top right of the queue (actionSelectorButton defined in
        MainWindow.h).  See additional documentation in RobD::MainWindow::launchAddActionDialog().

        @sa RobD::MainWindow::makeConnections() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::setupActionSelector() {
        actionSelectorButton = new QComboBox(this);
        actionSelectorButton->addItem(" ",0);
        actionSelectorButton->addItem(tr("Rotate"), 0);
        actionSelectorButton->addItem(tr("Zoom"), 0);
        actionSelectorButton->addItem(tr("Simulate"), 0);
        actionSelectorButton->addItem(tr("Pause"), 0);
        actionSelectorButton->addItem(tr("Initialize"), 0);
    }

    /*! @brief Initializes all of the push buttons in the main window like Playback and Record on the bottom right, and the dropdown menu for adding
        actions to the queue.

         @sa RobD::MainWindow::makeConnections() and RobD::MainWindow::setupMenuOptions()
    */
    void MainWindow::setupButtons() {
        playbackButton = new QPushButton(tr("Playback"), this);
        playbackButton->setMaximumWidth(80);
        recordButton = new QPushButton(tr("Record"), this);
        recordButton->setMaximumWidth(80);
        setupActionSelector();
    }

    /*! @brief Initializes all of the layouts and widgets necessary to organize the main window.

        The layouts created here will later contain all of the widgets to be added (the "queue,"
        "settingsDialog," etc.).
        "mainLayout" is the QVBoxLayout underlying MainWindow - it will contain "main."
        "visualizerTitleLayout" is simply the layout for the text above the OrbitalAnimator - it will contain "visualizerTitle."
        "visualizerLayout" is the layout that holds the orbitalAnimator and the settingsDialog - it will contain "visualizer."
        "queueBoxLayout" is the layout for the bottom half of the screen - it will contain "queueBox."
        "queueTitleLayout" is the layout at the top of "queueBoxLayout" - it will contain "actionSelector."
        "queueLayout" is the layout that wraps the "queue."
        "actionSelectorLayout" is the layout for "actionSelector," and it is located at the right of
        "queueTitleLayout." It contains the "actionSelectorButton" and its label.
        "playbackButtonLayout" is the layout for "playback," and it is located at the bottom of "queueBoxLayout."
        It contains "playbackButton" and the "recordButton."

        See MainWindow::setupUIElements()
    */
    void MainWindow::createUIElements() {
        mainLayout = new QVBoxLayout;
        visualizerTitleLayout = new QHBoxLayout;
        visualizerLayout = new QHBoxLayout;
        queueBoxLayout = new QVBoxLayout;
        queueTitleLayout = new QHBoxLayout;
        queueLayout = new QHBoxLayout;
        actionSelectorLayout = new QHBoxLayout;
        playbackButtonLayout = new QHBoxLayout;

        main = new QWidget;
        queueBox = new QWidget;
        queueBoxUpper = new QWidget;
        queueBoxLower = new QWidget;
        actionSelector = new QWidget;
        visualizer = new QWidget;
        visualizerTitle = new QWidget;
        playback = new QWidget;
    }

    /*! @brief Organizes all of the widgets into layouts.

        This function adds all of the widgets to the window. When widgets are added to the window,
        they are first added to a particular layout, and then that layout is applied to a "higher-level" widget.
        This helps organize the widgets into groups.  The widgets used here are created in RobD::MainWindow::createUIElements().

        The highest level widget is "main". All other widgets are added to main's layout, "mainLayout".
        There are three widgets within mainLayout: "visualizerTitle," "visualizer," and "queueBox."
        These three widgets are stacked vertically on top of each other in mainLayout.

        The layout for "visualizerTitle" is "visualizerTitleLayout," and it only contains one widget,
        the QLabel "title" (it is just a wrapper, but it is necessary due to the setMargin() function only being
        a member of QLayout). The QLabel "title" contains the text located above the OrbitalAnimator.

        The layout for "visualizer" is "visualizerLayout." It is a horizontal layout that contains the widgets
        "driver" (the OrbitalAnimator) and "settingsDialog" (the SettingsDialog).

        The layout for "queueBox" is "queueBoxLayout." It contains three widgets, "queueBoxUpper,"
        "queueBoxLower," and "playback," stacked vertically on top of each other in that order.
        The layout for "queueBoxUpper" is "queueTitleLayout." This layout contains the QLabel "queueTitle,"
        and the QComboBox "actionSelector." It is shown above the QTableWidget "queue." The layout for
        "queueBoxLower" is queueLayout, and it just contains the QTableWidget "queue." It is just a wrapper.
        Finally, the layout for "playback" is "playbackButtonLayout." It is a horizontal layout that contains the
        two buttons "recordButton" and "playbackButton."

        @sa MainWindow::createUIElements()
    */
    void MainWindow::setupUIElements() {
        QLabel* title = new QLabel;
        QFont titleFont = title->font();
        titleFont.setPointSize(18);
        title->setFont(titleFont);
        title->setText(tr("Orbit Visualizer"));

        visualizerTitleLayout->setMargin(5);
        visualizerTitleLayout->addWidget(title);
        visualizerTitle->setLayout(visualizerTitleLayout);

        visualizerLayout->addWidget(driver);
        visualizerLayout->addWidget(settingsDialog);
        visualizerLayout->setMargin(0);
        visualizer->setLayout(visualizerLayout);

        QLabel* actionSelectorLabel = new QLabel;
        actionSelectorLabel->setText(tr("Action Type: "));

        actionSelectorLayout->addWidget(actionSelectorLabel);
        actionSelectorLayout->addWidget(actionSelectorButton);
        actionSelector->setLayout(actionSelectorLayout);
        actionSelector->setMaximumWidth(240);

        QLabel* queueTitle = new QLabel;
        queueTitle->setMargin(5);
        QFont queueTitleFont = queueTitle->font();
        queueTitleFont.setPointSize(18);
        queueTitle->setFont(queueTitleFont);
        queueTitle->setText(tr("Action Queue"));

        queueTitleLayout->addWidget(queueTitle);
        queueTitleLayout->addWidget(actionSelector);
        queueTitleLayout->setMargin(0);
        queueBoxUpper->setLayout(queueTitleLayout);

        playbackButtonLayout->addWidget(recordButton);
        playbackButtonLayout->addWidget(playbackButton);
        playbackButtonLayout->setAlignment(Qt::AlignRight);
        playbackButtonLayout->setMargin(5);
        playback->setLayout(playbackButtonLayout);

        queueLayout->addWidget(queue);
        queueLayout->setMargin(0);
        queueBoxLower->setLayout(queueLayout);

        queueBoxLayout->addWidget(queueBoxUpper);
        queueBoxLayout->addWidget(queueBoxLower);
        queueBoxLayout->addWidget(playback);
        queueBoxLayout->setMargin(0);
        queueBoxLayout->setSpacing(0);
        queueBox->setLayout(queueBoxLayout);

        mainLayout->addWidget(visualizerTitle);
        mainLayout->addWidget(visualizer);
        mainLayout->addWidget(queueBox);
        main->setLayout(mainLayout);
    }

    /*! @brief Connects all buttons and actions to their appropriate slots.

        Each signal from a widget in the main window is connected to a slot method either in this class
        or in a member object's class.
    */
    void MainWindow::makeConnections() {
        connect(openSimulationFile, SIGNAL(triggered()), this, SLOT(openSimulationDialog()));
        connect(openEquatorialFile, SIGNAL(triggered()), this, SLOT(openEquatorial()));
        connect(openEclipticFile, SIGNAL(triggered()), this, SLOT(openEcliptic()));
        connect(removeSimulationFile, SIGNAL(triggered()), this, SLOT(removeSimulation()));
        connect(removeEquatorialFile, SIGNAL(triggered()), this, SLOT(removeEquatorial()));
        connect(removeEclipticFile, SIGNAL(triggered()), this, SLOT(removeEcliptic()));
        connect(removeAll, SIGNAL(triggered()), this, SLOT(removeAllOrbits()));
        connect(dispCentralBody, SIGNAL(triggered()), this, SLOT(displayCentralBody()));
        connect(centralBodyColor, SIGNAL(triggered()), this, SLOT(chooseCentralBodyColor()));
        connect(dispCoords, SIGNAL(triggered()), this, SLOT(displayCoords()));
        connect(dispMainOrbit, SIGNAL(triggered()), this, SLOT(displayMainOrbit()));
        connect(dispSpinAxis, SIGNAL(triggered()), this, SLOT(displaySpinAxis()));
        /*connect(queue, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
                driver, SLOT(performAction(QTableWidgetItem*)));*/
        connect(queue, SIGNAL(customContextMenuRequested(QPoint)), queue, SLOT(provideContextMenu(QPoint)));
        connect(playbackButton, SIGNAL(clicked()), this, SLOT(playbackQueue()));
        connect(recordButton, SIGNAL(clicked()), this, SLOT(record()));
        connect(actionSelectorButton, SIGNAL(activated(int)), this, SLOT(launchAddActionDialog()));
    }

    /*! @brief Called when a simulation file is loaded to update the appropriate default settings.

        Changes all of the settings associated with the simulation file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::openSimulationFile().
      */
    void MainWindow::simulationLoaded() {
        dispCentralBody->setEnabled(true);
        centralBodyColor->setEnabled(true);
        dispCoords->setEnabled(true);
        dispMainOrbit->setEnabled(true);
        dispSpinAxis->setEnabled(true);
        centralBodyShowing = true;
        coordsShowing = true;
        mainOrbitShowing = true;
        spinAxisShowing = true;
        removeSimulationFile->setEnabled(true);
        removeAll->setEnabled(true);
    }


    /*! @brief Called when an equatorial file is loaded.

        Changes all of the settings associated with the equatorial file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::openEquatorialFile().
      */
    void MainWindow::equatorialLoaded() {
        dispCentralBody->setEnabled(true);
        centralBodyColor->setEnabled(true);
        centralBodyShowing = true;
        removeEquatorialFile->setEnabled(true);
        removeAll->setEnabled(true);
    }


    /*! @brief Called when an ecliptic file is loaded.

        Changes all of the settings associated with the ecliptic file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::openEclipticFile().
      */
    void MainWindow::eclipticLoaded() {
        dispCentralBody->setEnabled(true);
        centralBodyColor->setEnabled(true);
        centralBodyShowing = true;
        removeEclipticFile->setEnabled(true);
        removeAll->setEnabled(true);
    }


    /*! @brief Called when the simulation data is removed.

        Changes all of the settings associated with the simulation file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::removeSimulationFile().
      */
    void MainWindow::simulationRemoved() {
        removeSimulationFile->setDisabled(true);
        dispCoords->setDisabled(true);
        dispMainOrbit->setDisabled(true);
        dispSpinAxis->setDisabled(true);
        if (!removeEquatorialFile->isEnabled() && !removeEclipticFile->isEnabled()) {
            removeAll->setDisabled(true);
            dispCentralBody->setDisabled(true);
            centralBodyColor->setDisabled(true);
        }
    }


    /*! @brief Called when the equatorial data is removed.

        Changes all of the settings associated with the equatorial file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::removeEquatorialFile().
      */
    void MainWindow::equatorialRemoved() {
        removeEquatorialFile->setDisabled(true);
        if (!removeSimulationFile->isEnabled() && !removeEclipticFile->isEnabled()) {
            removeAll->setDisabled(true);
            dispCentralBody->setDisabled(true);
            centralBodyColor->setDisabled(true);
        }
    }


    /*! @brief Called when the ecliptic data is removed.

        Changes all of the settings associated with the ecliptic file to the appropriate values.  You would change the default settings here.
        Called by RobD::MainWindow::removeEclipticFile().
      */
    void MainWindow::eclipticRemoved() {
        removeEclipticFile->setDisabled(true);
        if (!removeEquatorialFile->isEnabled() && !removeSimulationFile->isEnabled()) {
            removeAll->setDisabled(true);
            dispCentralBody->setDisabled(true);
            centralBodyColor->setDisabled(true);
        }
    }
}
