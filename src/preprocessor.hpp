#ifndef WEBCLVALIDATOR_WEBCLPREPROCESSOR
#define WEBCLVALIDATOR_WEBCLPREPROCESSOR

#include "reporter.hpp"

#include "clang/Lex/PPCallbacks.h"

namespace clang {
    class CompilerInstance;
}

/// \brief Preprocessor callbacks for WebCL C code.
class WebCLPreprocessor : public WebCLReporter
                        , public clang::PPCallbacks
{
public:

    explicit WebCLPreprocessor(clang::CompilerInstance &instance);
    ~WebCLPreprocessor();

    /// \brief Complain about include directives in main source files.
    /// \see PPCallbacks::InclusionDirective
    virtual void InclusionDirective(
        clang::SourceLocation HashLoc, const clang::Token &IncludeTok, 
        llvm::StringRef FileName, bool IsAngled,
        clang::CharSourceRange FilenameRange, const clang::FileEntry *File,
        llvm::StringRef SearchPath, llvm::StringRef RelativePath,
        const clang::Module *Imported);

    /// \brief Complain about enabled OpenCL extensions.
    /// \see PPCallbacks::PragmaOpenCLExtension
    virtual void PragmaOpenCLExtension(
        clang::SourceLocation NameLoc, const clang::IdentifierInfo *Name,
        clang::SourceLocation StateLoc, unsigned State);

private:

    clang::CompilerInstance &instance_;
    /// OpenCL extensions that can be enabled.
    std::set<std::string> extensions_;
};

#endif // WEBCLVALIDATOR_WEBCLPREPROCESSOR
