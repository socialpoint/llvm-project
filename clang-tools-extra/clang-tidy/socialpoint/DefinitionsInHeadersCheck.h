//===--- DefinitionsInHeadersCheck.h - clang-tidy----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SOCIALPOINT_DEFINITIONS_IN_HEADERS_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SOCIALPOINT_DEFINITIONS_IN_HEADERS_H

#include "../ClangTidyCheck.h"
#include "../utils/FileExtensionsUtils.h"

namespace clang {
namespace tidy {
namespace socialpoint {

/// Finds non-extern non-inline function and variable definitions in header
/// files, which can lead to potential ODR violations. THIS IS CUSTOM MODIFICATION
/// OF http://clang.llvm.org/extra/clang-tidy/checks/misc-definitions-in-headers.html
/// This modification allows to fix internal linkage definitions on header files
///
/// The check supports these options:
///   - `UseHeaderFileExtension`: Whether to use file extension to distinguish
///     header files. True by default.
///   - `HeaderFileExtensions`: a semicolon-separated list of filename
///     extensions of header files (The filename extension should not contain
///     "." prefix). ";h;hh;hpp;hxx" by default.
///
///     For extension-less header files, using an empty string or leaving an
///     empty string between ";" if there are other filename extensions.
///   - `IncludeInternalLinkage`: Internal linkage variable definitions are taken 
///     into account when IncludeInternalLinkage is set to true. By default is false
///     Examples at file scope:
///        const int a = 1;
///        static int b = 1;
///
// For the original check documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/misc-definitions-in-headers.html
class DefinitionsInHeadersCheck : public ClangTidyCheck {
public:
  DefinitionsInHeadersCheck(StringRef Name, ClangTidyContext *Context);
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const bool IncludeInternalLinkage;
  const bool UseHeaderFileExtension;
  const std::string RawStringHeaderFileExtensions;
  utils::FileExtensionsSet HeaderFileExtensions;

  template <typename T>
  void Fix(const T* decl, SourceManager *SourceManager, const LangOptions& LangOpts);
};

} // namespace socialpoint
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_SOCIALPOINT_DEFINITIONS_IN_HEADERS_H
