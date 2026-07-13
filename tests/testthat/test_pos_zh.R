test_that("Chinese profile uses the bundled mecab-jieba dictionary", {
  skip_if_not(Sys.getenv("MECAB_LANG") == "zh", "Skip testing on Chinese")
  skip_if(!l10n_info()[["UTF-8"]], "Non-UTF-8 locale")

  simplified <- enc2utf8("\u6b66\u6c49\u5e02\u89e3\u9664\u79bb\u6c49\u79bb\u9102\u901a\u9053\u7ba1\u63a7\u63aa\u65bd")
  traditional <- enc2utf8("\u8fd1\u671f\u81ea\u716e\u9632\u75ab\u5df2\u6210\u4e86\u6700\u65b0\u98f2\u98df\u89c0")

  simplified_result <- pos(simplified, format = "data.frame")
  traditional_result <- pos(traditional, format = "data.frame")

  expect_gt(nrow(simplified_result), 0L)
  expect_gt(nrow(traditional_result), 0L)
  expect_identical(paste0(simplified_result$token, collapse = ""), simplified)
  expect_identical(paste0(traditional_result$token, collapse = ""), traditional)
  expect_true(enc2utf8("\u6b66\u6c49\u5e02") %in% simplified_result$token)
  expect_true(all(nzchar(simplified_result$pos)))

  parallel_result <- posParallel(simplified, format = "data.frame")
  expect_identical(parallel_result$token, simplified_result$token)
  expect_identical(parallel_result$pos, simplified_result$pos)

  info <- dictionary_info()
  expect_true(any(info$type == "system"))
  expect_true(all(toupper(info$charset) == "UTF-8"))
})
