#ifndef DEF_COMMANDS_H
#define DEF_COMMANDS_H

#include <QByteArray>

namespace response{

const QByteArray GO = "GO;";
const QByteArray NOPIEZO = "PST;";

const QByteArray READY = "RDY;";
const QByteArray DONE = "DONE;";
const QByteArray SETUP = "SETUP;";
const QByteArray STREAM = "STREAM;";
const QByteArray F_BOUNDARY = ";";
const QByteArray FES = "SIG::FESH?;";
const QByteArray V="V";

}


#endif // DEF_COMMANDS_H
