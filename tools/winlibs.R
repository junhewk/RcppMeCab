# Download and patch MeCab source for Windows build
MECAB_SRC_URL <- "https://github.com/taku910/mecab/archive/61b90ba6e669dc2d7d533d4a80d206f3b31d52b1.tar.gz"
BUILD_DIR <- "mecab_build"
SRC_DIR <- file.path(BUILD_DIR, "mecab", "src")

if (!dir.exists(BUILD_DIR)) {
  dir.create(BUILD_DIR, recursive = TRUE)

  # Download
  tarball <- file.path(BUILD_DIR, "mecab-source.tar.gz")
  download.file(MECAB_SRC_URL, tarball, mode = "wb", method = "libcurl")
  untar(tarball, exdir = BUILD_DIR, extras = "--strip-components=1")

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
    "#define PACKAGE \"mecab\"",
    "#define VERSION \"0.996\"",
    "#define DIC_VERSION 102"
  ), file.path(SRC_DIR, "config.h"))

  # Patch source files
  src_files <- list.files(SRC_DIR, pattern = "\\.(cpp|h)$", full.names = TRUE)
  src_files <- src_files[!grepl("mecab_r_compat\\.h|config\\.h", src_files)]

  for (f in src_files) {
    lines <- readLines(f, warn = FALSE)
    lines <- c('#include "mecab_r_compat.h"', lines)
    lines <- gsub("std::cerr", "mecab_null_os_", lines)
    lines <- gsub("std::cout", "mecab_null_os_", lines)
    writeLines(lines, f)
  }

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
