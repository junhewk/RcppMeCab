#' @noRd
## usethis namespace: start
#' @importFrom Rcpp sourceCpp
#' @useDynLib RcppMeCab, .registration=TRUE
## usethis namespace: end
NULL

#' @noRd
#' @param libpath libpath
.onUnload <- function(libpath) {
  library.dynam.unload("RcppMeCab", libpath)
}
