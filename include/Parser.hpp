#include <QCommandLineParser>

inline QCommandLineParser parser;
inline QCommandLineOption categorizeAll("categorize-all", "Will recategorize all previously recorded transactions");
#ifndef NDEBUG
inline QCommandLineOption startFresh("start-fresh", "WARNING: Will remove all previously recorded transactions and start fresh");
#endif