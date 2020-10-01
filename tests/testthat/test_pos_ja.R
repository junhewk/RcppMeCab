if (Sys.getenv("MECAB_LANG") == "ja") {
  test_that("Test pos tagger works", {
    expect_equal(
      pos(
        enc2utf8("\u982d\u304c\u8d64\u3044\u9b5a\u3092\u98df\u3079\u305f\u732b"),
        format = "data.frame"
      )[4, 3],
      enc2utf8("\u8d64\u3044")
    )
  })
} else {
  test_that("Skipped the test on Japanese", {
    expect_true(TRUE)
  })
}

