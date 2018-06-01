#' parallel version of part-of-speech tagger
#'
#' \code{posParallel} returns part-of-speech (POS) tagged morpheme of the sentence.
#'
#' This is a parallelized version of MeCab part-of-speech tagger. The function gets a
#' character vector of any length, and runs a loop inside C++ with Intel TBB to provide faster
#' processing.
#'
#' Parallelizing over a character vector is not supported by \code{RcppParallel}.
#' Thus, this function makes duplicates of the input and the output.
#' Therefore, if your data volume is large, use \code{pos} or divide the vector to
#' several sub-vectors.
#'
#' If you want to get a morpheme only, use join = False to put tag names on the attribute.
#' Basically the function will return a list of character vectors with (morpheme)/(tag) elements.
#'
#' @param sentence A character vector of any length. For analyzing multiple sentences, put them in one character vector.
#' @param dict A location of user-specific MeCab dictionary. The default value is "".
#' @param join A bool to decide the output format. The default value is TRUE. If FALSE, the function will return morphemes only, and tags put in the attribute.
#' @return A string vector of POS tagged morpheme will be returned in conjoined character
#'  vecter form. Element name of the list are original phrases
#'
#' @examples
#' \dontrun{
#' sentence <- c(#some UTF-8 texts)
#' posParallel(sentence)
#' posParallel(sentence, join = FALSE)
#' }
#'
#' @importFrom RcppParallel RcppParallelLibs
#' @export
posParallel <- function(sentence, join = TRUE, dict = "") {
  if (typeof(sentence) != "character") stop("The function gets a character vector only.")

  if (join == TRUE) {
    result <- posParallelJoinRcpp(sentence, dict)
  } else {
    result <- posParallelRcpp(sentence, dict)
  }

  return(result)
}
