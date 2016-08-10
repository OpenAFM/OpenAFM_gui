#ifndef DEF_COMMANDS_H
#define DEF_COMMANDS_H

#include <QByteArray>

namespace response{

const QByteArray GO = "GO;";
const QByteArray READY = "RDY;";
const QByteArray DONE = "DONE;";
const QByteArray SETUP = "SETUP;";
const QByteArray STREAM = "STREAM;";
const QByteArray F_BOUNDARY = ";";

const QString sGO = "GO";
const QString sREADY = "RDY";
const QString sDONE = "DONE";
const QString sSETUP = "SETUP";
const QString sSTREAM = "STREAM";
const QString sF_BOUNDARY = ";";



}


#endif // DEF_COMMANDS_H
