#' Compile a MeCab user dictionary
#'
#' \code{dict_index} compiles a user dictionary CSV file into a binary
#' dictionary that can be used with \code{pos} and \code{posParallel}.
#'
#' This function wraps MeCab's \code{mecab-dict-index} internally, so you
#' do not need the command-line tool installed separately.
#'
#' @param dic_csv Character scalar. Path to the user dictionary CSV file(s).
#'   Multiple CSV files can be provided as a character vector.
#' @param out_dic Character scalar. Path for the output compiled dictionary file.
#' @param dic_dir Character scalar. Path to the system dictionary directory.
#'   This is required so that MeCab can reference the system dictionary
#'   configuration during compilation.
#' @param dic_charset Character scalar. Charset of the input CSV file.
#'   Default is \code{"utf-8"}.
#' @param out_charset Character scalar. Charset of the output dictionary.
#'   Default is \code{"utf-8"}.
#' @return Invisible \code{TRUE} on success.
#'
#' @examples
#' \dontrun{
#' dict_index(
#'   dic_csv = "user_words.csv",
#'   out_dic = "user.dic",
#'   dic_dir = "/usr/local/lib/mecab/dic/ipadic"
#' )
#'
#' # Then use the compiled dictionary:
#' pos("some text", user_dic = "user.dic")
#' }
#'
#' @export
dict_index <- function(dic_csv, out_dic, dic_dir,
                       dic_charset = "utf-8", out_charset = "utf-8") {
  if (!is.character(dic_csv) || length(dic_csv) < 1)
    stop("dic_csv must be a character vector of CSV file path(s)")
  if (!is.character(out_dic) || length(out_dic) != 1)
    stop("out_dic must be a single output file path")
  if (!is.character(dic_dir) || length(dic_dir) != 1)
    stop("dic_dir must be a single directory path")

  for (f in dic_csv) {
    if (!file.exists(f))
      stop("CSV file not found: ", f)
  }
  if (!dir.exists(dic_dir))
    stop("System dictionary directory not found: ", dic_dir)

  out_dir <- dirname(out_dic)
  if (!dir.exists(out_dir))
    stop("Output directory does not exist: ", out_dir)

  args <- c("mecab-dict-index",
            "-d", normalizePath(dic_dir, mustWork = TRUE),
            "-u", normalizePath(out_dic, mustWork = FALSE),
            "-f", dic_charset,
            "-t", out_charset,
            normalizePath(dic_csv, mustWork = TRUE))

  dictIndexRcpp(args)

  invisible(TRUE)
}
