/*!
 @file OrbitalAnimator.h
 @brief class definition for OrbitalAnimator, which does the OpenGL drawing.

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

#ifndef ORBITAL_ANIMATOR_H
#define ORBITAL_ANIMATOR_H

#include "Helpers/Point3d.h"
#include "Helpers/GLDrawingFunctions.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "QueueActionDialog.h"
#include "OrbitalAnimationDriver.h"
#include "Helpers/Orbit.h"
#include <QtOpenGL/QGLWidget>
#include <QFileDialog>
#include <QtGui/QPainter>
#include <QtGui/QProgressDialog>
#include <QtGui/QWheelEvent>
#include <QFontMetrics>
#include <QtOpenGL/QGLFramebufferObject>
#include <QtCore/QDebug>
#include <QMessageBox>
#include <sstream>
#include <algorithm>
#include <limits>
#include <stdexcept>

extern "C" {
// #include "../ffmpeg/ffmpeg.h"
}

#ifdef WIN32
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif

#ifdef WIN32
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif
#endif

#define ABS(x) ((x)<0?-(x):(x))

/*! @brief The code's main namespace
*/
namespace Disp
{
    /*! @brief class that does the OpenGL drawing
    */
    class OrbitalAnimator : public QGLWidget
    {
        Q_OBJECT
    public:
        /*! @brief Constructor
        */
        OrbitalAnimator(OrbitalAnimatorSettings& settings_, QWidget *parent = 0);
        /*! @brief Initializes the OrbitalAnimator instance
        */
        QWidget* setupUI(OrbitalAnimatorSettings& animatorSettings);
        /*! @brief Does something with Equatorial orbits

            More details on that something this function does

            @param eqo vector of type StaticDisplayOrbit holding all the static equatorial orbits
            @see setEclipticData()
            @see setSimulationData()
        */

        int heightForWidth( int width ) const;
        QSize sizeHint() const;
        void setFullOrbit(bool b) { drawFullOrbit = b; }
        void setdrawParticles(bool b) { drawParticles = b; }
        void setFillOrbits(bool b) { fillOrbits = b; }
        void setdrawOrbitNormals(bool b) { drawOrbitNormals = b; }
        void setZoom(double zoomPercent) { scaleFactor = zoomPercent; updateGL(); }
        void updateOrRecord();
        void saveCurrentImage(int id);
        bool simulationDataLoaded;
        bool equatorialDataLoaded;
        bool eclipticDataLoaded;
        bool nothingLoaded() { return !simulationDataLoaded && !equatorialDataLoaded && !eclipticDataLoaded; }
        SettingsDialog* settingsDialog;
        OrbitalAnimatorSettings& settings;
        QPushButton* savePicsButton;
        QWidget* controlsWidget;
        void clearSimulationData();
        void clearEquatorialData();
        void clearEclipticData();
        void clearAllData();
        void setLoading(bool b) { loading = b; }
        //void performIntermediateAction(QTableWidgetItem* a);
        void playbackQueue(QTableWidget* queue);
        void record(QTableWidget* queue);
        double getXRotation() { return xrotation; }
        double getYRotation() { return yrotation; }
        double getZRotation() { return zrotation; }
        double getZoomScale() { return scaleFactor; }
        int getCurrentFrame() { return currentIndex; }
        int getSimulationSize() { return simulationSize; }
        void updateEclipticCache(StaticDisplayOrbits const& eco);
        void updateEquatorialCache(StaticDisplayOrbits const& eqo);
        void updateSimulationCache(OrbitData const& d);

    public slots:
        void setCurrentIndex(int index);
        void setXRot(double deg);
        void setYRot(double deg);
        void setZRot(double deg);
        void setZoomFactor(double zoom);
        void rotate();
        void zoom();
        void simulate();
        void advanceTimeIndex();

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void wheelEvent(QWheelEvent* w);
        void mouseMoveEvent(QMouseEvent* m);

    private:
        enum Display { Pixmap, OpenGL };
        void performAction(Action a);
        void rotate(double x, double y, double z, int t);
        void zoom(double a, double t);
        void simulate(int a, int t);
        void doNothing(int t);
        void initialize(double x, double y, double z, double sc, int fr);
        void prepfs();
        void drawTrail();
        void drawParticle();
        void drawOrbit();
        void drawOrbitalNormal();
        template<Display> void drawStats();
        template<Display> void drawLoading();
        template<Display> void drawTime();
        //void makeMovie(QString moviePath);

        // These functoins are meant to allow generalization of overlay painting
        // via whether they are painted to a pixmap, or directly to opengl
        template<Display> void setTextColor(QColor c);
        template<Display> void drawText(QString str, int topLeftX, int topLeftY, QFontMetrics* fm);

        OrbitData orbitData;
        std::vector<Point3d> normals;
        double normalsScalar;
        double cosfs[360];
        double sinfs[360];
        int currentIndex;
        int simulationSize;
        StaticDisplayOrbits equatorialOrbits;
        StaticDisplayOrbits eclipticOrbits;
        double scaleFactor;
        Point3d minimum, maximum; //smallest and largest x, y, z, respectively
        double xrotation, yrotation, zrotation;
        QPoint lastPos;
        Sphere centralBody;
        QPainter* currentPainter;
        GLfloat coordLength;
        Point3d zEq;
        Point3d xEq;
        double obl;
        RotationAngles eqRotAngles;
        bool loading;
        bool recording;
        QDir tmpPNGFolder;
        int pictureNumber;
        int trailLength;
        bool drawFullOrbit;
        bool fillOrbits;
        bool drawParticles;
        bool drawOrbitNormals;

    };
} // namespace Disp

#endif
