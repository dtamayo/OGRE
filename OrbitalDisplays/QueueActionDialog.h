/*!
 @file QueueActionDialog.h
 @brief class definition for RobD::QueueActionDialog, the group of possible dialogs that pop up when the user wants to add an action to the queue.

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

#ifndef QUEUE_ACTION_DIALOG_H
#define QUEUE_ACTION_DIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include "Queue.h"
#include "OrbitalAnimationDriver.h"

/*! @brief This class defines the group of possible dialogs that pop up when the user wants to add an action to the queue.

    When the user selects an action type from the dropdown menu on the top right of the queue, an instance of this class is created (see @ref
    RobD::MainWindow::launchAddActionDialog()).  This class implements all the different dialogs that should be launched, corresponding to the
    different actions.

    All actions to be added to the queue will ask for a span (time over which action should be performed), as well as the row after which the action should
    be inserted.  The other spinboxes are given generic names since they will represent different quantities depending on the action selected.
*/
class QueueActionDialog : public QDialog
{
    Q_OBJECT

public:
    QueueActionDialog(int i, int j, std::vector<double> s, int size);
    Action act;

private:
    QDoubleSpinBox* span;
    QDoubleSpinBox* first;
    QDoubleSpinBox* second;
    QDoubleSpinBox* third;
    QDoubleSpinBox* fourth;
    QSpinBox* fifth;
    QSpinBox* insertRow;

private slots:
    void setValues();

};

#endif
