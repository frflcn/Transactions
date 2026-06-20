#include <QCommandLineParser>

inline QCommandLineParser parser;
#ifndef NDEBUG
inline QCommandLineOption startFresh("start-fresh", "WARNING: Will remove all previously recorded transactions and start fresh");
#endif