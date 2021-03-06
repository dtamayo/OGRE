/*!
 @file main.cpp
 @brief Runs the app and contains the main Doxygen documentation source.

 Creates an instance of MainWindow and gets things running by calling the member function MainWindow::show().
 One could include automatic testing here.
 @author Robert Douglas, Heming Ge, Daniel Tamayo
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

#include <QtGui/QApplication>
#include "OrbitalDisplays/MainWindow.h"
#include <iostream>
#include <QtCore/QDebug>
#include <QCommandLineOption>
#include <QCommandLineParser>
//#include <QCoreApplication>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("OGRE");
    QCoreApplication::setApplicationVersion("0.02");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main",
    "Visualizer for orbit simulations"));

    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption fileOption(QStringList() << "f" << "filename", QCoreApplication::translate("main", "Input file to load (Default is none--load within application)."), QCoreApplication::translate("main", "filename"), "");
    parser.addOption(fileOption);
    QCommandLineOption intOption(QStringList() << "i" << "integrator", QCoreApplication::translate("main", "Integrator used to generate input file (rebound, swift, dI). Default is rebound."), QCoreApplication::translate("main", "integrator"), "rebound");
    parser.addOption(intOption);
    QCommandLineOption typeOption(QStringList() << "t" << "type", QCoreApplication::translate("main", "Format of input file (osc or xyz). Default is osc."), QCoreApplication::translate("main", "type"), "osc");
    parser.addOption(typeOption);

    parser.process(a);

    QString integrator = parser.value(intOption);

    if(QString::compare(integrator, QString("rebound"), Qt::CaseInsensitive) != 0 && \
            QString::compare(integrator, QString("swift"), Qt::CaseInsensitive) != 0 && \
            QString::compare(integrator, QString("di"), Qt::CaseInsensitive) != 0)
    {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: integrator not supported. Must be rebound, swift, or dI")));
        parser.showHelp(1);
    }

    QString type = parser.value(typeOption);

    if(QString::compare(type, QString("osc"), Qt::CaseInsensitive) != 0 && \
            QString::compare(type, QString("xyz"), Qt::CaseInsensitive) != 0)
    {
        fprintf(stderr, "%s\n", qPrintable(QCoreApplication::translate("main", "Error: Input file type not supported. Must be osc or xyz")));
        parser.showHelp(1);
    }

    QString filename = parser.value(fileOption);

    Disp::MainWindow window(filename, integrator, type);

    window.show();

    //qDebug() << QCoreApplication::arguments().at(1);

    return a.exec();
}

/*! @page intro Read Me First!
  We have put considerable effort into the documentation in the hope that others might contribute to this open-source project.  Don't be
  overwhelmed by the length of the document!  It is not meant to be read from beginning to end.  We used the freely available software
  Doxygen (<a href="www.doxygen.org"> www.doxygen.org </a>) to automatically generate an index of all the classes, files, functions etc.
  used in the code, and to automatically cross-link them.  That's responsible for the bulk of the document.  We then wrote higher-level
  guides for implementing generic features like new buttons, which explain the steps and files involved, linking to specific parts of the
  code when needed.  Hopefully this will make it easy for you to pick up the parts of the code you need to implement your exciting new
  feature.  We want people to add to the code!

  To simplify generating a graphical user interface (GUI), and to ensure that our application would be portable to different operating
  systems, we used the programming language Qt.  For the graphics, we used OpenGL, which simplifies keeping track of rotations,
  translations etc., and has built-in drawing capabilities.  Two of us (HG and DT) had never used either of these languages before,
  so don't be intimidated---this guide should make them very easy to pick up!

  You should start with @ref layout to get an overview of the code.  @ref howto provides guides on how to add/edit different features
  of the code and is a great way to learn the code incrementally, only taking in the parts you need for your specific task.  If you want
  to learn about a specific function, class or file, you'll be able to find it in the indices generated by Doxygen.  If you want to
  add/improve the documentation for this (or any other!) project, learn how to use Doxygen on the @ref dox page.
*/

/*! @page layout Introduction to Qt and Code Overview
    Qt is a powerful language for creating graphical user interfaces (GUIs).  A huge advantage of Qt is that it makes applications
    extremely portable.  When the code is built, Qt detects the machine type being used and generates code that is native to the
    computer's operating system.  So if the GUI includes a menu bar at the top, and is built on an apple machine, Qt will generate code
    to display an Apple-style menu bar---and likewise for Windows or Linux.

    The philosophy behind Qt is called event-driven programming,
    where the tracking of the user’s actions is done automatically behind the scenes.  The programmer only has to define “signals” for specific
    user actions, and connect each of those “signals” to “slots”, i.e., the action that the program should take in response to that specific
    signal.  For example, one might want to create a slider for the zoom in the GUI.  Qt makes this easy since it already
    contains code to draw a slider on the screen, monitor mouse input to see if the user moves the location of the slider, and to turn
    the slider location into a number.  You, the programmer, only have to define a signal for "user moved slider", and connect it to
    a specific function you write (the slot) that will update the zoom value and redraw the screen.  You can see a concrete example of this in
    @ref modsetdiag.

    Qt is also intuitively modular.  Everything you see on the screen when you run the application are different 'widgets', and widgets
    can have other widgets inside them.  So, more concretely, the whole window is of a widget type called QMainWindow.  The code defines
    a custom class MainWindow that inherits the functionality of QMainWindow, and then adds a number of member widgets.  For example,
    the screen which displays the orbits (and runs OpenGL) is a QGLWidget, and each of the number boxes on the right with up and down arrows,
    sliders, checkboxes etc. are also pre-packaged widgets themselves.  You can see that with relatively minimal effort, Qt allows you to
    mix and match a wide variety of pre-packaged widgets into your own custom GUI.  You then simply interface the GUI you create with your
    own code through the system of signals and slots described above.

    With this general background, take a look at @ref howto for specific walkthroughs to implement your ideas!  Also, let me (Dan Tamayo)
    know if there are other How To's that you think would be widely useful, and we will try to put one together!
*/

/*! @page howto How do I...

  1.  @ref modsetdiag
  2.  @ref add2ndorb
  3.  @ref addmenuitem
  4.  @ref addbutton
  5.  @ref adddialog
  6.  @ref modqueue
  7.  @ref addclass

@section modsetdiag How do I add/edit buttons on the right of the window?
The settings dialog is the list of options and buttons to the right of the QGLWidget (the area that displays the orbits).  To add/edit this
interface, you have to work primarily with SettingsDialog.h and SettingsDialog.cpp.  We will walk through how the top widget (that says
'X rotation') is implemented.

The first thing you have to do when adding a widget is declare it in the class definition of SettingsDialog in SettingsDialog.h.

This can be done under “public:” or “private:”, depending on whether or not you want outside classes to have access to it).  Declaring objects
is easy; the format is
@verbatim
<Class name>*<instance name>;
@endverbatim
e.g.,
@code
QDoubleSpinBox* xRotationBox;
@endcode

Note the asterisk after the class name - we are declaring a pointer to a QDoubleSpinBox because we would like for it to be stored on the heap
(more on this later).  The QDoubleSpinBox in this example is just a box holding a number that can be edited by the user, and an up and down
arrow to the right of it that can alternatively be used to increase or decrease the value in the box.

Qt is widely used and has a big variety widget classes, so if you are looking to add an object that is different from the objects that
have already been added you can probably find what you need quickly by googling it. Qt will most likely have exactly what you are looking
for---if it doesn’t, you can probably modify or extend an existing Qt class to fit your needs. Keep in mind that if you want to use a new
class in SettingsDialog.h, you must first include it at the top, e.g.,
@code
#include <QtGui/QDoubleSpinBox>
@endcode

Once the widgets are defined, there are three steps that are done in member functions in SettingsDialog.cpp called internally by the
SettingsDialog constructor RobD::SettingsDialog::SettingsDialog().  See the functions' respective documentation for how they should be edited.
They each follow our example from above.

1.  Initialize the widgets.  This is done by RobD::SettingsDialog::setupUI().

2.  Make the widgets show up.  This is done by RobD::SettingsDialog::layoutControls().

3.  Plug the widget into your own code---so that when the user interacts with the widget, the code executes the appropriate action.  If you
are unfamiliar with signals and slots, check out @ref sigslots.

This example is a bit unorthodox, because the signal is not being
connected to a slot; instead, it is being connected to another signal. The second signal is attached to SettingsDialog (this), and is
called setXRot(). All this means is that the setXRot() signal from SettingsDialog will be sent whenever xRotationBox’s value changes.
At this point, nothing has actually been accomplished - a signal has been connected to another signal, but nothing has been executed.
To have something react to the signal, eventually the signal will have to be connected to a slot. In this example, this connection is made
in OrbitalAnimationDriver.cpp, under RobD::OrbitalAnimationDriver::makeConnections().

@code
connect(settingsDialog, SIGNAL(setXRot(double)), orbitalAnimator, SLOT(setXRot(double)));
@endcode

Note: this is slightly confusing because the signal and the slot have the same name. However, one is defined as a signal in RobD::SettingsDialog,
and the other is defined as a slot in RobD::OrbitalAnimator.

This time, the signal is attached to an instance of RobD::SettingsDialog (settingsDialog), and is user-defined (setXRot() - see above).
The slot is attached to an instance of OrbitalAnimator (orbitalAnimator), and is also user-defined (setXRot() - see OrtbitalAnimator.cpp).
Here, we have connected the signal sent from SetingsDialog to a slot defined in OrbitalAnimator. Overall, this means that whenever
xRotationBox’s value changes, the setXRot() method in OrbitalAnimator.cpp will be called.

@section add2ndorb How do I add a second orbital displayer to the window?

The part of the application that displays orbits is an instance of the RobD::OrbitalAnimator class.  The window as a whole is an instance of the
RobD::MainWindow class.  Finally, the RobD::OrbitalAnimator class interfaces with the rest of the program through the RobD::OrbitalAnimationDriver class.

A RobD::OrbitalAnimationDriver is instantiated in the constructor RobD::MainWindow::MainWindow() in MainWindow.h.  To add a second orbital display,
simply declare another instance of RobD::OrbitalAnimationDriver.  It will now appear in the upper left hand corner of the window - this will certainly
not look very good.

The organization of widgets in Qt is greatly simplified by their built-in layouts.  The layouts are created in RobD::MainWindow::createUIElements() and
set up in RobD::MainWindow::setupUIElements().  Once you choose the layout to which you want to add your second OrbitalAnimationDriver, call

@verbatim
<layout name>->addWidget(driver name)
@endverbatim

The current OrbitalAnimationDriver (driver) is in the visualizerLayout - if the new one is also added to this layout, it will show up next to the old one.
If you're interested in more layout options, check out the Qt Layout documentation: <a href="http://qt-project.org/doc/qt-4.8/layout.html">
http://qt-project.org/doc/qt-4.8/layout.html </a>.

Note that the settings dialog (buttons including the zoom slider) and the queue (the table at the bottom) are connected to the current OrbitalAnimator.
If another OrbitalAnimator is added in OrbitalAnimationDriver, it will not interact with either (it will interact with the mouse, however, since that's handled within
OrbitalAnimator).  If you don't need to control them individually, then you can connect the settings dialog and the queue to both OrbitalAnimators.
This can be done easily. Say the second OrbitalAnimator in OrbitalAnimationDriver is named orbitalAnimator2. Then, for every function call on orbitalAnimator in
OrbitalAnimationDriver.cpp, make the exact same call on orbitalAnimator2.
Another option may be to add another settings dialog and another queue.  To do this just duplicate the code for adding the current settings dialog
and queue using different names (see RobD::MainWindow::MainWindow()). In order to control where they appear, see RobD::MainWindow::setUIElements().
The additional settingsDialog and driver will have to be added to the appropriate layout using addWidget(). See the documentation for where each layout displays its
contents.

@section addmenuitem How do I add a menu bar item?

To add an item to the menu bar, first declare a new QAction for the item you want to add. This can be done in MainWindow.h under "private."
Then, in MainWindow.cpp, initialize the QAction in the method RobD::MainWindow::createMenuOptions().  Initial settings for the action are
set in RobD::MainWindow::setupMenuOptions().  The QAction itself should then be added to a menu in RobD::MainWindow::setupMenus().
Currently, there are two menus: file and options. Adding another menu can be done in the same format as the current ones, in RobD::MainWindow::setupMenus().

Finally, the QAction needs to be linked to the function (the SLOT) in RobD::MainWindow::makeConnections() that you want executed whenever that menu
item is clicked and a SIGNAL is sent out.  You would write this function (the SLOT) in RobD::MainWindow, or in any other class included
in RobD::MainWindow.  The SLOT also needs to be defined under either "private slots" or "public slots" in MainWindow.h.

@section addbutton How do I add a button to the window?

To add a button, you need to declare a new QPushButton for the button you want to add.  This should be done in MainWindow.h under "private."
Then, in MainWindow.cpp, initialize the QPushButton in the method RobD::MainWindow::setupButtons().  Then the button has to be added to a layout in
RobD::MainWindow::setupUIElements().  You could add it to one of the existing layouts, or you could make a new layout in
RobD::MainWindow::createUIElements().  For a very brief introduction to layouts and a link to more general Qt layout documentation,
see RobD::SettingsDialog::layoutControls().

Finally, the QPushButton needs to be linked to the function (the SLOT) in RobD::MainWindow::makeConnections() that you want executed whenever that menu
item is clicked and a SIGNAL is sent out.  You would write this function (the SLOT) in RobD::MainWindow, or in any other class included
in RobD::MainWindow.  The SLOT also needs to be defined under either "private slots" or "public slots" in MainWindow.h.

@section adddialog How do I pop up a dialog when a button or menu item is pressed?

For example, when you add a rotation to the queue, a dialog pops up prompting you for values.  Doing this is easy with Qt.  Let's assume you have already
set up your button or menu item and connected its SIGNAL that says it's been activated to a SLOT (see @ref addmenuitem and @ref addbutton for how to
do this).  Now you have to modify your function that acts as the SLOT in MainWindow.cpp.  We will follow the example of the add button for the different
rotation/zoom/etc. actions you can add to the queue.  The SLOT connected to this button is RobD::MainWindow::launchAddActionDialog().

Different dialogs in Qt inherit from the more general QDialog. This class has a method called exec() which makes the dialog show up.  The dialog is just
another window, so buttons, layouts, widgets, etc. can be added to it.  So, in our example, the add button will pop up a different dialog depending on
which action is selected to its left.  All the widgets for those dialogs are defined in QueueActionDialog.h, and are set up in QueueActionDialog.cpp,
analogously to the widgets added in the SettingsDialog.  You can see how this works in @ref modsetdiag.

Going back to the SLOT RobD::MainWindow::launchAddActionDialog(), it makes an instance of RobD::QueueActionDialog called dialog, and then uses Qt's
built-in exec() member function:

@code
if (dialog.exec() == QDialog::Accepted) {
@endcode

dialog.exec() will launch the dialog, and will run sending out SIGNALS for the user's actions until a SLOT that calls accepted() causes exec() to return.
In this case, a QPushButton called add is added in the constructor RobD::QueueActionDialog::QueueActionDialog(), and its SIGNAL clicked() (defined
by Qt and triggered whenever the button is clicked) is connected to the SLOT RobD::QueueActionDialog::setValues().  The SLOT then assigns the values
in the respective spinboxes (accessed through the member function <spinbox name>->value()), and calls accepted() to terminate the dialog.  If this occurs
dialog.exec() will return QDialog::Accepted, and the above if statement will evaluate to TRUE.  If the user exits the dialog without clicking 'Add',
it will be FALSE.

A useful subclass of QDialog is QFileDialog. QFileDialog is a dialog that prompts the user to select a file from a local directory.  This happens when
you hit open simulation in the file menu.  The selected file can be accessed using the method selectedFiles().first(). See how this is done in
RobD::MainWindow::openSimulation().

@section modqueue How do I modify the queue?

The queue is simply a QTableWidget. Each QTableWidgetItem represents an Action (struct found in QueueActionDialog.h).
The data for the action is stored in the item in the first column of each row.
To extract the data, simply call data(Qt::UserRole) on a QTableWidgetItem in the first column.
For an example, see OrbitalAnimator::playbackQueue().

To add a different type of action to the queue, see RobD::MainWindow::setupActionSelector().
Add another line following the same format, except use the new action name.
In QueueActionDialog.cpp, another case will have to be added, both in the constructor and QueueActionDialog::setValues().
Here, the dialog will be defined for the new action (add the appropriate input widgets and store them in appropriate variables in QueueActionDialog::setValues()).
Finally, modify RobD::OrbitalAnimator::performAction() by adding another case just like in QueueActionDialog.
This case will define what the OrbitalAnimator should do when the action is performed.

@section addclass How do I add a new class?

Adding a new class is straightforward. Right click the folder to which you want to add the class. There should be
and option "add new..." Select C++ class and follow the steps. Qt Creator should add .h and .cpp files to the
appropriate folders. Include the .h file in other files in order to use the class.

@section addReader How do I add a new reader?

To add a new reader, first create a new class in OrbitalReaders (see @ref addclass). Then, assuming that the new file
is similar in format to .swift or .dI files, you can closely emulate the code in DIReader or SwiftReader. The
header files should be pretty much the exact same (if the file has a header, copy DIReader; if it does not, copy
SwiftReader). In the source file, you may need to modify the QRegExp lineparser. Make it so that it has as many
rows as your file has columns. Each row represents a QRegExp for a whitespace and a number. They are defined at
the top of the file (you can modify these definitions if there are different symbols in your file). Then, in
read results, you can switch the order in which the numbers are stored depending on what the columns in your file
represent. Once you have created the reader, you must add it in as an option in OpenSimulationDialog.cpp. Follow
the format of the other readers and use the addItem() function to add it to the QComboBox selectFileType. Then go
to Disp::OrbitalAnimationDriver::setSimulationData() and add another if statement testing for your new file type.
Call your reader in the if statement, and you're all set.
*/

/*! @page General Qt Topics

@section sigslots Signals & Slots

In order for your widgets to interact with another part of the application (probably the QGLWidget), they must be able to communicate
with other classes and trigger specific function calls. Qt has a feature designed specifically for this---signals and slots,
described generally in @ref layout.

The idea behind this is simple. You can define “signals” and “slots” in any class that extends QObject (QObject is very general---QWidget
inherits from it, so all the widgets we talk about are QObjects).  Just add them in the widget's class definition under the keyword
“signals:” or “slots:”). Then you can connect one object’s signal to another object’s slot using the connect() method:

@verbatim
connect(<object a>, SIGNAL(<signal>), <object b>, SLOT(<slot>));
@endverbatim

Whenever the signal for object a is triggered, the slot for object b will be executed. This method can be called from any class that extends
QObject. For example, in RobD::SettingsDialog::makeConnections() :

@code
connect(xRotationBox, SIGNAL(valueChanged(double)), this, SIGNAL(setXRot(double)));
@endcode

In this example, the signal is attached to the QDoubleSpinBox xRotationBox. This signal in particular is valueChanged(). Note that this
signal is automatically defined by QDoubleSpinBox. If you are using one of Qt’s built in classes, you can see a dropdown menu of all of
its defined signals by typing

@verbatim
connect(<object name>, SIGNAL(
@endverbatim

and Qt Creator will show you all the available choices.  The valueChanged() signal is sent anytime xRotationBox’s value changes.
*/

/*! @page opengl OpenGL Basics

    Our simple description is below.  You can google any openGL commands, and for a more detailed introduction to using OpenGL within Qt \
    (which our code largely follows), see <a href="http://qt-project.org/doc/qt-4.8/opengl-hellogl.html">
    http://qt-project.org/doc/qt-4.8/opengl-hellogl.html </a>, as well as the documentation for the GL Widget within Qt:
    <a href="http://harmattan-dev.nokia.com/docs/library/html/qt4/qglwidget.html">
    http://harmattan-dev.nokia.com/docs/library/html/qt4/qglwidget.html </a>.  For even more detail specific to OpenGL, check out these
    two lectures from a graphics course at Cornell:  <a href="http://www.cs.cornell.edu/courses/CS4620/2012fa/lectures/09opengl01.pdf">
    http://www.cs.cornell.edu/courses/CS4620/2012fa/lectures/09opengl01.pdf </a> and
    <a href="http://www.cs.cornell.edu/Courses/cs4620/2012fa/lectures/10opengl02.pdf">
    http://www.cs.cornell.edu/Courses/cs4620/2012fa/lectures/10opengl02.pdf </a>.

    @section shapes Drawing Shapes

    OpenGL makes it easy to draw basic shapes and then to move and orient them as you like in the scene you render.  To draw a shape, you
    start with a command like in OrbitalAnimator::drawPlane()

    @code
    glBegin(GL_QUADS)
    @endcode

    This tells OpenGL that you want to start drawing a GL_QUAD or a quadrilateral (you can find other suggestively named available shapes
    in the code or in lecture 9 above).  You then supply the coordinates of as many vertices (corners) as are needed to draw the shape.
    In our example:

    @code
        glBegin(GL_QUADS);
        glVertex3f(-extent, -extent, 0);
        glVertex3f(-extent, extent, 0);
        glVertex3f(extent, extent, 0);
        glVertex3f(extent, -extent, 0);
        glEnd();
    @endcode

    If, for example, extent=1, then our four vertices are at (x,y,z) = (-1,-1,0), (-1,1,0), (1,1,0) and (1,-1,0).  After supplying all the vertices,
    you call glEnd().  Note that you can't draw curved surfaces in openGL.  You draw polygons with lots of vertices to simulate curved lines.

    @section rot Rotating the Scene and the ModelView Matrix

    Now that we have told openGL how to draw our square in coordinates convenient for us (in the plane of the square), we have to tell openGL where
    we would like that square drawn and how we would like it oriented.  This is determined by the ModelView matrix.  This matrix is a composition of
    a stack of transformations that you want applied to different objects in your scene.  Note that programmers have come up with a clever way of
    treating translations as a matrix transformation by representing 3-D points by 4-D quaternions, so translations also are incorporated into the
    ModelView matrix.

    An important thing to know about openGL is that commands that you enter apply to everything below them until a different command is encountered.
    So for example, toward the top of OrbitalAnimator::paintGL(), which sets up the drawing of everything in our scene, we have

    @code
        glRotatef(xrotation, 1, 0, 0);
        glRotatef(yrotation, 0, 1, 0);
        glRotatef(zrotation, 0, 0, 1);
    @endcode

    glRotatef(xrotation, 1, 0, 0) rotates the axes by xrotation degrees around the (1,0,0) axis, i.e., the x axis.  Since this is building up a
    composite matrix, the second rotation will rotate around the rotated (0,1,0) axis, i.e., the y-axis as it lies after the first rotation.
    Note that openGL is set up so that rotations (and other transformations) are executed from the top down (as opposed to linear algebra notation
    where the last matrix is the first to act on a vector).  These rotations are put at the top of OrbitalAnimator::PaintGL() to orient
    all objects that are drawn with the rotation values (xrotation, yrotation, zrotation) set by the user as he/she rotates things with the mouse.

    The openGL coordinate system is x pointing right, y pointing up and z pointing out of the screen to complete a right-handed triad.  We found it
    more intuitive to work with the traditional physics / celestial mechanics convention of x out of the screen, z up and y to the right.  So there
    are two rotations at the top of OrbitalAnimator::paintGL() to align the openGL system with our convention:

    @code
        glRotatef(-90, 1, 0, 0);
        glRotatef(-90, 0, 0, 1);
    @endcode

    Inevitably, one will want to apply different rotations to different objects.  For example, each rendered orbit should be rotated by its own set
    of Euler angles (i, $\Omega$, $\omega$).  But, as mentioned above, any rotation you do will get applied to all code below it.  Thus, when you tried
    to rotate the 2nd orbit, it would also be rotated by the 1st orbit's angles!  This is why each individual thing drawn in the code is sandwiched between
    calls to glPushMatrix() and glPopMatrix().  glPushMatrix() tells openGL to add transformations to a temporary stack.  You then draw your object with
    those transformations, and finally call glPopMatrix() to remove the temporary transformations from the stack so they won't apply to other objects drawn
    subsequently.  See OrbitalAnimator::PaintGL() for examples.

    @section proj The Projection Matrix

    There are several matrices other than the ModelView Matrix.  The only extra one our code uses is the projection matrix, which defines how a 3-D scene should
    be projected onto the 2-D screen.  In openGL, you do this by defining the geometry of the volume you want the camera to see.  We chose an
    orthographic projection, which takes a rectangular prism as the viewing volume.  There are other options, like a frustrum (you can see the differences
    on slides 16 and 34 here: <a href="http://www.cs.cornell.edu/Courses/cs4620/2012fa/lectures/10opengl02.pdf">
    http://www.cs.cornell.edu/Courses/cs4620/2012fa/lectures/10opengl02.pdf </a>.

    Any transformations you call in openGL keep getting stacked onto the current matrix, so to set up the projection matrix you have to explicitly
    switch between matrices so you don't add a transformation to the wrong stack!  The command is glMatrixMode() and you pass it GL_PROJECTION or GL_MODELVIEW.
    The projection matrix is set up in OrbitalAnimator::resizeGL().  This gets called when the object is created, and whenever the window is resized.  You first
    use glLoadIdentity() to load the identity matrix into the projection matrix, and then call glOrtho() to set the volume that the viewer will be able to see.
    The arguments to glOrtho() define the planes of the rectangular prism (left side, right side, top side, bottom side, near side, far side) that the viewer can see,
    in relation to the camera position (see below), e.g.

    @code
    glOrtho(-0.5, +0.5, -0.5, +0.5, 1.0, 40.0);
    @endcode

    This sets the size of the viewable space, and objects in the simulation should be scaled to this size.  Notice that the last two values, for the front and back side of
    the prism along the z axis (in and out of the screen) are positive, since they are relative to the camera, and openGL wants your viewable volume in front of you.  This
    choice goes from 1 unit in front of you, to 40 units in front of you.  These boundaries act as clipping planes--any particles going outside these boundaries won't be
    rendered.  I think if you put one of the clipping planes behind the camera with a negative value, then the z axis gets flipped--this was causing us problems.

    Finally, in OrbitalAnimator::resizeGL(), after switching back to the ModelView matrix, the camera position is defined through

    @code
    gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
    @endcode

    The first three arguments to this function determine where the camera is located. In this case, the camera is located 30 units along the positive openGL Z axis
    (out of the screen).  The next three arguments determine the point that the camera should look toward (in this case the origin).  The final three arguments
    determine a vector that defines the "up" direction (the openGL y axis, going vertically up the screen).

    @section initgl Intializing OpenGL

    OrbitalAnimator::initializeGL() initializes openGL.  There are tons of possible options, and we don't understand the details, but at least roughly:
    glClearColor sets the color for the background when glClear is called.  Here it is set to black.
    GL_DEPTH_TEST is a flag which tells openGL to test whether objects are in front or behind each other (so that if an object is behind another, it is not
    rendered).  The last two calls are in order to approximate transparency.  OpenGL does not directly support this, but there is a standard approach that blends
    colors according to their alpha (or opacity) value (the 4th number following the rgb values whenever a color is given).  It is described in more detail
    here:  <a href="http://www.opengl.org/archives/resources/faq/technical/transparency.htm"> http://www.opengl.org/archives/resources/faq/technical/transparency.htm </a>.

    For other options and more information, you could start here:
    <a href="http://qt-project.org/doc/qt-4.8/opengl-hellogl.html"> http://qt-project.org/doc/qt-4.8/opengl-hellogl.html </a> and
    <a href="http://harmattan-dev.nokia.com/docs/library/html/qt4/qglwidget.html"> http://harmattan-dev.nokia.com/docs/library/html/qt4/qglwidget.html </a>.

    @section othergl Other functions used

    We now describe the remaining miscellaneous openGL functions used in the main drawing function, OrbitalAnimator::paintGL().  It first calls

    @code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    @endcode

    We couldn't say exactly what each of these options does, but it basically clears all the different buffers in openGL, which seems like a safe way to go :)  If
    you are interested in this, see <a href="http://www.opengl.org/sdk/docs/man/xhtml/glClear.xml"> http://www.opengl.org/sdk/docs/man/xhtml/glClear.xml </a>.

    glScalef() is an important function that changes the size of an object by the passed factors along x, y, and z.  OrbitalAnimator::paintGL() determines that most positive
    and negative x,y and z values to set the scale and make sure that the whole simulation is viewable in the window.  It then sets all the x,y, and z scale factors to the
    RECIPROCAL of the largest dimension of the three (ensuring that everything fits, and that everything is stretched equally in the 3 directions).  This is done with

    @code
    glScalef(scaleFactor * orbitScaleFactor, scaleFactor * orbitScaleFactor, scaleFactor * orbitScaleFactor);
    @endcode

    scaleFactor is the zoom factor from the SettingsDialog, which by default is 1.  orbitScaleFactor is the reciprocal of the largest dimension.  For example, if the largest x
    value in the simulation is 1000, glScalef() would scale all distances by 1 (scaleFactor) * 1/1000 (orbitScaleFactor), so that the x value of this furthest particle would be
    1, and every other one would have a value < 1.  The user can then modify this by changing scaleFactor within the SettingsDialog and zoom in or out.

    Finally, glColor4f() takes 4 float values:  the r, g, and b values you want (0-1), and the alpha value that sets the opacity (0-1).  This will now be the color used
    for everything until it's called again, or unless it's sandwiched in a push/pop matrix combo (as it is in OrbitalAnimator::paintGL()).

*/

/*! @page dox Using Doxygen for Documentation

This manual was generated with Doxygen (<a href="www.doxygen.org"> www.doxygen.org </a>).  You write comments using a special format
  within the code, and then doxygen automatically generates and cross-links them into a document.  The doxygen manual can be found here:
  <a href="http://www.stack.nl/~dimitri/doxygen/manual/index.html"> http://www.stack.nl/~dimitri/doxygen/manual/index.html</a>.  It's long, so
  this is a brief introduction on the most useful commands that I have found.  The code used to
  generate this page can be found in OrbitalDisplaysRunner/Sources/main.cpp, and might provide a good template for setting up new pages.

  @section pages How to add new pages to the documentation
    To add a new page to the documentation (like this one!), use the \@page command.  See this link for syntax, and for an example
    of what it looks like in the HTML layout :

    <a href="http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdpage"> http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdpage </a>

    You can add sections (like the current section @ref pages) and subsections to the page with the \@section and \@subsection commands:

    <a href="http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdsection"> http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdsection </a>

    <a href="http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdsubsection"> http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdsubsection </a>

    All three of these commands (\@page, \@section, and \@subsection) have the same syntax of "command label title", for example

    @verbatim
    @section pages How to add new pages to the documentation
    @endverbatim

    Here, 'pages' is the internal label for the section, and all the following words get displayed as the title for the section in the
    documentation.  The label is there so that you can reference the section from other parts of the
    documentation with the \@ref command (<a href="http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdref">
    http://www.stack.nl/~dimitri/doxygen/manual/commands.html#cmdref </a>), as above with the link in blue where I referenced it using:

    @verbatim
    You can add sections (like the current section @ref pages)
    @endverbatim

    @b Important!  The Doxygen manual says you should only use lowercase words for the names of pages, sections or subsections.  This forces you to use the @ref command
    to reference them and avoids Doxygen unwittingly linking to them.

@section doc Documenting the code
    There are a number of ways to put in comment blocks that you can read about in the doxygen manual.  I'll only talk about one.  A comment block
    looks like a C-style comment block with an extra ! after the /*:

    @verbatim
    /*! Commands and comments
        ...
        ...
    * /
    @endverbatim

    There should be no space between the final * and /, but if I removed it, doxygen would think that the code generating this page should end
    at that point!  You should add such a comment block \em before any function, class, or file you want to document.  In the case of a file
    you would put it at the beginning of the file.

    For example, if you look at the source code for SettingsDialog.cpp, at the top we start with

    @verbatim
    /*! @file SettingsDialog.cpp
    @brief implementation for the class SettingsDialog, the column of boxes and sliders on the right of the window.

    @see SettingsDialog
    * /
    @endverbatim

    The brief description will show up below the entry for SettingsDialog.cpp in the file table of contents, so it's useful to try and consicely
    pack as much information as possible.  Don't just repeat what is self-explanatory.

    Then we document the namespace:

    @verbatim
    /*! @brief main namespace for the project
    * /
    namespace RobD
    @endverbatim

    If you don't document the namespace, then any classes and functions in the namespace won't get read by doxygen (even if they're documented)!
    Then if you scroll down to the documentation above setupUI(), the first few lines read:

    @verbatim
    /*! @brief Instantiates and sets default values and ranges for all the widgets in SettingsDialog.

    @sa @ref modsetdiag

    This function is called internally by the constructor RobD::SettingsDialog::SettingsDialog().
    ...
    @endverbatim

    Doxygen will put everything up until the first blank line into the brief description.  Everything below goes into the detailed description,
    which won't show up in the table of contents.  Here the detailed description starts with \@sa.  \@sa is the see also command.  It's probably
    useful for users to link relevant parts of the code as much as possible, and Doxygen makes that easy!  By the way, if I'd typed \@sa in
    the source, it would interpret as a \@sa command, but you can put a \\ in front of any \@ symbol so that it's not read as a command.

@section doxlinking Linking

Doxygen makes linking between different parts of the documentation very easy.  Here is the link to the page on this in the manual:
    <a href="http://www.stack.nl/~dimitri/doxygen/manual/autolink.html"> http://www.stack.nl/~dimitri/doxygen/manual/autolink.html </a>.
    Basically, any time you type a word that has at least one capital letter, if it refers to a \em documented class it will get cross-linked.
    If the word has a dot (like SettingsDialog.cpp), it will get interpreted as a file, and if the file is documented it will link.  For member
    functions and variables, you have to reference them with their parent class (and namespace!) so that doxygen can find them, e.g.
    RobD::SettingsDialog::setupUI().  Even if the function takes parameters, you can enter the function with empty brackets (), as long as the
    function isn't overloaded so there's no ambiguity.  To enter a URL, do

    @verbatim
    <a href="http://www.google.com"> Google </a>
    @endverbatim

    This will link to google, but make the link read 'Google'.

@section doxinscode Inserting code

To insert snippets of code use a block like this:

@verbatim
@code
lines of code here
@endcode
@endverbatim

It will highlight c++ commands.  To enter blocks code exactly as you type them in, use

@code
@verbatim
pseudocode or whatever you want
@endverbatim
@endcode

@section doxsettings Important Doxygen Settings

You set these by going in Qt Creator to Tools -> Doxygen -> Edit Doxyfile, and going to the expert tab.

EXTRACT_PRIVATE Found under the 'build' topic on the left.  Set to yes, so that private member functions that are documented in the code are integrated into the documentation.

REPEAT_BRIEF Found under the 'project' topic.  Currently set to yes, but we could change our minds.  Yes causes the brief description to be repeated at the beginning
of the detailed description.

*/

/*! @page TODO

  Drawing Trails
  Drawing the normal vector to the orbit
  Parsing XYZ files
  Adding Translations
  Including FFMPEG and tying it into record button
  Adding fields to Orbit so that single orbits can be highlighted (with different colors or sizes)

*/

/*! @page Compiler Errors

  @section thread On Mac OS X, you might be loading two sets of Qt binaries into the same process. Check that all plugins are compiled against the right Qt binaries.
Export DYLD_PRINT_LIBRARIES=1 and check that only one set of binaries are being loaded.  QObject::moveToThread: Current thread (0x1040002c0) is not the object's
thread (0x1060d3f20). Cannot move to target thread (0x1040002c0)

I got this error on Mac when I tried to package all the libraries into the executable.  Afterwards, when I tried to compile, Qt was finding the Qt binaries both
in the application folder and in my system, and was getting confused.  I fixed it by removing the folder of the same name with Debug and some other text appended
to it that gets created by Qt when you build the project (in the same directory as the project), as this is the one that holds the executable that gets built.
Qt then creates a new folder that doesn't have the libraries packaged in as before.

*/
/*
The actions that can be added into the queue are: rotate, zoom, simulate, do nothing, and initialize.
There can only be one initialize action in the queue, and it is always in the first row.
Adding an initialize action into the queue is effectively modifying the current initialize action.
The other four actions are self explanatory - the user specifies the parameters for the action (x, y, z for rotate, number of frames for simulate, etc.) and
the amount of time over which the action is to be performed (span).
If the span is set to 100 (default), the action will produce 100 images when the queue is recorded.
*/

