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
    h->setSectionResizeMode(QHeaderView::Stretch);
    setContextMenuPolicy(Qt::CustomContextMenu);
    QStringList headers;
    headers << "Action" << "Span" << "X Rotation" << "Y Rotation" << "Z Rotation"
            << "Zoom Factor" << "Frame Number";
    setHorizontalHeaderLabels(headers);
    Action act;
    act.typ = INITIALIZE;
    act.x = 0;
    act.y = 0;
    act.z = 0;
    act.scale = 1;
    act.frame = 0;
    act.span = 0;
    act.queueIndex = 0;
    addActionToQueue(act);
}

/*! @brief Used for calculating the state of the simulation after the action a1 is performed.

    The current state of the simulation is changed according to action a1.
    The resulting values are placed into action a2.

*/
void Queue::calculateNextState(Action a1, Action &a2) {
    a2.x = a1.x;
    a2.y = a1.y;
    a2.z = a1.z;
    a2.scale = a1.scale;
    a2.frame = a1.frame;
    switch (a2.typ) {
    case ROTATE:
        a2.x += a2.dx;
        a2.y += a2.dy;
        a2.z += a2.dz;
        break;
    case ZOOM:
        a2.prevScale = a2.scale;
        a2.scale = a2.newScale;
        break;
    case SIMULATE:
        a2.frame += a2.dFrame;
        break;
    }
}

/*! @brief Takes in an action and returns an array of strings describing the state of the simulation before the action.

    This function formats the members of the passed action - x, y, z, scale, and frame.
    It then puts them in a string array "strings" and returns it.

*/
std::vector<QString> getStateStrings(Action action) {
    QString x, y, z, sc, fr;
    x = QString::number(action.x, 'f', 2);
    y = QString::number(action.y, 'f', 2);
    z = QString::number(action.z, 'f', 2);
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
    QString text, x, y, z, sc, fr;
    switch (action.typ) {
    case ROTATE:
        x = QString::number(action.dx);
        y = QString::number(action.dy);
        z = QString::number(action.dz);
        text = QString("Rotate (%1, %2, %3) degrees").arg(x, y, z);
        break;
    case ZOOM:
        sc = QString::number(action.newScale);
        text = QString("Zoom to %1").arg(sc);
        break;
    case SIMULATE:
        fr = QString::number(action.dFrame);
        text = QString("Simulate %1 frames").arg(fr);
        break;
    case PAUSE:
        text = "Pause";
        break;
    case INITIALIZE:
        text = "Initialize";
        break;
    }

    QVariant v;
    v.setValue(action);
    QTableWidgetItem* act = new QTableWidgetItem(0);
    act->setText(text);
    act->setData(Qt::UserRole, v);
    act->setFlags(act->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem* span = new QTableWidgetItem(0);
    text = QString::number(action.span);
    span->setText(text);
    span->setFlags(span->flags() ^ Qt::ItemIsEditable);
    span->setTextAlignment(Qt::AlignHCenter);

    int index = action.queueIndex;
    if (action.typ != INITIALIZE || rowCount() == 0) {
        insertRow(index);
        setRowHeight(index, 20);
    }

    std::vector<QString> strings = getStateStrings(action);

    setItem(index, 0, act);
    setItem(index, 1, span);
    for (int i=0; i < 5; i++) {
        QTableWidgetItem* item = new QTableWidgetItem(0);
        item->setText(strings[i]);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignHCenter);
        setItem(index, i + 2, item);
    }
    updateStates(index + 1, rowCount());
}

/*! @brief Calculates and updates the states of all actions with indices between "start" and "end" in the queue.

    For each action, this function finds the state of the action before it, and then calculates the correct
    state for current action. It then updates the text in the table.
*/
void Queue::updateStates(int start, int end) {
    for (int i = start; i < end; i++) {
        QVariant v1 = item(i - 1, 0)->data(Qt::UserRole);
        Action a1 = v1.value<Action>();
        QVariant v2 = item(i, 0)->data(Qt::UserRole);
        Action a2 = v2.value<Action>();
        calculateNextState(a1, a2);
        a2.queueIndex = i;
        v2.setValue(a2);
        item(i, 0)->setData(Qt::UserRole, v2);
        std::vector<QString> strings = getStateStrings(a2);
        for (int j=0; j < 5; j++) item(i, j + 2)->setText(strings[j]);
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

/*! @brief Removes a row from the table (effectively removing an actin from the queue).

    After removal, this function updates the states of all of the actions that follow it.

*/
void Queue::removeAction() {
    removeRow(rowToRemove);
    updateStates(rowToRemove, rowCount());
}
