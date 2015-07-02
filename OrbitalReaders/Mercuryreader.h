#ifndef MERCURYREADER_H
#define MERCURYREADER_H

#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "Helpers/Point3d.h"
#include "Helpers/Orbit.h"

class MercuryReader
{
public:
    MercuryReader(QString filename, QString dataType);
    OrbitData const& getData() const { return data; }

private:
    void readOsc(QTextStream& stream, int ID);
    void readXYZ(QTextStream& stream, int ID);

    QRegExp lineParser;
    OrbitData data;
};

#endif // MERCURYREADER_H

