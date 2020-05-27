#include "clang_completion.h"

#include <QStringList>

Clang_Completion::Clang_Completion(QString &fileContent, int ln, int col)
    : _file(fileContent), _line(ln), _col(col)
{

}

void Clang_Completion::complete()
{
        QStringList entry;
        CXUnsavedFile unsavedFiles[1];
         unsavedFiles[0].Filename = "dummy.cpp";
         unsavedFiles[0].Contents = _file.toStdString().c_str();
         unsavedFiles[0].Length = _file.toStdString().length();
        CXIndex index = clang_createIndex(0, 0);
        const char* arg = "-Wall -x c++ -x c++-header -fsyntax-only -Xclang";
        CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        "dummy.cpp", &arg, 0,
        unsavedFiles, 1,
                    CXTranslationUnit_DetailedPreprocessingRecord );


       CXCodeCompleteResults* res = clang_codeCompleteAt(unit, "dummy.cpp", _line, _col, unsavedFiles, 1, CXCodeComplete_IncludeMacros
                                                         |CXCodeComplete_IncludeCodePatterns |CXCodeComplete_IncludeBriefComments |CXCodeComplete_IncludeCompletionsWithFixIts);


       /* for (unsigned i = 0; i < clang_codeCompleteGetNumDiagnostics(res); i++) {
            const CXDiagnostic& diag = clang_codeCompleteGetDiagnostic(res, i);
            const CXString& s = clang_getDiagnosticSpelling(diag);
            qDebug() << clang_getCString(s);
        }*/ ///////////// EMIT when parse on change enabled

        for (unsigned i = 0; i < res->NumResults; i++) {
            const CXCompletionString& str = res->Results[i].CompletionString;

            for (unsigned j = 0; j < clang_getNumCompletionChunks(str); j++) {
                if (clang_getCompletionChunkKind(str, j) != CXCompletionChunk_TypedText)
                    continue;

                const CXString& out = clang_getCompletionChunkText(str, j);
                entry.append(clang_getCString(out));
            }

        }

        emit new_info(entry);
        clang_disposeCodeCompleteResults(res);

       clang_disposeTranslationUnit(unit);
       clang_disposeIndex(index);
}

void Clang_Completion::run()
{
    if(_line <= 1 && _col <= 1)
        return;
    complete();
}
