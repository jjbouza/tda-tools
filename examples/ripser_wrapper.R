# Alexander Wagner, April 2018
  
ripser_wrapper <- function(rip.input, rip.format, rip.dim, rip.threshold = -1){
  # rip.input = point cloud or below diagonal distance matrix
  # rip.format = "point-cloud" or "lower-distance"
  # rip.dim = max degree of homology computed
  # rip.threshold = maximum diameter of Rips filtration, -1 for all diameters
  
  # Convert input to character array.
  if (rip.format == "point-cloud"){
    rip.input <- do.call(paste, c(as.data.frame(rip.input)))
  }
  if (rip.format == "lower-distance"){
    rip.input <- paste(t(rip.input)[upper.tri(rip.input)], collapse = " ")
  }
  
  # Create system command based on format, homology degree, and threshold.
  rip.args <- paste("--format", rip.format)
  rip.args <- paste(rip.args, paste("--dim", rip.dim))
  if (rip.threshold != -1){
    rip.args <- paste(rip.args, paste("--threshold", rip.threshold))
  }

  # Use Ripser.
  barcode <- system2(command = "./bin/ripser", 
                     args = rip.args, 
                     stdout = TRUE,
                     input = rip.input)
  # Convert text output to list of matrices giving barcodes
  new.degrees.at <- which(grepl(pattern = "persistence", x = barcode))
  ph <- vector("list", length(new.degrees.at))
  for (i in 1:length(new.degrees.at)){
    if (i == length(new.degrees.at)){
      num.pairs <- length(barcode) - new.degrees.at[i]
    }
    else{
      num.pairs <- new.degrees.at[i + 1] - new.degrees.at[i] - 1
    }
    # Leave i-th persistence diagram empty if there are no bars.
    if (num.pairs == 0){
      break
    }
    bd.pairs <- matrix(0, nrow = num.pairs, ncol = 2)
    for (j in 1:num.pairs){
      pair.string <- gsub(")", "", 
                          gsub("[", "", 
                               gsub(" ", "",
                                    barcode[j + new.degrees.at[i]], 
                                    fixed=T), 
                               fixed=T), 
                          fixed=T)
      pair <- scan(text = pair.string, sep = ",", quiet = TRUE)
      if (is.na(pair[2])){
        pair[2] <- -1
      }
      bd.pairs[j, ] <- pair
    }
    ph[[i]] <- bd.pairs
  }
  return(ph)
}
