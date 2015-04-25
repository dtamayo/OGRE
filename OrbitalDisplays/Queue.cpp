/*!
 @file Queue.cpp
 @brief Implementation for RobD::Queue, which holds the actions added by the user at the bottom of the screen.

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

#include "Queue.h"

/*! @brief Constructor for the queue.

    This constructor calls the QTableWidget constructor, then creates some initial settings for the table.
    The top of each column is labeled with a header.
    Rows in the table correspond with actions in the queue.
    To begin with, one action is added to the queue - initialize.
    By default, the values for this action are standard - Rotate(0, 0, 0), Zoom 1, frame 0.

*/

Queue::Queue(int rows, int columns, QWidget* parent) : QTableWidget(rows, columns, parent) {
    setColumnWidth(0, 210);
    QHeaderView* h = horizontalHeader();
    h->setResizeMode(QHeaderView::Stretch);
    setContextMenuPolicy(Qt::CustomContextMenu);
    QStringList headers;
    headers << "Action" << "Span" << "X Rotation" << "Y Rotation" << "Z Rotation"
            << "Zoom Factor" << "Frame Number";
    setHorizontalHeaderLabels(headers);
    Action act;
    act.typ = INITIALIZE;
    act.xrot = 0;
    act.yrot = 0;
    act.zrot = 0;
    act.scale = 1;
    act.frame = 0;
    act.span = 0;
    act.queueIndex = 0;
    addActionToQueue(act);
}



/*! @brief Takes in an action and returns an array of strings describing the state of the simulation before the action.

    This function formats the members of the passed action - xrot, yrot, zrot, scale, and frame.
    It then puts them in a string array "strings" and returns it.

*/
std::vector<QString> getStateStrings(Action action) {
    QString x, y, z, sc, fr;
    x = QString::number(action.xrot, 'f', 2);
    y = QString::number(action.yrot, 'f', 2);
    z = QString::number(action.zrot, 'f', 2);
    sc = QString::number(action.scale, 'f', 2);
    fr = QString::number(action.frame);
    std::vector<QString> strings (5);
    strings[0] = x;
    strings[1] = y;
    strings[2] = z;
    strings[3] = sc;
    strings[4] = fr;
    return strings;
}

/*! @brief Takes in an action and adds it to the queue.

    This function parses the passed action and displays the appropriate information in the table.
    It also stores the action as data in the QTableWidgetItem in the first column.
    This is done using the function setData().
    This function takes in an int and a QVariant.
    The int represents a role - this is basically an index into an array or a key into a hashtable.
    The role used here is predefined within Qt - Qt::UserRole.
    This role is specifically set aside by Qt for the user to use - it should be empty unless the user puts something in it.
    The QVariant is the data to be stored. QVariants can store any type of data - even structs!
    In this case, an action struct is being stored using the function setValue().
    After storing the action into the QVariant, the QVariant can be stored in the QTableWidgetItem using setData().

    To extract the data from the QTableWidgetItem, simply call the function data().
    This function takes in an int representing a role. To retrieve the data store here, pass it Qt::UserRole.
    The function will return a QVariant. The QVariant can be cast into whatever data type is stored within it using the value() function.
    In this case, of course, the data type is the struct Action. The form for the function should look like this:

    Action act = qvariant.value<Action>();

    "Action" here can be replaced with an arbitrary data type.
*/
void Queue::addActionToQueue(Action action) {
    QVariant v;
    v.setValue(action);
    QTableWidgetItem* act = new QTableWidgetItem(0);
    act->setData(Qt::UserRole, v);
    act->setFlags(act->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem* span = new QTableWidgetItem(0);
    span->setFlags(span->flags() ^ Qt::ItemIsEditable);
    span->setTextAlignment(Qt::AlignHCenter);

    int index = action.queueIndex;
    if (action.typ != INITIALIZE || rowCount() == 0) {
        insertRow(index);
        setRowHeight(index, 20);
    }

    setItem(index, 0, act);
    setItem(index, 1, span);
    for (int i=0; i < 5; i++) {
        QTableWidgetItem* item = new QTableWidgetItem(0);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignHCenter);
        setItem(index, i + 2, item);
    }
    updateQueue(index, rowCount());
}

/* @brief Updates all actions with indices between "start" and "end" in the queue.
 *
    For each action, this function makes sure it syncs up with the previous action in the
    queue. It then updates the text in the table.  Called whenever queue is updated.
*/
void Queue::updateQueue(int start, int end) {
    for (int i = start; i < end; i++) {
        QVariant v2 = item(i, 0)->data(Qt::UserRole);
        Action act = v2.value<Action>();

        if(act.typ != INITIALIZE){
            QVariant v1 = item(i - 1, 0)->data(Qt::UserRole);
            Action prevAct = v1.value<Action>();
            syncAction(prevAct, act);

            if(act.frame < prevAct.frame){
                act.frame = prevAct.frame;
            }
        }

        QString text, x, y, z, sc, fr;
        switch (act.typ) {
        case ROTATE:
            x = QString::number(act.xrot);
            y = QString::number(act.yrot);
            z = QString::number(act.zrot);
            text = QString("Rot. to (%1, %2, %3)").arg(x, y, z);
            break;
        case ZOOM:
            sc = QString::number(act.scale);
            text = QString("Zoom to %1").arg(sc);
            break;
        case SIMULATE:
            fr = QString::number(act.frame);
            text = QString("Sim. to frame %1").arg(fr);
            break;
        case PAUSE:
            text = "Pause";
            break;
        case INITIALIZE:
            text = "Initialize";
            break;
        }

        act.queueIndex = i;
        v2.setValue(act);
        item(i, 0)->setData(Qt::UserRole, v2);
        item(i, 0)->setText(text);

        text = QString::number(act.span);
        item(i, 1)->setText(text);

        std::vector<QString> strings = getStateStrings(act);
        for (int j=0; j < 5; j++) item(i, j + 2)->setText(strings[j]);
    }
}

/*! @brief Used to make sure a queue action is synced up with the previous one in the queue.

    Syncs up action a2 with the previous action in the queue (a1).  Makes sure all the state
    variables that are not modified by the a2 action match those of a1.  Called when the queue
    is changed in any way.
*/

void Queue::syncAction(Action a1, Action &a2) {
    if(a2.typ != ROTATE){   // if not rotating, set to previous action's values
        a2.xrot = a1.xrot;
        a2.yrot = a1.yrot;
        a2.zrot = a1.zrot;
    }
    if(a2.typ != ZOOM){
        a2.scale = a1.scale;
    }
    if(a2.typ != SIMULATE){
        a2.frame = a1.frame;
    }

    if(a2.frame < a1.frame){
        a2.frame = a1.frame;
    }
}

/*! @brief Called when the queue is right clicked.

    Connected to the customContextMenuRequested signal from the queue. It creates and displays a menu.
*/
void Queue::provideContextMenu(QPoint p) {
    QTableWidgetItem* item = itemAt(p);
    if (item->column() == 0 && item->row() != 0) {
        rowToRemove = item->row();
        QMenu menu;
        QAction* remove = new QAction("Remove", this);
        connect(remove, SIGNAL(triggered()), this, SLOT(removeAction()));
        menu.addAction(remove);
        menu.exec(QCursor::pos());
    }
}

/*! @brief Removes a row from the table (effectively removing an action from the queue).

    After removal, this function updates the queue with all of the actions that follow it.

*/
void Queue::removeAction() {
    removeRow(rowToRemove);
    updateQueue(rowToRemove, rowCount());
}
