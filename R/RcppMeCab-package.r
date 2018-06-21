#' RcppMeCab: Rcpp Wrapper for MeCab Library
#'
#' R package based on \code{Rcpp} for \code{MeCab}: Yet Another Part-of-Speech and
#' Morphological Analyzer (\url{http://taku910.github.io/mecab/}). The purpose of
#' this package is providing a seamless developing and analyzing environment for
#' CJK texts. This package utilizes parallel programming for providing
#' highly efficient text preprocessing \code{posParallel()} function.
#' For installation, please refer to README.md file.
#'
#' @name RcppMeCab
#' @docType package
#' @author Junhewk Kim Taku Kudo
#' @import Rcpp
#' @importFrom RcppParallel RcppParallelLibs
#' @useDynLib RcppMeCab
#'
#' @details
#' This package utilizes \code{MeCab} C API and \code{Rcpp} codes.
#'
#' @references
#' \itemize{
#' \item{\href{http://taku910.github.io/mecab/}{MeCab}}
#' \item{\href{http://www.rcpp.org}{Rcpp: Seamless R and C++ Integration}}
#' \item{\href{http://eunjeon.blogspot.com}{Eunjeon project}}
#' }
#'
#' @keywords MeCab part-of-speech morpheme nlp Korean Japanese Chinese
NULL

.onUnload <- function (libpath) {
  library.dynam.unload("RcppMeCab", libpath)
}
