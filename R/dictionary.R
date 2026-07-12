#' Inspect loaded MeCab dictionaries
#'
#' Returns metadata reported by MeCab for the active system and user
#' dictionaries. This is useful for diagnosing which dictionary a package or
#' R session is actually using.
#'
#' @param sys_dic Character scalar. System dictionary directory. When empty,
#'   the `mecabSysDic` option and then MeCab's default are used.
#' @param user_dic Character scalar. Optional compiled user dictionary.
#' @return A data frame with one row per loaded dictionary and columns
#'   `filename`, `charset`, `type`, `size`, `left_size`, `right_size`, and
#'   `version`.
#' @examples
#' \dontrun{
#' dictionary_info()
#' }
#' @export
dictionary_info <- function(sys_dic = "", user_dic = "") {
  if (!is.character(sys_dic) || length(sys_dic) != 1L || is.na(sys_dic)) {
    stop("sys_dic must be a non-missing character scalar", call. = FALSE)
  }
  if (!is.character(user_dic) || length(user_dic) != 1L || is.na(user_dic)) {
    stop("user_dic must be a non-missing character scalar", call. = FALSE)
  }

  if (!nzchar(sys_dic) && !is.null(getOption("mecabSysDic"))) {
    sys_dic <- getOption("mecabSysDic")
  }

  dictionaryInfoRcpp(sys_dic, user_dic)
}
