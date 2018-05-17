#' part-of-speech tagger
#'
#' \code{pos} returns part-of-speech (POS) tagged morpheme of the sentence.
#'
#' This is a basic function for MeCab part-of-speech tagger.
#'
#' @param sentence A character vector.
#' @param dict A location of user-specific MeCab dictionary. The default value is "".
#' @return A string vector of POS tagged morpheme will be returned in conjoined character
#'  vecter form. Element name of the list are original phrases
#'
#' @examples
#' \dontrun{
#' pos("")
#' }
#'
#' @export
pos <- function(sentence, dict = "") {
  if (typeof(sentence) != "character") stop("The function gets a character vector only.")

  result <- posRcpp(sentence, dict)

  return(result)
}
