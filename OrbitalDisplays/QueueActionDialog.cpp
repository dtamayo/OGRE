/*!
 @file QueueActionDialog.cpp
 @brief Implementation for RobD::QueueActionDialog, the group of possible dialogs that pop up when the user wants to add an action to the queue.

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

#include "QueueActionDialog.h"

/*! @brief Constructor that creates the appropriate dialog for the action type passed.

    The different action types are #defined in Queue.h.  The constructor is passed one of these action types, the number of actions in the queue prior
    to adding this new one, and the state of the orbital animator that holds rotation angles etc. to populate default values in the dialog (see @ref
    RobD::OrbitalAnimationDriver::getState()).  The type is put into the member variable act, which is an instance the Action struct defined in Queue.h,
    and is then used to choose which type of dialog to create.

    We now detail the layout for the dialogs.  For a good tutorial on layouts, see <a href="http://qt-project.org/doc/qt-4.8/layouts-basiclayouts.html">
    http://qt-project.org/doc/qt-4.8/layouts-basiclayouts.html </a>.  In all cases, the dialog consists of a mainLayout of type QVBoxLayout (a simple
    vertical layout), to which two widgets are added.  The top one holds all the subwidgets (spinboxes etc.) for the user to set the values
    appropriate to the chosen action.  It is called "form."  It uses a QFormLayout, which lets you add a widget (spinbox, slider etc.) on each line,
    along with a label preceding it (add an action in the application to see this more concretely).  The second widget added to mainLayout is the
    QPushButton "add."  The function calls in the action-specific blocks are hopefully self-evident---the different spinbox widgets were given generic
    names since they represent different quantities for different actions.  For initialize, the current values in the
    RobD::OrbitalAnimator (passed to RobD::QueueActionDialog in the constructor) are used to populate the default values for the respective widgets.  The
    add button's clicked SIGNAL is connected to RobD::QueueActionDialog::setValues(), which populates act with the user-selected values.
    The NO_ACTION action type is there so that a blank action type is displayed by default in the drop-down menu.

*/

QueueActionDialog::QueueActionDialog(int type, int numRows, std::vector<double> state, int size) : QDialog() {
    act.typ = type;
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QWidget* form = new QWidget;
    QFormLayout* formLayout = new QFormLayout;
    QPushButton* add = new QPushButton("Add", this);
    switch (act.typ) {
    case ROTATE:
        first = new QDoubleSpinBox;
        second = new QDoubleSpinBox;
        third = new QDoubleSpinBox;
        first->setRange(-180, 180);
        second->setRange(-180, 180);
        third->setRange(-180, 180);
        formLayout->addRow("Rotate X by: ", first);
        formLayout->addRow("Rotate Y by: ", second);
        formLayout->addRow("Rotate Z by: ", third);
        setWindowTitle("Rotate");
        break;
    case ZOOM:
        first = new QDoubleSpinBox;
        first->setRange(0.001, 5000);
        first->setValue(1);
        formLayout->addRow("New Zoom Factor: ", first);
        setWindowTitle("Zoom");
        break;
    case SIMULATE:
        fifth = new QSpinBox;
        fifth->setRange(0, size);
        formLayout->addRow("Advance frames by: ", fifth);
        setWindowTitle("Simulate");
        break;
    case PAUSE:
        setWindowTitle("Pause");
        first = new QDoubleSpinBox;
        first->setRange(0,1000);
        formLayout->addRow("Pause duration (sec): ", first);
        break;
    case INITIALIZE:
        setWindowTitle("Initialize");
        first = new QDoubleSpinBox;
        second = new QDoubleSpinBox;
        third = new QDoubleSpinBox;
        fourth = new QDoubleSpinBox;
        fifth = new QSpinBox;
        first->setRange(-180, 180);
        second->setRange(-180, 180);
        third->setRange(-180, 180);
        fourth->setRange(0.001, 5000);
        fifth->setRange(0, size);
        first->setValue(state[0]);
        second->setValue(state[1]);
        third->setValue(state[2]);
        fourth->setValue(state[3]);
        fifth->setValue(state[4]);
        formLayout->addRow("X Rotation: ", first);
        formLayout->addRow("Y Rotation: ", second);
        formLayout->addRow("Z Rotation: ", third);
        formLayout->addRow("Zoom scale: ", fourth);
        formLayout->addRow("Frame Number: ", fifth);
    }
    if (act.typ != NO_ACTION && act.typ != INITIALIZE) { // if NO_ACTION or INITIALIZE, don't need to define the timespan or ask where to insert the action
        span = new QDoubleSpinBox;
        span->setRange(0.001, 1000);
        span->setMinimumWidth(81);
        span->setValue(1);
        formLayout->addRow("Span (sec): ", span);
        insertRow = new QSpinBox;
        insertRow->setRange(1, numRows);
        insertRow->setValue(numRows);
        formLayout->addRow("Insert after row: ", insertRow);
    }
    form->setLayout(formLayout);
    mainLayout->addWidget(form);
    mainLayout->addWidget(add);
    setLayout(mainLayout);
    connect(add, SIGNAL(clicked()), this, SLOT(setValues()));
}

/*! @brief Adds all the user-selected values into the RobD::QueueActionDialog 's member variable act.  Called once the user clicks "Add" in the dialog.
*/
void QueueActionDialog::setValues() {
    act.span = act.typ == INITIALIZE ? 0 : span->value();   // span = 0 if the action type is initialize, otherwise the value in the span widget
    qDebug() << act.span;
    act.queueIndex = act.typ == INITIALIZE ? 0 : insertRow->value();
    switch(act.typ) {
    case ROTATE: act.dx = first->value(); act.dy = second->value(); act.dz = third->value(); break;
    case ZOOM: act.newScale = first->value(); break;
    case SIMULATE: act.dFrame = fifth->value(); break;
    case INITIALIZE:
        act.x = first->value();
        act.y = second->value();
        act.z = third->value();
        act.scale = fourth->value();
        act.frame = fifth->value();
        break;
    }
    accept();
}
