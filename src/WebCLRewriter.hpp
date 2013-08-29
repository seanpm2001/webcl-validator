#ifndef WEBCLVALIDATOR_WEBCLREWRITER
#define WEBCLVALIDATOR_WEBCLREWRITER

#include "clang/Basic/SourceLocation.h"

#include <map>
#include <set>
#include <utility>

namespace clang {
    class CompilerInstance;
    class Rewriter;
}

/// Improves rewriter so that we can do a little more complex modifications
/// and still query modified rewritten text.
///
/// Doesn't apply replacements directly, but collects them and leaves original
/// SourceLocations untouched. Current implementation works in cases where
/// all the replacements are done from deepest first and wider after that etc.
/// overlapping replacements are not supported at all.
///
/// e.g. consider following are 5 replacements a,b,c,d and e
///
/// <start>----a1---b1--c1-c2--------b2---a2-----d1---e1---e2---d2---<eof>
///
/// To make them correclty replacements must be applied in order
/// c,b,a,e,d and afterwads there cannot be any more replacements inside area
/// a1-a2 or d1-d2, however those exact ranges can be still rewritten. 
class WebCLRewriter {
public:
  
  WebCLRewriter(clang::CompilerInstance &instance, clang::Rewriter &rewriter);

  /// \brief Looks char by char forward until the position where next
  /// requested character is found from original source.
  clang::SourceLocation findLocForNext(clang::SourceLocation startLoc,
                                       char charToFind);
  
  /// \brief Removes given range.
  void removeText(clang::SourceRange range);

  /// \brief Inserts text at given location.
  void insertText(clang::SourceLocation location, const std::string &text);

  /// \brief Replaces given range.
  void replaceText(clang::SourceRange range, const std::string &text);

  /// \return Original unmodified text from the given range.
  std::string getOriginalText(clang::SourceRange range);

  /// \brief if for asked source range has been added transformations, return
  ///
  /// Returns ransformed result like rewriters getRewrittenText. Combines on the
  /// fly original data from source and modified ranges from requested range.
  ///
  /// NOTE: If requested range is *inside* any replaced range this might
  ///       return wrong results.
  ///
  std::string getTransformedText(clang::SourceRange range);

  /// Applies transformations.
  void applyTransformations();

private:

  clang::CompilerInstance &instance_;
  clang::Rewriter &rewriter_;
  
  typedef std::pair< int, int >                  ModifiedRange;
  typedef std::map< ModifiedRange, std::string > RangeModifications;

  /// \brief Map of modified source ranges and corresponding replacements.
  typedef std::pair< clang::SourceLocation, clang::SourceLocation > WclSourceRange;
  typedef std::map<WclSourceRange, std::string> ModificationMap;
  // filtered ranges, which has only the top level modifications and does not
  // include nested ones (top level should already contain nested changes as string)
  typedef std::set<ModifiedRange> RangeModificationsFilter;

  /// \brief Get modified source ranges and replacements.
  ///
  /// Returns only toplevel modifications to be easily delegated to rewriter.
  /// each call might regenerate underlying map, so get map once for getting
  /// e.g. begin and end iterators.
  ModificationMap& modifiedRanges();

  /// \brief Returns toplevel modifications to allow applying them to source.
  /// Updates list filteredModifiedRanges_ if there has been new replacements
  /// after the last call.
  RangeModificationsFilter& filteredModifiedRanges();
  
  /// \brief Map of all replacements.
  RangeModifications modifiedRanges_;

  /// \brief Set of toplevel replacements which are not nested inside any other replacement.
  RangeModificationsFilter filteredModifiedRanges_;

  /// \brief State if we should regenerate the filtered ranges data
  bool isFilteredRangesDirty_;

  /// \brief Used to deliver modification list data outside of this class.
  ModificationMap externalMap_;
};

#endif // WEBCLVALIDATOR_WEBCLREWRITER
