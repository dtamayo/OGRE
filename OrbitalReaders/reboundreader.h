#ifndef REBOUNDREADER_H
#define REBOUNDREADER_H

#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtDebug>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "OrbitalDisplays/Point3d.h"
#include "OrbitalDisplays/Orbit.h"

class ReboundReader
{
public:
    ReboundReader(QString filename);
    OrbitData const& getData() const { return data; }

private:
    void readResults(QTextStream& stream);

    QRegExp lineParser;
    OrbitData data;
};

#endif // REBOUNDREADER_H
