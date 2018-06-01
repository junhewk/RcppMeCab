#' part-of-speech tagger
#'
#' \code{pos} returns part-of-speech (POS) tagged morpheme of the sentence.
#'
#' This is a basic function for MeCab part-of-speech tagger. The function gets a
#' character vector of any length, and runs a loop inside C++ to provide faster
#' processing.
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
#' pos(sentence)
#' pos(sentence, join = FALSE)
#' }
#'
#' @export
pos <- function(sentence, join = TRUE, dict = "") {
  if (typeof(sentence) != "character") stop("The function gets a character vector only.")

  if (join == TRUE) {
    if (length(sentence) > 1) {
      result <- posLoopJoinRcpp(sentence, dict)
    } else {
      result <- posJoinRcpp(sentence, dict)
    }
  } else {
    if (length(sentence) > 1) {
      result <- posLoopRcpp(sentence, dict)
    } else {
      result <- posRcpp(sentence, dict)
    }
  }


  return(result)
}
