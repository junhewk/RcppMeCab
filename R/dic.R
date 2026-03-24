#' Download and install a MeCab dictionary
#'
#' Downloads and installs a MeCab system dictionary for the specified language.
#' Japanese dictionaries are compiled from source using the built-in
#' \code{mecab-dict-index}; Korean dictionaries are downloaded pre-compiled.
#' No system-level MeCab installation is required.
#'
#' Dictionaries are stored in the user data directory
#' (\code{tools::R_user_dir("RcppMeCab", "data")}).
#'
#' @param lang Character scalar. Language code: \code{"ja"} for Japanese
#'   (IPAdic) or \code{"ko"} for Korean (mecab-ko-dic).
#' @return Invisible path to the installed dictionary directory.
#'
#' @examples
#' \dontrun{
#' download_dic("ja")
#' download_dic("ko")
#' pos("東京は日本の首都です。", lang = "ja")
#' }
#'
#' @export
download_dic <- function(lang) {
  lang <- match.arg(lang, c("ja", "ko"))
  dic_dir <- file.path(tools::R_user_dir("RcppMeCab", "data"), lang)

  if (lang == "ja") {
    .download_dic_ja(dic_dir)
  } else {
    .download_dic_ko(dic_dir)
  }

  message("Dictionary installed: ", dic_dir)
  invisible(dic_dir)
}

#' List installed MeCab dictionaries
#'
#' Shows all available MeCab dictionaries, including the bundled dictionary
#' and any downloaded via \code{\link{download_dic}}.
#'
#' @return A data frame with columns \code{lang}, \code{name}, \code{path},
#'   and \code{active}.
#'
#' @examples
#' \dontrun{
#' list_dic()
#' }
#'
#' @export
list_dic <- function() {
  user_dir <- tools::R_user_dir("RcppMeCab", "data")
  bundled <- system.file("dic", package = "RcppMeCab")
  active <- getOption("mecabSysDic", "")

  rows <- list()

  # Bundled dictionary
  if (nzchar(bundled) && file.exists(file.path(bundled, "sys.dic"))) {
    rows <- c(rows, list(data.frame(
      lang = "bundled", name = "bundled",
      path = bundled, active = (active == bundled),
      stringsAsFactors = FALSE
    )))
  }

  # User-installed dictionaries
  for (lang in c("ja", "ko")) {
    dic_path <- file.path(user_dir, lang)
    if (file.exists(file.path(dic_path, "sys.dic"))) {
      name <- if (lang == "ja") "ipadic" else "mecab-ko-dic"
      rows <- c(rows, list(data.frame(
        lang = lang, name = name,
        path = dic_path, active = (active == dic_path),
        stringsAsFactors = FALSE
      )))
    }
  }

  if (length(rows) == 0) {
    message("No dictionaries installed. Use download_dic() to install one.")
    return(invisible(data.frame(
      lang = character(), name = character(),
      path = character(), active = logical(),
      stringsAsFactors = FALSE
    )))
  }

  do.call(rbind, rows)
}

#' Set the active MeCab dictionary by language
#'
#' Sets the default system dictionary used by \code{\link{pos}} and
#' \code{\link{posParallel}}. This is equivalent to calling
#' \code{options(mecabSysDic = path)} but allows selection by language code.
#'
#' @param lang Character scalar. Language code (\code{"ja"} or \code{"ko"})
#'   or \code{"bundled"} to use the dictionary bundled with the package.
#' @return Invisible path to the activated dictionary directory.
#'
#' @examples
#' \dontrun{
#' set_dic("ja")
#' pos("東京は日本の首都です。")
#'
#' set_dic("ko")
#' pos("안녕하세요")
#' }
#'
#' @export
set_dic <- function(lang) {
  lang <- match.arg(lang, c("ja", "ko", "bundled"))
  dic_path <- .resolve_dic(lang)
  options(mecabSysDic = dic_path)
  invisible(dic_path)
}

# ---- internal helpers --------------------------------------------------------

#' Resolve a language code to a dictionary path
#' @noRd
.resolve_dic <- function(lang) {
  if (lang == "bundled") {
    dic_path <- system.file("dic", package = "RcppMeCab")
    if (!nzchar(dic_path) || !file.exists(file.path(dic_path, "sys.dic")))
      stop("No bundled dictionary found")
    return(dic_path)
  }

  dic_path <- file.path(tools::R_user_dir("RcppMeCab", "data"), lang)
  if (!file.exists(file.path(dic_path, "sys.dic")))
    stop("Dictionary for '", lang, "' not installed. Run download_dic(\"", lang, "\")")
  dic_path
}

#' Download and compile Japanese IPAdic
#' @noRd
.download_dic_ja <- function(dic_dir) {
  src_url <- "https://github.com/taku910/mecab/archive/61b90ba6e669dc2d7d533d4a80d206f3b31d52b1.tar.gz"
  tmp_dir <- tempfile("mecab_ja_")
  dir.create(tmp_dir, recursive = TRUE)
  on.exit(unlink(tmp_dir, recursive = TRUE), add = TRUE)

  tarball <- file.path(tmp_dir, "mecab.tar.gz")
  message("Downloading IPAdic source...")
  utils::download.file(src_url, tarball, mode = "wb", quiet = TRUE)

  message("Extracting...")
  utils::untar(tarball, exdir = tmp_dir)
  # taku910/mecab archive has mecab-<hash>/mecab-ipadic/ structure
  extracted <- list.dirs(tmp_dir, recursive = FALSE)
  ipadic_dir <- file.path(extracted[1], "mecab-ipadic")
  if (!dir.exists(ipadic_dir))
    stop("Could not find mecab-ipadic directory in archive")

  dir.create(dic_dir, recursive = TRUE, showWarnings = FALSE)

  message("Compiling dictionary (this may take a moment)...")
  args <- c("mecab-dict-index",
            "-d", normalizePath(ipadic_dir, mustWork = TRUE),
            "-o", normalizePath(dic_dir, mustWork = TRUE),
            "-f", "euc-jp",
            "-t", "utf-8")
  result <- dictIndexRcpp(args)
  if (result != 0)
    stop("Dictionary compilation failed (return code: ", result, ")")

  file.copy(file.path(ipadic_dir, "dicrc"), dic_dir, overwrite = TRUE)
  message("Japanese (IPAdic) dictionary installed.")
}

#' Download pre-compiled Korean mecab-ko-dic
#' @noRd
.download_dic_ko <- function(dic_dir) {
  dic_url <- "https://github.com/Pusnow/mecab-ko-msvc/releases/download/release-0.999/mecab-ko-dic.tar.gz"
  tmp_dir <- tempfile("mecab_ko_")
  dir.create(tmp_dir, recursive = TRUE)
  on.exit(unlink(tmp_dir, recursive = TRUE), add = TRUE)

  tarball <- file.path(tmp_dir, "mecab-ko-dic.tar.gz")
  message("Downloading mecab-ko-dic...")
  utils::download.file(dic_url, tarball, mode = "wb", quiet = TRUE)

  dir.create(dic_dir, recursive = TRUE, showWarnings = FALSE)

  message("Extracting...")
  utils::untar(tarball, exdir = dic_dir, extras = "--strip-components=1")
  if (!file.exists(file.path(dic_dir, "sys.dic")))
    stop("Dictionary extraction failed: sys.dic not found")

  message("Korean (mecab-ko-dic) dictionary installed.")
}
