/*!
 @file Queue.h
 @brief Class definition for RobD::Queue, which holds the actions added by the user at the bottom of the screen.  Also defines the Action struct used
 by RobD::Queue and RobD::QueueActionDialog.

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

#ifndef QUEUE_H
#define QUEUE_H

#include <QTableWidget>
#include <QTableWIdgetItem>
#include <QMenu>
#include <QAction>
#include <QHeaderView>

#define NO_ACTION 0
#define ROTATE 1
#define ZOOM 2
#define SIMULATE 3
#define PAUSE 4
#define INITIALIZE 5

/*! @brief Structure for the various actions that can be chosen by the user to add to the queue.

    Holds variables for all the possible actions.  When the user selects an action, only the appropriate members will be populated.
    Other members will be garbage data.
    "typ" determines the type of the action (see definitions in Queue.h).
    "dFrame" is populated if the type is SIMULATE. It determines how many frames to simulate.
    "frame" is the frame number of the simulation before this action is performed.
    "span" is the number of frames over which this action will be performed.
    "queueIndex" is the position of this action within the queue.
    "dx", "dy", and "dz" are populated if the type is ROTATE. They determinee how many degrees to rotate around
    the x, y, and z axis, respectively.
    "newScale" is populated if the type is ZOOM. It determines the new zoom percentage.
    "x", "y", and "z" are the rotation values before this action is performed.
    "scale" is the zoom percentage before this action is performed.
    "prevScale" is the zoom percentage before the previous action is performed.
*/

struct Action
{
    int typ, dFrame, queueIndex, frame, prevFrame, newFrame;
    double dxrot, dyrot, dzrot, span, xrot, yrot, zrot, scale, prevScale, newScale;
};

Q_DECLARE_METATYPE(Action)

/*! @brief Queue to hold all the user-selected actions at the bottom of the screen.
*/

class Queue : public QTableWidget
{
    Q_OBJECT

public:
    Queue(int i, int j, QWidget* parent);
    void addActionToQueue(Action act);
    void updateQueue(int i, int j);
    void syncAction(Action a1, Action& a2);

public slots:
    void provideContextMenu(QPoint p);
    void removeAction();

private:
    int rowToRemove;
};

#endif
