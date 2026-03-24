# Download and patch MeCab source for Windows build
# Selects taku910/mecab for Japanese or Pusnow/mecab-ko-msvc for Korean
# based on the MECAB_LANG environment variable (default: "ko").

MECAB_LANG <- Sys.getenv("MECAB_LANG", "ko")

if (MECAB_LANG == "ja") {
  MECAB_SRC_URL <- "https://github.com/taku910/mecab/archive/61b90ba6e669dc2d7d533d4a80d206f3b31d52b1.tar.gz"
  MECAB_VERSION <- "0.996"
} else {
  MECAB_SRC_URL <- "https://github.com/Pusnow/mecab-ko-msvc/archive/refs/tags/release-0.999.tar.gz"
  MECAB_VERSION <- "0.999"
}

BUILD_DIR <- "mecab_build"
SRC_DIR <- file.path(BUILD_DIR, "mecab", "src")

if (!dir.exists(BUILD_DIR)) {
  dir.create(BUILD_DIR, recursive = TRUE)

  # Download
  tarball <- file.path(BUILD_DIR, "mecab-source.tar.gz")
  download.file(MECAB_SRC_URL, tarball, mode = "wb", method = "libcurl")
  untar(tarball, exdir = BUILD_DIR, extras = "--strip-components=1")

  # For mecab-ko-msvc, source is in src/ directly (not mecab/src/)
  if (MECAB_LANG != "ja" && !dir.exists(SRC_DIR)) {
    dir.create(file.path(BUILD_DIR, "mecab"), recursive = TRUE)
    file.rename(file.path(BUILD_DIR, "src"), SRC_DIR)
  }

  cat("Using MeCab source:", MECAB_LANG, MECAB_VERSION, "\n")

  # Write R-compat header
  writeLines(c(
    "#ifndef MECAB_R_COMPAT_H_",
    "#define MECAB_R_COMPAT_H_",
    "#include <iostream>",
    "#include <cstdio>",
    "class MeCabNullBuf : public std::streambuf {",
    "protected:",
    "  int overflow(int c) { return c; }",
    "};",
    "static MeCabNullBuf mecab_null_buf_;",
    "static std::ostream mecab_null_os_(&mecab_null_buf_);",
    "#endif"
  ), file.path(SRC_DIR, "mecab_r_compat.h"))

  # Write config.h
  writeLines(c(
    "#define HAVE_GETENV 1",
    "#define HAVE_STRING_H 1",
    "#define HAVE_STDLIB_H 1",
    "#define HAVE_STDINT_H 1",
    "#define HAVE_WINDOWS_H 1",
    paste0("#define PACKAGE \"mecab\""),
    paste0("#define VERSION \"", MECAB_VERSION, "\""),
    "#define DIC_VERSION 102"
  ), file.path(SRC_DIR, "config.h"))

  # Patch source files: prepend R-compat header, redirect cerr/cout
  src_files <- list.files(SRC_DIR, pattern = "\\.(cpp|h)$", full.names = TRUE)
  src_files <- src_files[!grepl("mecab_r_compat\\.h|config\\.h", src_files)]

  for (f in src_files) {
    lines <- readLines(f, warn = FALSE)
    lines <- c('#include "mecab_r_compat.h"', lines)
    lines <- gsub("std::cerr", "mecab_null_os_", lines)
    lines <- gsub("std::cout", "mecab_null_os_", lines)
    writeLines(lines, f)
  }

  # mecab.h: disable dllimport for static linking
  f <- file.path(SRC_DIR, "mecab.h")
  lines <- readLines(f, warn = FALSE)
  lines <- gsub("__declspec\\(dllimport\\)", "extern", lines)
  writeLines(lines, f)

  # thread.h: fix _stdcall -> __stdcall for MinGW
  f <- file.path(SRC_DIR, "thread.h")
  lines <- readLines(f, warn = FALSE)
  lines <- gsub("_stdcall", "__stdcall", lines, fixed = TRUE)
  writeLines(lines, f)

  # string_buffer.h: add size_t overload for 64-bit MinGW (size_t = unsigned long long)
  f <- file.path(SRC_DIR, "string_buffer.h")
  lines <- readLines(f, warn = FALSE)
  lines <- gsub("(operator<<\\(unsigned long int n\\)\\s*\\{[^}]*\\})",
                "\\1\n  StringBuffer& operator<<(unsigned long long int n) { _UITOA(n); }",
                lines)
  writeLines(lines, f)

  # common.h: remove exit(-1) from die class
  f <- file.path(SRC_DIR, "common.h")
  lines <- readLines(f, warn = FALSE)
  lines <- lines[!grepl("^\\s*exit\\(-1\\);", lines)]
  writeLines(lines, f)

  # utils.h: sprintf -> snprintf
  f <- file.path(SRC_DIR, "utils.h")
  lines <- readLines(f, warn = FALSE)
  lines <- gsub('std::sprintf\\(s, "%-16f", val\\)',
                'std::snprintf(s, sizeof(s), "%-16f", val)', lines)
  writeLines(lines, f)

  # utils.cpp: gut progress_bar body
  f <- file.path(SRC_DIR, "utils.cpp")
  lines <- readLines(f, warn = FALSE)
  in_func <- FALSE
  brace_depth <- 0
  new_lines <- character()
  for (line in lines) {
    if (!in_func && grepl("^int progress_bar", line)) {
      in_func <- TRUE
      brace_depth <- brace_depth + lengths(regmatches(line, gregexpr("\\{", line)))
      brace_depth <- brace_depth - lengths(regmatches(line, gregexpr("\\}", line)))
      new_lines <- c(new_lines, line)
      next
    }
    if (in_func) {
      brace_depth <- brace_depth + lengths(regmatches(line, gregexpr("\\{", line)))
      brace_depth <- brace_depth - lengths(regmatches(line, gregexpr("\\}", line)))
      if (brace_depth <= 0 && grepl("\\}", line)) {
        new_lines <- c(new_lines, "  return 1;", "}")
        in_func <- FALSE
      }
      next
    }
    new_lines <- c(new_lines, line)
  }
  writeLines(new_lines, f)

  # eval.cpp: sprintf -> snprintf
  f <- file.path(SRC_DIR, "eval.cpp")
  lines <- readLines(f, warn = FALSE)
  lines <- gsub("sprintf\\(buf\\.get\\(\\),", "snprintf(buf.get(), 256,", lines)
  writeLines(lines, f)

  cat("MeCab source patched successfully.\n")
} else {
  cat("MeCab source already prepared.\n")
}
