/*!
 @file OrbitalAnimator.cpp
 @brief Implementation of class that displays the orbits.  Extends QGLWidget. Instantiated by OrbitalAnimationDriver. Instantiates settingsDialog.
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

#include "OrbitalAnimator.h"

#define COORD_X 0
#define COORD_Y 1
#define COORD_Z 2
#define W 3
#define OMEGA 4
#define I 5

namespace Disp
{
    /*! @brief Constructor for the OrbitalAnimator

        This constructor is called from OrbitalAnimationDriver::setupUI().  The OrbitalAnimator extends the QGLWidget class.
        The constructor initializes default values and prepares the arrays cosfs and sinfs.
    */
    OrbitalAnimator::OrbitalAnimator(OrbitalAnimatorSettings& settings_, QWidget *parent/* = 0*/)
        : QGLWidget(parent)
        , simulationDataLoaded(false)
        , equatorialDataLoaded(false)
        , eclipticDataLoaded(false)
        , settings(settings_)
        , currentIndex(0)
        , simulationSize(0)
        , scaleFactor(1)
        , xrotation(0)
        , yrotation(0)
        , zrotation(0)
        , centralBody(20, 20, 0)
        , loading(false)
        , recording(false)
        , pictureNumber(0)
        , trailLength(60)
        , drawFullOrbit(false)
    {
        QFont newFont(font());
        newFont.setPointSize(30);
        setFont(newFont);

        obl = degToRads(97.77);
        zEq = Point3d(0, sin(obl), cos(obl));
        xEq = Point3d(1, 0, 0);
        eqRotAngles = angularMapping(zEq, xEq);

        prepfs();
        setMouseTracking(true);
        connect(&settings, SIGNAL(changed()), this, SLOT(updateGL()));
    }

    /*! @brief Initializes the settings dialog

        Called from OrbitalAnimationDriver::setupUI().  It creates a settingsDialog with the passed settings, puts it into a widget using a simple layout and returns it.
    */
    QWidget* OrbitalAnimator::setupUI(OrbitalAnimatorSettings& animatorSettings) {
        controlsWidget = new QWidget;
        settingsDialog = new SettingsDialog(animatorSettings, controlsWidget);
        QVBoxLayout* controlsLayout = new QVBoxLayout;
        controlsLayout->addWidget(settingsDialog);
        controlsWidget->setLayout(controlsLayout);
        return controlsWidget;
    }

    /*! @brief Sets up OpenGL context rendering flags

        This function is part of QGLWidget, which this class inherits, but we modify it. It sets up flags to be used when the scene is rendered.  See @ref proj in
        @ref opengl for a description of the functions used.
    */
    void OrbitalAnimator::initializeGL() {
        glEnable(GL_DEPTH_TEST);

        /*GLfloat values[2];
        glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values);
        qDebug() << "GL_LINE_WIDTH_GRANULARITY value is " << values[0];
        glGetFloatv (GL_LINE_WIDTH_RANGE, values);
        qDebug() << "GL_LINE_WIDTH_RANGE values are " << values[0] << "and" << values[1];*/

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glLineWidth(3.5);

        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    /*! @brief Resizes the OpenGL viewport

        This function is reimplemented from QGLWidget. It is called when the window is created
        and whenever it is resized.  It changes the window's dimensions, and
        sets up the projection matrix. See @ref proj in @ref opengl for a description of the projection
        matrix and the specific commands implemented below.
    */
    void OrbitalAnimator::resizeGL(int w, int h) {
        int newDim = std::min(w, h);
        glViewport(0, 0, (GLint)newDim, (GLint)newDim);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-0.5, +0.5, -0.5, +0.5, 1.0, 40.0);
        glMatrixMode(GL_MODELVIEW);
        gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
    }

    /*! @brief Renders the scene

        This function is called everytime updateGL() is called, and is where everything in the OrbitalAnimator is drawn.  For a description of the various
        functions used and an overview of openGL, see @ref opengl.

        The scale is determined by the first file loaded (whether it be a simulation file, equatorial file, or
        ecliptic file).  This is done in the respective updateEcliptic/Equatorial/SimulationCache functions.
        The scale is calculated by subtracting the most negative point from the most positive point in the
        file, and then taking the inverse.  See @ref othergl for more details on how the scale is set.
    */
    void OrbitalAnimator::paintGL() {
        glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        double orbitXScaleFactor = maximum.x - minimum.x;
        double orbitYScaleFactor = maximum.y - minimum.y;
        double orbitZScaleFactor = maximum.z - minimum.z;
        double orbitScaleFactor = 1. / std::max(orbitXScaleFactor, std::max(orbitYScaleFactor, orbitZScaleFactor));
        glScalef(scaleFactor * orbitScaleFactor, scaleFactor * orbitScaleFactor, scaleFactor * orbitScaleFactor);
        glRotatef(-90, 1, 0, 0); // Change x-y orbital plane to x-z Display plane for display
        glRotatef(-90, 0, 0, 1);
        glRotatef(xrotation, 1, 0, 0);
        glRotatef(yrotation, 0, 1, 0);
        glRotatef(zrotation, 0, 0, 1);
        if (settings.displayCoords() && simulationDataLoaded) drawCoords(coordLength);

        glPushMatrix();
        glColor4f(settings.centralBodyColor().red() / 255.,
                  settings.centralBodyColor().green() / 255.,
                  settings.centralBodyColor().blue() / 255.,
                  settings.centralBodyColor().alpha() / 255.);
        if (settings.displayCentralBody()) {
            centralBody.updateRadius(0.02 * coordLength);
            centralBody.draw();
        }
        glPopMatrix();

        for (size_t i = 0; i < equatorialOrbits.size(); ++i) {
            if (equatorialOrbits[i].frameStart <= currentIndex && equatorialOrbits[i].frameEnd >= currentIndex) {
                glPushMatrix();
                glRotatef(radsToDeg(eqRotAngles.phi), 0, 0, 1); // now do the three rotations to line up the axes
                glRotatef(radsToDeg(eqRotAngles.theta), 0, 1, 0);
                glRotatef(radsToDeg(eqRotAngles.psi), 0, 0, 1);
                glRotatef(equatorialOrbits[i].Omega, 0, 0, 1);
                glRotatef(equatorialOrbits[i].i, 1, 0, 0);
                glRotatef(equatorialOrbits[i].w, 0, 0, 1);
                glColor4f(equatorialOrbits[i].red / 255.,
                          equatorialOrbits[i].green / 255.,
                          equatorialOrbits[i].blue / 255.,
                          1.);
                drawOrbitalRing(equatorialOrbits[i].orbitCoords);
                glPopMatrix();
            }
        }

        for (size_t i = 0; i < eclipticOrbits.size(); ++i) {
            if (eclipticOrbits[i].frameStart <= currentIndex && eclipticOrbits[i].frameEnd >= currentIndex) {
                glPushMatrix();
                glRotatef(eclipticOrbits[i].Omega, 0, 0, 1);
                glRotatef(eclipticOrbits[i].i, 1, 0, 0);
                glRotatef(eclipticOrbits[i].w, 0, 0, 1);
                glColor4f(eclipticOrbits[i].red / 255.,
                          eclipticOrbits[i].green / 255.,
                          eclipticOrbits[i].blue / 255.,
                          1.);
                drawOrbitalRing(eclipticOrbits[i].orbitCoords);
                glPopMatrix();
            }
        }

        if (settings.displayMainOrbit() && simulationDataLoaded) {
            glPushMatrix();
            if (drawFullOrbit) {
                //drawOrbitalNormal();
                //drawParticle();
                drawOrbit();
            }
            else { /*drawTrail();*/ drawParticle(); }
            glPopMatrix();
        }
/*
        if(settings.displaySpinAxis() && simulationDataLoaded)
        {
            glPushMatrix();
            glColor4f(0, 0, 1., 1.);
            drawVector(zEq*coordLength);
            glPopMatrix();
        }
*/

        // DT: Here are plane drawing examples. For opacity, you should draw these are end. Not sure why, honestly
        //drawXZPlane(coordLength);
        //drawYZPlane(coordLength);
        //drawXYPlane(coordLength);

        if (loading) drawLoading<OpenGL>();
        if (!recording) drawStats<OpenGL>();
        glPopMatrix();
    }

    /*! @brief Draws a trail behind the particle

        This function currently does not work correctly and is not being called.
        Right now, this function draws the trail of the first particle in the XY plane.
    */
    void OrbitalAnimator::drawTrail() {
        glPushMatrix();
        glBegin(GL_LINE_STRIP);
        glColor4f(0.8, 0.4, 0.0, 1.0);
        OrbitData::const_iterator itr = orbitData.begin();
        for (int i = currentIndex < trailLength ? 0 : currentIndex - trailLength; i < currentIndex; i++) {
            glVertex3f((itr->second)[i].posInPlane.x,
                       (itr->second)[i].posInPlane.y,
                       (itr->second)[i].posInPlane.z);
        }
        glEnd();
        glPopMatrix();
    }

    /*! @brief Draws the particles

        This function draws all of the particles as spheres.
        It simply iterates through orbitData, calculates the positions, and draws the particles.
        This function does not get called if the full orbit is being drawn.
        Only works if Orbit::calculatePosition() has been called on the particle.
    */
    void OrbitalAnimator::drawParticle() {
        for (OrbitData::const_iterator itr = orbitData.begin(); itr != orbitData.end(); itr++) {
            if ((size_t)currentIndex < (itr->second).size()) {
                glPushMatrix();
                glColor4f((itr->second)[currentIndex].color.r,
                          (itr->second)[currentIndex].color.g,
                          (itr->second)[currentIndex].color.b,
                          (itr->second)[currentIndex].color.alpha);
                glRotatef((itr->second)[currentIndex].Omega, 0, 0, 1);
                glRotatef((itr->second)[currentIndex].i, 1, 0, 0);
                glRotatef((itr->second)[currentIndex].w, 0, 0, 1);
                glTranslatef((itr->second)[currentIndex].posInPlane.x,
                             (itr->second)[currentIndex].posInPlane.y,
                             (itr->second)[currentIndex].posInPlane.z);
                Sphere obj(20, 20, (itr->second)[currentIndex].particleSize * coordLength);
                obj.draw();
                glPopMatrix();
            }
        }
    }

    /*! @brief Draws the full orbit of the first particle

        This function draws the whole orbit of the first particle as a circle.
        Only works if Orbit::calculateOrbit() has been called on the particle.
    */
    void OrbitalAnimator::drawOrbit() {
        int ctr = 0;
        for (OrbitData::const_iterator itr = orbitData.begin(); itr != orbitData.end(); itr++) { // iterate over particles
            ctr++;
            if(ctr != 4) { continue; }

            if ((size_t)currentIndex < (itr->second).size()) {
                /*glColor4f(settings.orbitalPlaneColor().red() / 255.,
                          settings.orbitalPlaneColor().green() / 255.,
                          settings.orbitalPlaneColor().blue() / 255.,
                          settings.orbitalPlaneColor().alpha() / 255.);
                glPushMatrix();
                glRotatef((itr->second)[currentIndex].Omega, 0, 0, 1);
                glRotatef((itr->second)[currentIndex].i, 1, 0, 0);
                glRotatef((itr->second)[currentIndex].w, 0, 0, 1);
                glBegin(GL_POLYGON);
                for (int f = 0; f < 360; ++f) {
                    glVertex3f((itr->second)[currentIndex].orbitCoords[f].x,
                               (itr->second)[currentIndex].orbitCoords[f].y,
                               (itr->second)[currentIndex].orbitCoords[f].z);
                }
                glEnd();*/

                glColor4f(settings.orbitColor().red() / 255.,
                          settings.orbitColor().green() / 255.,
                          settings.orbitColor().blue() / 255.,
                          settings.orbitColor().alpha() / 255.);
                glBegin(GL_LINE_STRIP);
                for (int f = 0; f < 360; ++f) {
                    glVertex3f((itr->second)[currentIndex].orbitCoords[f].x,
                               (itr->second)[currentIndex].orbitCoords[f].y,
                               (itr->second)[currentIndex].orbitCoords[f].z);
                    qDebug() << ctr << (itr->second)[currentIndex].orbitCoords[f].x << (itr->second)[currentIndex].orbitCoords[f].y << (itr->second)[currentIndex].orbitCoords[f].z;
                }

                glVertex3f((itr->second)[currentIndex].orbitCoords[0].x,
                           (itr->second)[currentIndex].orbitCoords[0].y,
                           (itr->second)[currentIndex].orbitCoords[0].z);
                glEnd();
                glPopMatrix();
            }
        }
    }

/*
    void OrbitalAnimator::drawOrbitalNormal()
    {
        glColor4f(settings.orbitColor().red() / 255.,
                  settings.orbitColor().green() / 255.,
                  settings.orbitColor().blue() / 255.,
                  settings.orbitColor().alpha() / 255.);
        drawVector(normals[currentIndex] * normalsScalar);
    }
*/

    /*! @brief Calls Orbit::calculateOrbit() on all the particles in eclipticOrbits

        This function is part of the loading process for ecliptic orbits.
        It readies the Orbit objects in eclipticOrbits for drawing.
        The scale is set by ecliptic if nothing else is currently loaded.
        This function is called from Disp::OrbitalAnimationDriver::setEclipticData().
    */
    void OrbitalAnimator::updateEclipticCache(StaticDisplayOrbits const& eco) {
        eclipticOrbits = eco;
        if (nothingLoaded()) { maximum = Point3d::minPoint(); minimum = Point3d::maxPoint(); }

        for (size_t i = 0; i < eclipticOrbits.size(); i++) eclipticOrbits[i].calculateOrbit(cosfs, sinfs);

        if (nothingLoaded()) {
            for (size_t i = 0; i < eclipticOrbits.size(); ++i)
            {
                minimum = findMin(eclipticOrbits[i].posInPlane, minimum);
                maximum = findMax(eclipticOrbits[i].posInPlane, maximum);
            }
        }
        loading = false;
        updateGL();
    }

    /*! @brief Calls Orbit::calculateOrbit() on all the particles in equatorialOrbits

        This function is part of the loading process for equatorial orbits.
        It readies the Orbit objects in equatorialOrbits for drawing.
        The scale is set by equatorialOrbits if nothing else is currently loaded.
        This function is called from Disp::OrbitalAnimationDriver::setEquatorialData().
    */
    void OrbitalAnimator::updateEquatorialCache(StaticDisplayOrbits const& eqo) {
        equatorialOrbits = eqo;
        if (nothingLoaded()) { maximum = Point3d::minPoint(); minimum = Point3d::maxPoint(); }

        for (size_t i = 0; i < equatorialOrbits.size(); i++) equatorialOrbits[i].calculateOrbit(cosfs, sinfs);

        if (nothingLoaded()) {
            for (size_t i = 0; i < equatorialOrbits.size(); ++i)
            {
                minimum = findMin(equatorialOrbits[i].posInPlane, minimum);
                maximum = findMax(equatorialOrbits[i].posInPlane, maximum);
            }
        }
        loading = false;
        updateGL();
    }

    /*! @brief Calls either Orbit::calculateOrbit() or Orbit::calculatePosition() on all the particles in orbitData

        This function is part of the loading process for simulation files.
        It readies the Orbit objects in orbitData for drawing.
        If the user selected to draw the full orbit in the OpenSimulationDialog, Orbit::calculateOrbit() will be
        called on all particles. Otherwise, Orbit::calculatePosition() will be called on all particles (this
        function is much faster).
        The scale is set by orbitData if nothing else is currently loaded.
        This function is called from Disp::OrbitalAnimationDriver::setSimulationData().
    */
    void OrbitalAnimator::updateSimulationCache(OrbitData const& d) {
        orbitData = d;
        if (nothingLoaded()) { maximum = Point3d::minPoint(); minimum = Point3d::maxPoint(); }
        simulationSize = 0;
        for (OrbitData::iterator itr = orbitData.begin(); itr != orbitData.end(); itr++) {
            for (size_t i = 0; i < (itr->second).size(); i++) {
                if (!drawFullOrbit) (itr->second)[i].calculatePosition(cosfs, sinfs);
                else (itr->second)[i].calculateOrbit(cosfs, sinfs);
                if (nothingLoaded()) {
                    minimum = findMin((itr->second)[i].posInPlane, minimum);
                    maximum = findMax((itr->second)[i].posInPlane, maximum);
                }
            }
            if ((itr->second).size() > (size_t)simulationSize) simulationSize = (itr->second).size();
        }

        coordLength = std::max(ABS(maximum.x), std::max(ABS(maximum.y), std::max(ABS(maximum.z),
                               std::max(ABS(minimum.x), std::max(ABS(minimum.y), ABS(minimum.z))))));
        settingsDialog->setFrameRange(simulationSize);
        loading = false;
        updateGL();

    }

    /*! @brief Removes the equatorial orbits

        This function clears equatorialOrbits and resets the scale if equatorialOrbits is the only thing that
        has been loaded.
        Called when the "Remove Equatorial Orbits" option is selected from the menubar.
    */
    void OrbitalAnimator::clearEquatorialData() {
        equatorialOrbits.clear();
        equatorialDataLoaded = false;
        if (!eclipticDataLoaded && !simulationDataLoaded) {
            minimum = Point3d(0, 0, 0);
            maximum = Point3d(0, 0, 0);
            coordLength = 0;
        }
        updateGL();
    }

    /*! @brief Removes the ecliptic orbits

        This function clears eclipticOrbits and resets the scale if eclipticOrbits is the only thing that
        has been loaded.
        Called when the "Remove Ecliptic Orbits" option is selected from the menubar.
    */
    void OrbitalAnimator::clearEclipticData() {
        eclipticOrbits.clear();
        eclipticDataLoaded = false;
        if (!equatorialDataLoaded && !simulationDataLoaded) {
            minimum = Point3d(0, 0, 0);
            maximum = Point3d(0, 0, 0);
            coordLength = 0;
        }
        updateGL();
    }

    /*! @brief Removes the simulation data

        This function clears orbitData and resets the scale if orbitData is the only thing that
        has been loaded.
        Called when the "Remove Simulation Orbit" option is selected from the menubar.
    */
    void OrbitalAnimator::clearSimulationData() {
        orbitData.clear();
        simulationDataLoaded = false;
        if (!eclipticDataLoaded && !equatorialDataLoaded) {
            minimum = Point3d(0, 0, 0);
            maximum = Point3d(0, 0, 0);
            coordLength = 0;
        }
        updateGL();
    }

    /*! @brief Removes everythign

        This function clears everything that has been loaded and resets the scale.
        Called when the "Remove All Orbits" option is selected from the menubar.
    */
    void OrbitalAnimator::clearAllData() {
        orbitData.clear();
        eclipticOrbits.clear();
        equatorialOrbits.clear();
        simulationDataLoaded = false;
        equatorialDataLoaded = false;
        eclipticDataLoaded = false;
        minimum = Point3d(0, 0, 0);
        maximum = Point3d(0, 0, 0);
        coordLength = 0;
        updateGL();
    }

    void setNewRotation(double &rot, double dx) {
        double xtemp = rot + dx;
        if (xtemp > 180) rot = xtemp - 360;
        else if (xtemp < -180) rot = xtemp + 360;
        else rot = xtemp;
    }

    /*! @brief Rotates the simulation

        This function rotates the simulation by (x, y, z) over "time."
    */
    void OrbitalAnimator::rotate(double x, double y, double z, int time) {
        double t = (double)time;
        double dx = x / t;
        double dy = y / t;
        double dz = z / t;
        for (int i=0; i < time; i++) {
            setNewRotation(xrotation, dx);
            setNewRotation(yrotation, dy);
            setNewRotation(zrotation, dz);
            //settingsDialog->xRotationBox->setValue(xrotation);
            //settingsDialog->yRotationBox->setValue(yrotation);
            //settingsDialog->zRotationBox->setValue(zrotation);
            updateOrRecord();
        }
    }

    /*! @brief Zooms the simulation

        This function zooms the simulation to "amt" over "time."
    */
    void OrbitalAnimator::zoom(double amt, int time) {
        double dz = (amt * scaleFactor - scaleFactor) / time;
        for (int i=0; i < time; i++) {
            scaleFactor += dz;
            settingsDialog->zoomScaleBox->setValue(scaleFactor);
            //settingsDialog->scrollZoom->setValue(scaleFactor * 100);
            updateOrRecord();
        }
    }

    /*! @brief Plays the simulation

        This function increases the frame number of the simulation by "amt" over "time."
    */
    void OrbitalAnimator::simulate(int amt, int time) {
        int remainder = amt % time;
        int ds = (amt - remainder) / time;
        for (int i=0; i < time; i++) {
            currentIndex += ds;
            settingsDialog->scrollTimeIndex->setValue(currentIndex);
            settingsDialog->timeIndex->setValue(currentIndex);
            updateOrRecord();
        }
        currentIndex += remainder;
        if (remainder != 0) updateOrRecord();
    }

    /*! @brief Pauses the simulation

        This function pauses for "time" frames.
    */
    void OrbitalAnimator::doNothing(int time) { for (int i=0; i < time; i++) updateOrRecord(); }

    /*! @brief Sets the simulation state

        This function sets the simulation to the passed state.
        Rotations are set to (x,y,z), zoom is set to "sc", and frame number is set to "fr."
    */
    void OrbitalAnimator::initialize(double x, double y, double z, double sc, int fr) {
        xrotation = x;
        yrotation = y;
        zrotation = z;
        scaleFactor = sc;
        currentIndex = fr;
        //settingsDialog->xRotationBox->setValue(xrotation);
        //settingsDialog->yRotationBox->setValue(yrotation);
        //settingsDialog->zRotationBox->setValue(zrotation);
        settingsDialog->zoomScaleBox->setValue(scaleFactor);
        settingsDialog->scrollTimeIndex->setValue(currentIndex);
        settingsDialog->timeIndex->setValue(currentIndex);
        updateOrRecord();
    }

    /*! @brief Executes an action

        This function takes in an Action (defined in Queue.h) and executes it in the simulation.
    */
    void OrbitalAnimator::performAction(Action act) {
        switch (act.typ) {
        case ROTATE: rotate(act.dx, act.dy, act.dz, act.span); break;
        case ZOOM: zoom(act.newScale, act.span); break;
        case SIMULATE: simulate(act.dFrame, act.span); break;
        case PAUSE: doNothing(act.span); break;
        case INITIALIZE: initialize(act.x, act.y, act.z, act.scale, act.frame);
        }
    }

    /*! @brief Sets the simulation to the correct state, then performs the action

        This function sets the simulation to the state stored in the Action and then performs the action.
    */
    void OrbitalAnimator::checkAndPerformAction(QTableWidgetItem* item) {
        QVariant v = item->data(Qt::UserRole);
        Action act = v.value<Action>();
        double x = act.x, y = act.y, z = act.z, sc = act.scale, fr = act.frame;
        switch (act.typ) {
        case ROTATE:
            x -= act.dx;
            y -= act.dy;
            z -= act.dz;
            break;
        case ZOOM:
            sc = act.prevScale;
            break;
        case SIMULATE:
            fr -= act.dFrame;
            break;
        }
        if (act.typ != 4) initialize(x, y, z, sc, fr);
        performAction(act);
    }

    /*! @brief Iterates through queue and calls Disp::OrbitalAnimator::performAction() on each item.
    */
    void OrbitalAnimator::playbackQueue(QTableWidget* queue) {
        for (int i=0; i < queue->rowCount(); i++) {
            QVariant v = queue->item(i, 0)->data(Qt::UserRole);
            Action act = v.value<Action>();
            performAction(act);
        }
    }

    /*! @brief Does the same thing as playbackQueue, except it records as it does so.

        Launches a QFileDialog to ask the user where she wants to save the pictures.
    */
    void OrbitalAnimator::record(QTableWidget* queue) {

        // first make a dialog to get the folder wants to store images in
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose or create the folder to which you want images output"), qgetenv("HOME"), QFileDialog::ShowDirsOnly);
        if(dirName.compare(QString::QString(""), Qt::CaseSensitive) == 0) {   return;  } // if the directory name = "", then user hit cancel so we return
        tmpPNGFolder = QDir::QDir(dirName);

        recording = true;
        playbackQueue(queue);   // generate images

        recording = false;
        pictureNumber = 0;
    }
/*    /*! @brief stitches the movie out of individual frames using the ffmpeg library

    void OrbitalAnimator::makeMovie(QString moviePath)
    {
        int argc = 8;

        QString input = tmpPNGFolder.absolutePath() + "/orb%05d.png";

        char arg0[] = "ffmpeg";
        char arg1[] = "-i";
        char arg2[input.size()+1];
        memcpy(arg2, input.toStdString().c_str(), input.size()+1);
        char arg3[] = "-c:v";
        char arg4[] = "libx264";
        char arg5[] = "-pix_fmt";
        char arg6[] = "yuv420p";
        char arg7[moviePath.size()+1];
        memcpy(arg7, moviePath.toStdString().c_str(), moviePath.size()+1);

        char *argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0], &arg5[0], &arg6[0], &arg7[0]};
        int errorCode;

        errorCode = ffmpeg(argc, argv);

        if(errorCode)
        {
            qDebug("ffmpeg returned non-zero value: %i", errorCode);
            exit(errorCode);
        }
    }
*/
    /*! @brief Depending on whether the bool recording is true, either calls updateGL() or saveCurrentImage().
    */
    void OrbitalAnimator::updateOrRecord() {
        if (recording) {
            saveCurrentImage(pictureNumber);
            pictureNumber++;
        }
        else updateGL();
    }

    /*! @brief Zooms the simulation based on the mouse wheel movement.

        Reimplemented from QGLWidget.
    */
    void OrbitalAnimator::wheelEvent(QWheelEvent* we)
    {
        int numDegrees = we->delta() / 8;
        int numSteps = numDegrees / 15;

        if (numSteps < 0) scaleFactor *= pow(.8, -numSteps);
        else scaleFactor *= pow(1.25, numSteps);

        settingsDialog->zoomScaleBox->setValue(scaleFactor);
        updateGL();
    }

    /*! @brief Rotates the simulation based on mouse dragging movement.

        Reimplemented from QGLWidget.
        If the mouse is dragged using the left mouse button, the rotations occur around the Y and Z axes.
        If the mouse is dragged using the right mouse button, the rotations occur around the Y and X axes.
    */
    void OrbitalAnimator::mouseMoveEvent(QMouseEvent* event)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();
        int speed = 1;

        if (event->buttons() & Qt::LeftButton) {
            setNewRotation(yrotation, speed * dy);
            setNewRotation(zrotation, speed * dx);
            //settingsDialog->yRotationBox->setValue(yrotation);
            //settingsDialog->zRotationBox->setValue(zrotation);
            updateGL();
        }
        else if (event->buttons() & Qt::RightButton) {
            setNewRotation(yrotation, speed * dy);
            setNewRotation(xrotation, speed * dx);
            //settingsDialog->yRotationBox->setValue(yrotation);
            //settingsDialog->xRotationBox->setValue(xrotation);
            updateGL();
        }

        lastPos = event->pos();
    }

    /*! @brief Populates the cosfs and sinfs arrays.

        Called from Disp::OrbitalAnimator::OrbitalAnimator().
    */
    void OrbitalAnimator::prepfs() {
        for (int f = 0; f < 360; ++f) {
            cosfs[f] = cos(static_cast<double>(f / 180. * M_PI));
            sinfs[f] = sin(static_cast<double>(f / 180. * M_PI));
        }
    }

    template<>
    void OrbitalAnimator::setTextColor<OrbitalAnimator::Pixmap>(QColor c)
    {
        if (!currentPainter) throw std::runtime_error("No painter set");
        currentPainter->setPen(c);
    }

    template<>
    void OrbitalAnimator::setTextColor<OrbitalAnimator::OpenGL>(QColor c)
    {
        glColor4f(c.red() / 255., c.green() / 255., c.blue() / 255., c.alpha() / 255.);
    }

    template<>
    void OrbitalAnimator::drawText<OrbitalAnimator::Pixmap>(QString str, int topLeftX, int topLeftY, QFontMetrics* fm)
    {
        if (!currentPainter) throw std::runtime_error("No painter set");
        currentPainter->drawText(QPointF(topLeftX, fm->height() + topLeftY), str);
    }

    template<>
    void OrbitalAnimator::drawText<OrbitalAnimator::OpenGL>(QString str, int topLeftX, int topLeftY, QFontMetrics* fm)
    {
        renderText(topLeftX, fm->height() + topLeftY, str);
    }

    /*! @brief Writes "Loading..." on the screen while a simulation is being loaded.

        Called by OrbitalAnimator::paintGL()*/
    template<OrbitalAnimator::Display disp>
    void OrbitalAnimator::drawLoading()
    {
        setTextColor<disp>(QColor(0, 255, 0, 255));
        QFontMetrics fm(font());
        QString text;
        text = QString("Loading...");
        int textWidth = fm.width(text);
        drawText<disp>(text, width() - textWidth - 10, height() - 50, &fm);
    }

    /*! @brief Writes the rotation, zoom and frame values on the display.

        Called by OrbitalAnimator::paintGL()*/

    template<OrbitalAnimator::Display disp>
    void OrbitalAnimator::drawStats()
    {
        setTextColor<disp>(QColor(255, 255, 255, 255));
        QString x = QString("X Rot (deg) : %1").arg(xrotation); // substitutes xrotation for %1
        QString y = QString("Y Rot (deg) : %1").arg(yrotation);
        QString z = QString("Z Rot (deg) : %1").arg(zrotation);
        QString zoom = QString("Zoom : %1%").arg(scaleFactor * 100);
        QString frame = QString("Frame Number : %1").arg(simulationDataLoaded ? currentIndex : 0);
        QFont f;
        f.setPointSize(16);
        renderText(10, 25, x, f); // renderText writes the 3rd parameter passed, at the pixel locations passed in the first two parameters.
        renderText(10, 45, y, f);
        renderText(10, 65, z, f);
        renderText(10, 85, zoom, f);
        renderText(10, 105, frame, f);
    }

    /*! @brief SLOT executed when xrotation value is changed by the user in the SettingsDialog, which governs the display's orientation (around the x axis).*/
    void OrbitalAnimator::setXRot(double deg) { xrotation = std::max(-180.0, std::min(180.0, deg)); updateGL(); }
    /*! @brief SLOT executed when yrotation value is changed by the user in the SettingsDialog, which governs the display's orientation (around the y axis).*/
    void OrbitalAnimator::setYRot(double deg) { yrotation = std::max(-180.0, std::min(180.0, deg)); updateGL(); }
    /*! @brief SLOT executed when zrotation value is changed by the user in the SettingsDialog, which governs the display's orientation (around the z axis).*/
    void OrbitalAnimator::setZRot(double deg) { zrotation = std::max(-180.0, std::min(180.0, deg)); updateGL(); }

    /*! @brief SLOT executed when zoom is changed in the SettingsDialog.

        Connected in OrbitalAnimationDriver::makeConnections().

      This version should be used if zoom is changed to a slider.*/
    void OrbitalAnimator::setZoomFactor(int zoom)
    {
        double f = (double)zoom;
        f = std::pow(10, zoom / 100.0);
        f = std::max(0.001, std::min(100000.0, f));
        scaleFactor = f;
        updateGL();
    }

    /*! @brief SLOT executed when the index is changed.

        Connected in OrbitalAnimationDriver::makeConnections().*/
    void OrbitalAnimator::setCurrentIndex(int index)
    {
        currentIndex = std::max(std::min(index, simulationSize - 1), 0);
        updateGL();
    }

    /*! @brief Moves the simulation frame forward one, and resets the frame to the beginning if it reaches the end. */
    void OrbitalAnimator::advanceTimeIndex()
    {
        ++currentIndex;
        if (currentIndex > simulationSize) currentIndex = 0;
        updateGL();
    }

    /*void OrbitalAnimator::printImages(QString directory, int timeStep) DEPRECATED
    {
        QProgressDialog progress("Saving Images...", "Cancel", 0, simulationSize / timeStep, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();

        int prevIndex = currentIndex;
        QString fileNameTemplate(directory.isEmpty() ? "orbitalFrame_%1.png" : directory + "/orbitalFrame_%1.png");
        int opNum = 1;
        for (currentIndex = 0; currentIndex < simulationSize; currentIndex += timeStep, ++opNum)
        {
            QGLFramebufferObject buffer(width(), height(), QGLFramebufferObject::CombinedDepthStencil);
            buffer.bind();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            paintGL();
            buffer.release();

            QPixmap p(QPixmap::fromImage(buffer.toImage()));

            QPainter painter(&p);
            painter.setFont(font());
            currentPainter = &painter;
            currentPainter = 0;

            p.save(fileNameTemplate.arg(currentIndex));

            progress.setValue(opNum);
            if (progress.wasCanceled())
                break;
        }
        currentIndex = prevIndex;
    }*/

    /*! @brief saves a snapshot of the OrbitalAnimator to a file with the passed id at the end, in the directory passed.

        Has to set the filename written out to have the same number of digits for every frame, e.g., 1 -> 00001, so that ffmpeg can read it.
        I set the number of digits to 5.  The limit of 10^5 frames corresponds at ~30 fps to an hour-long video, which presumably is enough!
        Called by OrbitalAnimator::updateOrRecord()
    */

    void OrbitalAnimator::saveCurrentImage(int id)
    {
        QString fileNameTemplate("orb%1.png");
        if (tmpPNGFolder.exists())
            fileNameTemplate = tmpPNGFolder.absolutePath() + QString("/") + fileNameTemplate;
        else {
            qWarning("Bug:  Folder to put still images in was not created properly");
            exit(1);
        }

        // need to make the filename the same number of digits in every frame so that ffmpeg can read them
        QString fname = fileNameTemplate.arg(pictureNumber, 5, 'f', 0, '0'); // these arguments mean that there should be 5 digits, the format should be 'f'
                                                                            // (no scientific notation), that there should be 0 digits after the decimal
                                                                            // and that the filler characters should be zeroes (to put in front of the ID if
                                                                            // it's less than 5 digits)
        {
            QGLFramebufferObject buffer(width(), height(), QGLFramebufferObject::CombinedDepthStencil);
            buffer.bind();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            paintGL();
            buffer.release();

            /* the x264 codec requires that the images passed to it have even dimensions (e.g., 546x468).  So we first make a temporary pixmap to store the image,
               and then use its copy function to generate our final image with the values in ptemp, bounded by the rectangle defined by the 4 passed values.
               The first two are the (x,y) coordinates to start from--(0,0) is the top left corner--and then you provide the width and height of the image.
               We pass the value from ptemp - the width (or height) modulo 2.  If the number was even, modulo 2 = 0 and it's unchanged.  If odd, modulo 2 = 1,
               and we pass it one less than the original value.  So if both dimensions were odd, it would cut off the right-most column and bottom row of the image.
            */

            QPixmap ptemp(QPixmap::fromImage(buffer.toImage()));
            QPixmap p = ptemp.copy(0,0,ptemp.width()-ptemp.width()%2, ptemp.height()-ptemp.height()%2);

            QPainter painter(&p);
            painter.setFont(font());
            currentPainter = &painter;
            currentPainter = 0;

            p.save(fname);
        }
    }

    /* Used by commented out code in settingsDialog for doing relative rotations (e.g., rotate by 30 degrees from current state)*/
    void OrbitalAnimator::rotate() {
        rotate(settingsDialog->rotateAmountX->value(),
               settingsDialog->rotateAmountY->value(),
               settingsDialog->rotateAmountZ->value(),
               settingsDialog->rotateSpeed->value());
    }

    void OrbitalAnimator::zoom() {
        zoom(settingsDialog->zoomAmount->value(), settingsDialog->zoomSpeed->value());
    }

    void OrbitalAnimator::simulate() {
        simulate(settingsDialog->simulateAmount->value(), settingsDialog->simulateSpeed->value());
    }

}

