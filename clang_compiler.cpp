#include "clang_compiler.h"

Clang_Compiler::Clang_Compiler(const QString& ws)
{
    _ws = ws;
}

void Clang_Compiler::set(const QString &ws)
{
    _ws = ws;
}

void Clang_Compiler::collect_paths(const QString &ws)
{
    QDirIterator it(ws, QStringList() << "*.h" << "*.cpp" << "*.c" << "*.hpp", QDir::NoFilter,  QDirIterator::Subdirectories);
    _files.clear();
    while(it.hasNext())
    {
        _files.push_back(it.next());
    }
    emit debug_add(QString::number(_files.size()) +" elements found in your workspace!");
}

void Clang_Compiler::run()
{
        setPriority(HighestPriority);
    mutex.lock();
    QString msg = "Build procedure started.";
    emit debug_add(msg);
    collect_paths(_ws);

    for(auto f: _files)
    {
        compile(f);
    }
    msg = "Compilation procedure finished!";
     emit debug_add(msg);
     mutex.unlock();
}
#include <QDebug>
void Clang_Compiler::compile(const QString& file)
{
    CXIndex Index = clang_createIndex(0, 0);
    CXTranslationUnit TU = clang_parseTranslationUnit(
        Index,
        file.toStdString().c_str(), nullptr, 0,
        nullptr, 0,
        CXTranslationUnit_None);

     for (unsigned I = 0, N = clang_getNumDiagnostics(TU); I != N; ++I)
     {
     CXDiagnostic Diag = clang_getDiagnostic(TU, I);

     CXString String = clang_formatDiagnostic(Diag,
     clang_defaultDiagnosticDisplayOptions());
     QString error = clang_getCString(String);
   /*  ;*/
     emit debug_add(error);
     clang_disposeString(String);
     }

     clang_disposeTranslationUnit(TU);
     clang_disposeIndex(Index);
}


