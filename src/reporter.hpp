#ifndef WEBCLVALIDATOR_WEBCLREPORTER
#define WEBCLVALIDATOR_WEBCLREPORTER

#include "clang/Basic/Diagnostic.h"

namespace clang {
    class CompilerInstance;
}

/// \brief Mixin class for reporting errors.
class WebCLReporter
{
public:

    explicit WebCLReporter(clang::CompilerInstance &instance);
    ~WebCLReporter();

    clang::DiagnosticBuilder warning(
        clang::SourceLocation location, const char *format);
    clang::DiagnosticBuilder error(
        clang::SourceLocation location, const char *format);
    clang::DiagnosticBuilder fatal(const char *format);

    bool isFromMainFile(clang::SourceLocation location);

protected:

    clang::CompilerInstance &instance_;

private:

    clang::DiagnosticBuilder message(
        clang::DiagnosticsEngine::Level level, const char *format,
        clang::SourceLocation *location = 0);

};

#endif // WEBCLVALIDATOR_WEBCLREPORTER
