/*!
* @file OpenSimulationDialog.cpp
 @brief Implementation for the OpenSimulationDialog class, used for loading simulations.
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

#include "OpenSimulationDialog.h"

OpenSimulationDialog::OpenSimulationDialog() : QDialog() {
    setWindowTitle("Open Simulation");
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QFormLayout* form = new QFormLayout;
    selectFileType = new QComboBox;
    selectFileType->addItem("Rebound",0);
    selectFileType->addItem("SWIFT", 0);
    selectFileType->addItem("dI", 0);
    selectDataType = new QComboBox;
    //selectDataType->addItem("Orbital Elements");
    //selectDataType->addItem("XYZ");
    QHBoxLayout* fileSelectorLayout = new QHBoxLayout;
    fileSelector = new QLineEdit;
    QPushButton* browse = new QPushButton("Browse", this);
    fileSelectorLayout->addWidget(fileSelector);
    fileSelectorLayout->addWidget(browse);
    drawFullOrbit = new QCheckBox;
    form->addRow("Select file type: ", selectFileType);
    //form->addRow("Select data type: ", selectDataType);
    form->addRow("Select file: ", fileSelectorLayout);
    form->addRow("Draw full orbit: ", drawFullOrbit);
    QHBoxLayout* buttons = new QHBoxLayout;
    QPushButton* acceptButton = new QPushButton("Accept", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    buttons->addWidget(cancelButton);
    buttons->addWidget(acceptButton);
    mainLayout->addLayout(form);
    mainLayout->addLayout(buttons);
    setLayout(mainLayout);
    connect(browse, SIGNAL(clicked()), this, SLOT(openFileDialog()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void OpenSimulationDialog::openFileDialog() {
    QFileDialog dlg;
    if (dlg.exec() == QDialog::Accepted) fileSelector->setText(dlg.selectedFiles().first());
}

