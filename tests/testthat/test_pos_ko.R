test_that("Test pos tagger works on ko", {
  skip_if(!l10n_info()[["UTF-8"]], "Non-UTF-8 locale")
  skip_if_not(Sys.getenv("MECAB_LANG", "ko") == "ko", "Skip testing on Korean")

  info <- dictionary_info()
  expect_true(any(info$type == "system"))

  result <- pos(
    enc2utf8("\uc548\ub155\ud558\uc138\uc694 \uc800\ub294 \ubc15\ucc2c\uc5fd \uc785\ub2c8\ub2e4."),
    format = "data.frame"
  )
  expect_gt(nrow(result), 0L)
  expect_equal(result[4, 4], enc2utf8("\uc800"))
})
