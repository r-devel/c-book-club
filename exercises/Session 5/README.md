# Exercise based on R's C interface, ch 20 of Advanced R 1st edition (http://adv-r.had.co.nz/C-interface.html)

Use `pryr::show_c_source()` to find the C source for an R function that uses `.Internal()`. 

Use `inline::cfunction()` to write equivalent code that uses `.Call()` instead. 

Convert the C routine to pure R and write some R code to test that your R version is equivalent.