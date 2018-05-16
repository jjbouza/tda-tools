library('devtools')
library('Rcpp')

setwd('~/tda/tdatools')
Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
compileAttributes()
build()
install()
