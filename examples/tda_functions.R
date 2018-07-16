# tda_functions.R
# Peter Bubenik, March 2018


eucl.dist <- function(x1, x2) sqrt(sum((x1 - x2) ^ 2))

colMax <- function(data) apply(vertices,2, max)
colMin <- function(data) apply(vertices,2, min)

draw_filtered_triangulation <- function(triangles,vertices,triangle_values){
  num_triangles <- dim(triangles)[1]
  M <- matrix(0,nrow=3*num_triangles, ncol=3) # columns: x,z,z coords; rows: three vertices of each triangle
  for (i in 1:num_triangles) {
    M[3*(i-1)+1,] <- vertices[triangles[i,1],] 
    M[3*(i-1)+2,] <- vertices[triangles[i,2],] 
    M[3*(i-1)+3,] <- vertices[triangles[i,3],] 
  }
  cols <- myColorRamp(c("red", "blue"), triangle_values) 
  cols3 <- character(3*num_triangles)
  for (i in 1:num_triangles) {
    cols3[3*(i-1)+1] <- cols[i]
    cols3[3*(i-1)+2] <- cols[i]
    cols3[3*(i-1)+3] <- cols[i]
  }
  open3d()
  triangles3d(M, col = cols3)
}

draw_filtered_graph <- function(edges,vertices,edge_values){
  num_edges <- dim(edges)[1]
  M <- matrix(0,nrow=2*num_edges, ncol=3) # columns: x,y,z coords; rows: two vertices of each edge
  for (i in 1:num_edges) {
    M[2*(i-1)+1,] <- vertices[edges[i,1],1:3] 
    M[2*(i-1)+2,] <- vertices[edges[i,2],1:3] 
  }
  cols <- myColorRamp(c("red", "blue"), edge_values) 
  cols2 <- character(2*num_edges)
  for (i in 1:num_edges) {
    cols2[2*(i-1)+1] <- cols[i]
    cols2[2*(i-1)+2] <- cols[i]
  }
  open3d()
  segments3d(M, col = cols2, lwd = 1)
#  for (i in 1:10){
#    tube <- cylinder3d(M[(2*(i-1)+1):(2*(i-1)+2),])
#    shade3d(addNormals(subdivision3d(tube, depth = 2)), col = "green")
#  }
}

animate_filtered_triangulation <- function(triangles,vertices,triangle_values,num_frames,file,animation_delay){
  num_triangles <- dim(triangles)[1]
  ordered_by_val <- order(triangle_values)
  M <- matrix(0,nrow=3*num_triangles, ncol=3) # columns: x,y,z coords; rows: three vertices of each triangle
  for (i in 1:num_triangles) {
    M[3*(i-1)+1,] <- vertices[triangles[ordered_by_val[i],1],] 
    M[3*(i-1)+2,] <- vertices[triangles[ordered_by_val[i],2],] 
    M[3*(i-1)+3,] <- vertices[triangles[ordered_by_val[i],3],] 
  }
  cols <- myColorRamp(c("red", "blue"), triangle_values[ordered_by_val]) 
  cols3 <- character(3*num_triangles)
  for (i in 1:num_triangles) {
    cols3[3*(i-1)+1] <- cols[i]
    cols3[3*(i-1)+2] <- cols[i]
    cols3[3*(i-1)+3] <- cols[i]
  }
  open3d()
  par3d(windowRect = c(0,0,500,500))
  observer3d(0,0,4)
  userMat1 <- matrix( data = c(1, 0, 0, 0, 
                                0, 0.3420201, 0.9396926, 0,
                                0, -0.9396926, 0.3420201, 0,
                               0, 0, 0, 1), nrow=4, ncol=4, byrow = TRUE)
  userMat2 <- matrix( data = c(0.62316626, -0.7604749, 0.1825973,    0,
                               0.03232119,  0.2583160, 0.9655197,    0,
                               -0.78142124, -0.5957775, 0.1855532,    0,
                               0, 0, 0, 1), 
                      nrow=4, ncol=4, byrow = TRUE)
  pts <- rbind(colMin(vertices),colMax(vertices)) # use to set constant frame
  points3d(pts,size=0.000001,col="white")
  for (i in 1:num_frames){
    num_triangles_to_draw <- round(num_triangles*i/num_frames)
    triangles3d(M[1:(3*num_triangles_to_draw),], col = cols3[1:(3*num_triangles_to_draw)]) # partial triangulation
    par3d(userMatrix=userMat1*(num_frames-i)/num_frames + userMat2*i/num_frames) # rotate viewpoint
    rgl.snapshot(filename=paste(file,"_frame_",sprintf("%03d", i), ".png", sep="")) # write to file
  }
  system(paste("convert -delay ",animation_delay," -loop 0 ",file,"_frame*.png ",file,".gif",sep=""))
  # view animated gif with browser
#  system(paste("open -a /Applications/Google\ Chrome.app/ ",file,".gif",sep=""))
}

put_on_positive_integer_grid <- function(M, grid_size)
  1+round((M-min(M))/(max(M)-min(M))*(grid_size))

pers_hom_filtered_triangulation <- function(triangles,triangle_values,grid_size,file,operating_system){
  triangle_integer_values <- put_on_positive_integer_grid(triangle_values,grid_size) # perseus wants integers other than -1 and 0
  triangles_and_values <- cbind(triangles,triangle_integer_values)
  output_filename <- paste(file,".prs",sep="")
  output_file <- file(output_filename,'w')
  writeLines(c("2","1"), output_file)
  write(t(triangles_and_values),file=output_file,ncolumns=4,append=TRUE)
  close(output_file)
  if (operating_system == 'Mac')
    perseus_file <- file.path("..",binary_directory,"perseusMac")
  else if (operating_system == 'PC')
    perseus_file <- file.path("..",binary_directory,"perseusWin")
  else if (operating_system == 'Linux')
    perseus_file <- file.path("..",binary_directory,"perseusLin")
  system(paste(perseus_file,"simtop",output_filename,file,"r",sep=" ")) # use top-down reduction engine instead of problematice bottom-up coreduction engine
  ph_0 <- as.matrix(read.table(paste(file,"_0.txt",sep="")))
  ph_1 <- as.matrix(read.table(paste(file,"_1.txt",sep="")))
  ph <- list(ph_0,ph_1)
}

pers_hom_filtered_graph <- function(edges,edge_values,grid_size,file,operating_system){
  edge_integer_values <- put_on_positive_integer_grid(edge_values,grid_size) # perseus wants integers other than -1 and 0
  edges_and_values <- cbind(edges,edge_integer_values)
  output_filename <- paste(file,".prs",sep="")
  output_file <- file(output_filename,'w')
  writeLines(c("1","1"), output_file)
  write(t(edges_and_values),file=output_file,ncolumns=3,append=TRUE)
  close(output_file)
  if (operating_system == 'Mac')
    perseus_file <- file.path("..",binary_directory,"perseusMac")
  else if (operating_system == 'PC')
    perseus_file <- file.path("..",binary_directory,"perseusWin")
  else if (operating_system == 'Linux')
    perseus_file <- file.path("..",binary_directory,"perseusLin")
  system(paste(perseus_file,"simtop",output_filename,file,"r",sep=" ")) # use top-down reduction engine instead of problematice bottom-up coreduction engine
  ph_0 <- as.matrix(read.table(paste(file,"_0.txt",sep="")))
  if (file.size(paste(file,"_1.txt",sep="")) == 0){
    ph_1 <- matrix(,nrow=0,ncol=2)
  } else{
    ph_1 <- as.matrix(read.table(paste(file,"_1.txt",sep="")))
  }
  ph <- list(ph_0,ph_1)
}

plot_persistence_diagram <- function(bd,title="",xlim=NULL,ylim=NULL){
  bd_finite <- matrix(bd[bd[,2]!=-1,],ncol=2)
  bd_infinite <- matrix(bd[bd[,2]==-1,],ncol=2) 
  plot(bd_finite,xlab="Birth",ylab="Death",pch=20,col="blue",asp=1,main=title,xlim=xlim,ylim=ylim)
  bd_infinite[,2] <- par("usr")[4]
  points(bd_infinite,pch=15,col="red")
  abline(0,1)
}

persistence_landscape_old <- function(bd_list,file,max_filtration_value,depth,degrees){
  for (i in degrees){
    bd <- bd_list[[i+1]]
    if (dim(bd)[1] > 0)
      bd[bd[,2]==-1,2] <- max_filtration_value #threshold infinite intervals
    filename <- paste(file,"_",i,sep="")
    output_file <- file(filename,'w')
    write(t(bd),file=output_file,ncolumns=2,append=FALSE)
    close(output_file)
    print(paste("../bin/PlotOfLandscape",filename,"0",depth-1,sep=" "))
    system(paste("../bin/PlotOfLandscape",filename,"0",depth-1,sep=" "))
  }
}

death_vector <- function(bd,max_filtration_value){
  if (dim(bd)[1] > 0)
    bd[bd[,2]==-1,2] <- max_filtration_value #threshold infinite intervals
  dv <- sort(bd[,2],decreasing=TRUE)
  return(dv)
}

plot_persistence_landscape_from_files <- function(file,degree,max_depth){
  depth <- 0
  if (file.exists(paste(file,"_",degree,"_",depth,".dat",sep=""))){
    PL <- as.matrix(read.table(paste(file,"_",degree,"_",depth,".dat",sep="")))
    min_value <- min(PL[,1])
    max_value <- max(PL[,1])
    max_height <- max(PL[,2])
    plot(0, type="n", main=paste("PL of ",file," in degree ",degree), xlab="(Birth+Death)/2", 
         ylab="(Death-Birth)/2", xlim=c(min_value, max_value), ylim=c(0, max_height))
    abline(0,0)
    depth <- 0
    for (i in 0:max_depth)
      if (file.exists(paste(file,"_",degree,"_",i,".dat",sep="")))
        depth <- i
    cols <- myColorRamp(c("blue", "yellow"), 1:(depth+1)) 
    for (i in 0:depth){
      PL <- as.matrix(read.table(paste(file,"_",degree,"_",i,".dat",sep="")))
      lines(PL,col=cols[i+1])
    }
  }
}

persistence_landscape_vector_int <- function(file,degree,max_depth){
  depth <- 0
  for (i in 0:max_depth)
    if (file.exists(paste(file,"_",degree,"_",i,".dat",sep="")))
      depth <- i
    pl <- vector()
    for (i in 0:depth){
      PL <- as.matrix(read.table(paste(file,"_",degree,"_",i,".dat",sep="")))
      PLint <- round(PL*2)
      storage.mode(PLint) <- "integer"
      PLy <- rep(0,grid_size*2+1)
      for (j in 1:(dim(PLint)[1]-1))
        for (k in PLint[j,1]:PLint[j+1,1])
          PLy[k] <- PLint[j,2] + (k-PLint[j,1])*(PLint[j+1,2]-PLint[j,2])/(PLint[j+1,1]-PLint[j,1])
      pl <- c(pl,PLy)
    }
    pl
}

persistence_landscape_value <- function(PL,param_val){
  y_val <- 0
  #Collection of pnts.
  for(i in 1:(dim(PL)[1]-1))
    if( (param_val >= PL[i,1]) & (param_val < PL[i+1,1]))
      y_val <- PL[i,2] + (param_val-PL[i,1]) * (PL[i+1,2]-PL[i,2]) / (PL[i+1,1]-PL[i,1])
    y_val
}

persistence_landscape_vector <- function(file,pl_param_vals,max_depth){
  depth <- -1
  for (i in 0:max_depth)
    if (file.exists(paste(file,"_",i,".dat",sep="")))
      depth <- i
  pl <- vector()
  if (depth >= 0){
    for (i in 0:depth){
      PL <- as.matrix(read.table(paste(file,"_",i,".dat",sep="")))
      file.remove(paste(file,"_",i,".dat",sep=""))
      PLy <- rep(0,length(pl_param_vals))
      for (j in 1:length(pl_param_vals))
        PLy[j] <- persistence_landscape_value(PL,pl_param_vals[j])
      pl <- c(pl,PLy)
    }
    file.remove(file)
    file.remove(paste(file,"_gnuplotCommand.txt",sep=""))
  }
  pl
}

plot_persistence_landscape_from_vector <- function(vec,ncol,main,fixed_max_y = 0){
  if (fixed_max_y == 0)
    plot(0, type="n", xlim=c(0, ncol-1), ylim=c(min(vec),max(vec)), main=main,ylab="Persistence")
  else
    plot(0, type="n", xlim=c(0, ncol-1), ylim=c(min(vec),fixed_max_y), main=main,ylab="Persistence")
  abline(0,0)
  PL <- matrix(vec,,ncol=ncol,byrow=TRUE)
  depth <- dim(PL)[1]
  cols <- myColorRamp(c("blue", "yellow"), 1:depth) 
  for (i in 1:depth)
    lines(cbind(0:(ncol-1),PL[i,]),col=cols[i])
}

matrix_from_list_of_vectors <- function(list_of_vectors){
  max_length <- 0
  num <- length(list_of_vectors)
  for (i in 1:num)
    max_length <- max(max_length, length(list_of_vectors[[i]]))
  for (i in 1:num)
    list_of_vectors[[i]] <- c(list_of_vectors[[i]],rep(0,max_length-length(list_of_vectors[[i]])))
  M <- matrix(0, nrow = num, ncol = max_length)
  for (i in 1:num)
    M[i,] <- list_of_vectors[[i]]
  M
}

combine_directions <- function(pl_vector_list,grid_size){
  num_vectors <- length(pl_vector_list)
  max_length <- 0
  for (i in 1:num_vectors)
    if (length(pl_vector_list[[i]]) > max_length)
      max_length <- length(pl_vector_list[[i]])
  pldv <- rep(0,num_vectors*max_length)
  for (i in 1:num_vectors){
    depth <- length(pl_vector_list[[i]])/(grid_size+1)
    if (depth > 0){
      for (j in 1:depth)
        for (k in 1:(grid_size+1))
          pldv[(j-1)*(num_vectors*(grid_size+1))+(i-1)*(grid_size+1)+k] <- pl_vector_list[[i]][(j-1)*(grid_size+1)+k]
    }
  }
  pldv  
}

permutation_test <- function(M1,M2,N){
  t_obs <- eucl.dist(colMeans(M1),colMeans(M2))
  k <- dim(M1)[1]
  M <- rbind(M1,M2)
  n <- dim(M)[1]
  count <- 0
  for (i in 1:N){
    perm <- sample(1:n)
    t <- eucl.dist(colMeans(M[perm[1:k],]),colMeans(M[perm[(k+1):n],]))
    if (t>=t_obs)
      count <- count+1
  }
  return(count/N)
}

svm3d <- function(x,y,z,cl,figure_list,xlab,ylab,zlab,main,cost=1){ # x,y,z,cl vectors of same length, cl is 1 or -1 (class)
  require("e1071")
  t <- data.frame(x=x, y=y, z=z, cl=cl)
  svm_model <- svm(cl~x+y+z, t, type='C-classification', kernel='linear', scale=FALSE, cost=cost)
  w <- t(svm_model$coefs) %*% svm_model$SV
  detalization <- 100                                                                                                                                                                 
  grid <- expand.grid(seq(from=min(t$x),to=max(t$x),length.out=detalization),                                                                                                         
                      seq(from=min(t$y),to=max(t$y),length.out=detalization))                                                                                                         
  z <- (svm_model$rho- w[1,1]*grid[,1] - w[1,2]*grid[,2]) / w[1,3]
  open3d()
  plot3d(grid[,1],grid[,2],z,xlab=xlab,ylab=ylab,zlab=zlab,main=main)  # this will draw plane.
  points3d(t$x[which(t$cl==-1)], t$y[which(t$cl==-1)], t$z[which(t$cl==-1)], col='red')
  points3d(t$x[which(t$cl==1)], t$y[which(t$cl==1)], t$z[which(t$cl==1)], col='blue')
  text3d(t$x[which(t$cl==-1)], t$y[which(t$cl==-1)], t$z[which(t$cl==-1)],text=figure_list[which(t$cl==-1)],cex=0.7, col="red", adj=c(-0.2,-0.2))
  text3d(t$x[which(t$cl==1)], t$y[which(t$cl==1)], t$z[which(t$cl==1)],text=figure_list[which(t$cl==1)],cex=0.7, col="blue", adj=c(-0.2,-0.2))
}

pca_plot <- function(pl, main, labels, vector_length, plot_loadings = FALSE){
  require("scatterplot3d")
  # Principal Component Analysis
  pca <- prcomp(pl,center=TRUE,scale.=FALSE,tol=0.01)
  plot(pca,type="l",main=main)
  # print(summary(pca))
  # plot loading vectors
  loading_vectors <- t(pca$rotation)
  if (plot_loadings==TRUE)
    for (j in 1:3)
      plot_persistence_landscape_from_vector(loading_vectors[j,],vector_length,paste(main,"loading vector",j))
  # static 3d scatterplot
  s3d <- scatterplot3d(pca$x[,1],pca$x[,2],pca$x[,3],color="blue",pch=19,type="h",xlab="pca1",ylab="pca2",zlab="pca3",main=main)
  s3d.coords <- s3d$xyz.convert(pca$x[,1],pca$x[,2],pca$x[,3]) # get projected 2d coordinates
  text(s3d.coords$x, s3d.coords$y, labels=labels, cex=.5, pos=4)
  # dynamic 3d scatterplot
  open3d()
  plot3d(pca$x[,1],pca$x[,2],pca$x[,3], col="blue", size=5,xlab="pca1",ylab="pca2",zlab="pca3",main=main)
  text3d(pca$x[,1],pca$x[,2],pca$x[,3],text=labels,cex=0.7, col="red", adj=c(-0.2,-0.2))
  pca
}

pl_comparison <- function(plm_list, sample_classes, vector_length, classes_to_compare, N, cost, 
                          degrees, class_names, death_vector = FALSE, num_to_skip = c(0,0,0,0,0,0)){
  require("scatterplot3d")
  num_classes <- length(sample_classes)
  ii <- 1:num_classes # find figures corresponding to the two groups
  group1 <- ii[which(sample_classes[]==classes_to_compare[1])]
  group2 <- ii[which(sample_classes[]==classes_to_compare[2])]
  comparison_classes <- c(sample_classes[group1],sample_classes[group2])
#  degree <- 1 # degree of homology to use
  for (degree in degrees){ # for each degree of homology
    # plot Average Persistence Landscapes and their difference
    pl_matrix <- plm_list[[degree+1]]
    if (death_vector == TRUE && degree == 0){
      pl_matrix <- pl_matrix[,-(1:num_to_skip[1])]
    } else{
      pl_matrix <- pl_matrix[,-(1:(num_to_skip[degree+1]*length(pl_param_vals)))]
    }
    pl1 <- pl_matrix[group1,]
    pl2 <- pl_matrix[group2,]
    apl1 <- colMeans(pl1)
    apl2 <- colMeans(pl2)
    if (death_vector == TRUE && degree == 0){
      vl <- length(apl1)
      } else{
      vl <- vector_length
      }
    plot_persistence_landscape_from_vector(apl1,vl,paste("Average PL in degree",degree,"for",classes_to_compare[1]))
    plot_persistence_landscape_from_vector(apl2,vl,paste("Average PL in degree",degree,"for",classes_to_compare[2]))
    plot_persistence_landscape_from_vector(apl2-apl1,vl,paste(classes_to_compare[2],"-",classes_to_compare[1]," in degree",degree))
    # Permutation test for significance of L^2 norm of this difference
    p_value <- permutation_test(pl1,pl2,N)
    print(paste("Permutation test in degree",degree,":",p_value)) # deg 0: 0.0005, deg 1: 0.0004, deg 2: 0.0006
    # Principal Component Analysis
    pca <- pca_plot(pl=rbind(pl1,pl2),main=paste("PCA for PL in degree",degree),labels=comparison_classes,vl)
    # Classification using Support Vector Machine applied to first 3 PCA coordinates
    svm3d(pca$x[,1], pca$x[,2], pca$x[,3], c(rep(-1,length(group1)),rep(1,length(group2))), comparison_classes, 
          "pca1", "pca2", "pca3",paste("PCA and SVM for PL in degree",degree), cost)
  }
}

# Support Vector Classification with k-fold Cross Validation
classification_svm_short <- function(M,sample_classes,num_folds,C){
  require("kernlab") 
  svm_model <- ksvm(M,sample_classes,type="C-svc",scaled=c(),kernel="vanilladot",C=C,cross=num_folds)
  print(svm_model)
}

# Support Vector Classification with k-fold Cross Validation
classification_svm <- function(M,svm_classes,num_folds,cost){
  require("kernlab") 
  num_folds <- min(num_folds,length(svm_classes))
  # Perform k fold cross validation
  folds <- cut(seq(1,nrow(M)),breaks=num_folds,labels=FALSE) # Create k equally size folds
  folds <- folds[sample(length(folds))]  # Randomly shuffle the folds
  # SVM for PL matrix - use ksvm from kernlab (slow); svm from e1071 results in stack overflow 
  num_sv <- 0 # total number of support vectors
  pred_table <- matrix(0,nrow=nlevels(svm_classes),ncol=nlevels(svm_classes))
  for(i in 1:num_folds){
    # Segement your data by fold using the which() function 
    testIndices <- which(folds==i,arr.ind=TRUE)
    testSet <- M[testIndices,,drop=FALSE] # prevent 1 row matrix from turning into a vector
    trainSet <- M[-testIndices,,drop=FALSE]
    # Fit the model
    svm_model <- ksvm(trainSet,svm_classes[-testIndices],type="C-svc",scaled=c(),kernel="vanilladot",C=cost)
    num_sv <- num_sv + nSV(svm_model)
    # Make the prediction (the dependent variable, without using the Type)
    svm_pred <- predict(svm_model, testSet)
    pred_table <- pred_table + table(pred = svm_pred, true = svm_classes[testIndices])
  }
  print(pred_table)
  pred_accuracy <- sum(diag(pred_table)) / sum(pred_table)
  print(paste("Average number of Support Vectors:",num_sv/num_folds))
  print(paste("Predication Accuracy:",pred_accuracy)) 
}

# Support Vector Classification for Kernel with k-fold Cross Validation
classification_svm_gram_short <- function(Gram,svm_classes,num_folds,cost){
  require("kernlab")
  svm_model <- ksvm(Gram,svm_classes,type="C-svc",kernel="matrix",C=cost,cross=num_folds)
  print(svm_model)
}

# Support Vector Classification for Kernel with k-fold Cross Validation
classification_svm_gram <- function(Gram,svm_classes,num_folds,cost){
  require("kernlab")
  num_folds <- min(num_folds,length(svm_classes))
  # Perform k fold cross validation
  folds <- cut(seq(1,nrow(Gram)),breaks=num_folds,labels=FALSE) # Create k equally size folds
  folds <- folds[sample(length(folds))]  # Randomly shuffle the folds
  # SVM for PL matrix - use ksvm from kernlab (slow); svm from e1071 results in stack overflow 
  num_sv <- 0 # total number of support vectors
  pred_table <- matrix(0,nrow=nlevels(svm_classes),ncol=nlevels(svm_classes))
  i <- 1
  for(i in 1:num_folds){
    # Segement your data by fold using the which() function 
    testIndices <- which(folds==i,arr.ind=TRUE)
    Ktrain <- as.kernelMatrix(Gram[-testIndices,-testIndices, drop=FALSE]) # training kernel
    # Fit the model
    svm_model <- ksvm(Ktrain,svm_classes[-testIndices],type="C-svc",kernel="matrix",C=cost)
    num_sv <- num_sv + nSV(svm_model)
    # Make the prediction (the dependent variable, without using the Type)
    Ktest <- as.kernelMatrix(Gram[testIndices, -testIndices,drop=F][,SVindex(svm_model), drop=F])
    svm_pred <- predict(svm_model, Ktest)
    pred_table <- pred_table + table(pred = svm_pred, true = svm_classes[testIndices])
  }
  print(pred_table)
  pred_accuracy <- sum(diag(pred_table)) / sum(pred_table)
  print(paste("Average number of Support Vectors:",num_sv/num_folds))
  print(paste("Predication Accuracy:",pred_accuracy)) 
}

# Classification of Persistence Landscapes via SVM and k-fold cross validation, in three ways
pl_classification <- function(plm_list, sample_classes, vector_length, classes_to_use, 
        degrees_to_use, num_folds, cost, pca_coords_to_use){
  require("kernlab")
  pl <- numeric()
  for (i in degrees_to_use){
    pl_matrix <- plm_list[[i+1]]
    pl <- cbind(pl, pl_matrix)
  }
  main="PCA for PL in degree(s)"
  for (i in degrees_to_use)
    main <- paste(main,i)
  # restrict the matrix pl to those figures that you want to use
  indices_to_use <- c()
  for (i in 1:length(classes_to_use))
    indices_to_use <- c(indices_to_use,which(sample_classes[]==classes_to_use[i]))
  pl <- pl[indices_to_use,,drop=F]
  svm_classes <- factor(sample_classes[indices_to_use,drop=F])
  # Principal Component Analysis
  pca_plot(pl,main,labels=sample_classes[indices_to_use],vector_length,plot_loadings=FALSE) # don't plot loading vectors - need to fix for degres 0:1 for example
  # SVM for using kernels - use ksvm from kernlab (can be slow)
  print("Classification using SVM, kernels, and k-fold cross validation")
  Gram <- crossprod(t(pl)) # Gram matrix
#  classification_svm_gram_short(Gram,svm_classes,num_folds,cost)
  classification_svm_gram(Gram,svm_classes,num_folds,cost)
  # Support Vector Classification with k-fold Cross Validation
  print("Classification using SVM and k-fold cross validation")
#  classification_svm_short(pl,svm_classes,num_folds,cost)
  classification_svm(pl,svm_classes,num_folds,cost)
  # SVM for first dozen or so PCA coordinates - use svm from e1071 (fast)
  print("Classification using PCA, SVM, and k-fold cross validation")
  pca <- prcomp(pl,center=TRUE,scale.=FALSE,tol=0.01)
#  classification_svm_short(pca$x[,pca_coords_to_use,drop=FALSE],svm_classes,num_folds,cost)
  classification_svm(pca$x[,pca_coords_to_use,drop=FALSE],svm_classes,num_folds,cost)
}

pl_classification_linear <- function(plm_list, sample_classes, vector_length, classes_to_use, 
                              degrees_to_use, num_folds, cost){
  require("LiblineaR")
  pl <- numeric()
  for (i in degrees_to_use)
    pl <- cbind(pl, plm_list[[i+2]])
  main="Difference of PL in degree(s)"
  for (i in degrees_to_use)
    main <- paste(main,i)
  # restrict the matrix pl to those figures that you want to use
  indices_to_use <- c()
  for (i in 1:length(classes_to_use))
    indices_to_use <- c(indices_to_use,which(sample_classes[]==classes_to_use[i]))
  pl <- pl[indices_to_use,,drop=F]
  svm_classes <- factor(sample_classes[indices_to_use,drop=F])
#  svm_model <- LiblineaR(pl,type=3,cost=cost,bias=FALSE)
#  w <- svm_model$W
#  for (i in 1:dim(w)[1])
#    plot_persistence_landscape_from_vector(w[i,],(grid_size+1)*num_directions,main)
  SVA <- rep(0,2)
  #  LiblineaR(pl,z,type=0,bias=FALSE,cross=10)
  SVA[1] <- LiblineaR(pl,type=3,cost=cost,bias=FALSE,cross=10)
  SVA[2] <- LiblineaR(pl,type=7,cost=cost,bias=FALSE,cross=10)
  SVA
}

ranked_distance_matrix <- function(dist_mat){
  ltv <- dist_mat[lower.tri(dist_mat)] # get vector of lower triangular matrix entries
  rltv <- rank(ltv) # ranks of entries in ltv
  n <- dim(dist_mat)[1]
  rank_matrix <- matrix(0L,nrow=number_of_points,ncol=number_of_points)
  c <- 1
  for (j in 1:(n-1))
    for (i in (j+1):n){
      rank_matrix[i,j] <- rltv[c]
      c <- c+1
    }
  rank_matrix <- rank_matrix + t(rank_matrix)
  return(rank_matrix)
}

pers_hom_ripser_old <- function(points, ripser_threshold, ripser_dimension, filename){
  # write pcd to text file
  pcd_filename <- paste(filename,".pcd",sep="")
  pcd_file <- file(pcd_filename,'w')
  write.table(points, pcd_file, row.names = FALSE, col.names = FALSE)
  close(pcd_file)
  rps_filename <- paste(filename,".rps",sep="")
  ripser_command <- paste("./bin/ripser --format point-cloud --threshold",
                          ripser_threshold,
                          "--dim",
                          ripser_dimension,
                          pcd_filename,">",rps_filename)
  start.time <- Sys.time()
  system(ripser_command)
  end.time <- Sys.time()
  time.taken <- end.time - start.time
  # print(time.taken)
  ph <- ripser2barcode(filename)
}

ripser2barcode <- function(filename){
  rps_filename <- paste(filename,".rps",sep="")
  rps_file <- file(rps_filename,'r')
  rps_lines <-readLines(rps_file)
  close(rps_file)
  new_degree <- vector(mode="logical",length(rps_lines))
  for (i in 4:length(rps_lines)){
    if (substring(rps_lines[i],1,1) == "p"){
      new_degree[i] <- TRUE
    }
  }
  new_degrees_at <- which(new_degree!=0)
  ph <- vector("list",length(new_degrees_at))
  new_degrees_at <- c(new_degrees_at,(length(rps_lines)+1))
  for (i in 1:(length(new_degrees_at)-1)){
    num_pairs <- new_degrees_at[i+1]-new_degrees_at[i]-1
    bd_pairs <- matrix(0,nrow=num_pairs,ncol=2)
    for (j in 1:num_pairs){
      j_shifted <- j + new_degrees_at[i]
      pair_string <- gsub(")","",gsub("[","",gsub(" ","",rps_lines[j_shifted],fixed=T),fixed=T),fixed=T)
      pair <- scan(text = pair_string, sep = ",", quiet = TRUE)
      if (is.na(pair[2])) pair[2] <- -1
      bd_pairs[j,] <- pair
    }
    ph[[i]] <- bd_pairs
  }
  ph
}

pers_hom_ripser <- function(rip.input, 
                                 rip.format, 
                                 rip.threshold = -1,
                                 rip.dim = -1){
  # rip.input = point cloud or distance matrix
  # rip.format = "point-cloud" or "lower-distance"
  # rip.threshold = maximum diameter of Rips filtration, -1 for all diameters
  # rip.dim = max dimension of homology computed, -1 for all dimensions
  # by Alexander Wagner
  
  # Convert input to character array
  if (rip.format == "point-cloud"){
    rip.input <- do.call(paste, c(as.data.frame(rip.input)))
  }
  if (rip.format == "lower-distance"){
    rip.input <- paste(t(rip.input)[upper.tri(rip.input)], collapse = " ")
  }
  
  # Create system command based on whether threshold or not
  rip.args <- paste("--format", rip.format)
  if (rip.dim != -1){
    rip.args <- paste(rip.args, paste("--dim", rip.dim))
  }
  if (rip.threshold != -1){
    rip.args <- paste(rip.args, paste("--threshold", rip.threshold))
  }
  
  # Use Ripser
  barcode <- system2(command = "./bin/ripser", args = rip.args, 
                     stdout = TRUE,
                     input = rip.input)
  
  # Convert text output to list of matrices giving barcodes
  new.degree <- grepl(pattern = "persistence", x = barcode)
  new.degrees.at <- which(new.degree)
  ph <- vector("list", length(new.degrees.at))
  new.degrees.at <- c(new.degrees.at, (length(barcode) + 1))
  for (i in 1:(length(new.degrees.at) - 1)){
    num.pairs <- new.degrees.at[i + 1] - new.degrees.at[i] - 1
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

pers_hom_rips_perseus <- function(points, step_size, num_steps, filename, binary_directory, operating_system){
  ambient_dim <- dim(points)[2]
  num_points <- dim(points)[1]
  points_with_radius <- cbind(points,rep(0,num_points)) # give each point an initial radius of 0
  # write pcd to text file
  perseus_filename <- paste(filename,".prs",sep="")
  perseus_file <- file(perseus_filename,'w')
  write(ambient_dim,perseus_file)
  write.table(t(c(1, step_size, num_steps)), perseus_file, append = TRUE, row.names = FALSE, col.names = FALSE)
  write.table(points_with_radius, perseus_file, append = TRUE, row.names = FALSE, col.names = FALSE)
  close(perseus_file)
  if (operating_system == 'Mac'){
    perseus_file <- file.path("..",binary_directory,"perseusMac")
  } else if (operating_system == 'PC'){
    perseus_file <- file.path("..",binary_directory,"perseusWin")
  } else if (operating_system == 'Linux')
    perseus_file <- file.path("..",binary_directory,"perseusLin")
  perseus_command <- paste(perseus_file,"brips",perseus_filename,filename,sep=" ")
  system(perseus_command)
  ph_0 <- as.matrix(read.table(paste(filename,"_0.txt",sep="")))
  ph_list <- list(ph_0)
  if (!file.exists(paste(filename,"_1.txt",sep="")) || (file.size(paste(filename,"_1.txt",sep="")) == 0)){
    ph_1 <- matrix(,nrow=0,ncol=2)
  } else{
    ph_1 <- as.matrix(read.table(paste(filename,"_1.txt",sep="")))
  }
  ph <- list(ph_0,ph_1)
}

pers_hom_filtered_cubical_complex <- function(filtration_array, filename, binary_directory, operating_system){
  dimension <- length(dim(filtration_array))
  # write filtration_array to perseus input text file
  perseus_filename <- paste(filename,".prs",sep="")
  perseus_file <- file(perseus_filename,'w')
  write(dimension, perseus_file)
  for (i in 1:dimension)  
    write(dim(filtration_array)[i], perseus_file)
  filtration_vector <- c(filtration_array)
  for (i in 1:length(filtration_vector))
    write(filtration_vector[i], perseus_file)
  close(perseus_file)
  if (operating_system == 'Mac'){
    perseus_file <- file.path("..",binary_directory,"perseusMac")
  } else if (operating_system == 'PC'){
    perseus_file <- file.path("..",binary_directory,"perseusWin")
  } else if (operating_system == 'Linux')
    perseus_file <- file.path("..",binary_directory,"perseusLin")
  perseus_command <- paste(perseus_file,"cubtop",perseus_filename,filename,sep=" ")
  system(perseus_command)
  ph <- vector("list",dimension)
  for (i in 1:dimension){
    if (!file.exists(paste(filename,"_",i-1,".txt",sep="")) || (file.size(paste(filename,"_",i-1,".txt",sep="")) == 0)){
      ph[[i]] <- matrix(,nrow=0,ncol=2)
    } else{
      ph[[i]] <- as.matrix(read.table(paste(filename,"_",i-1,".txt",sep="")))
    }
  }
  ph
}

persistence_landscape <- function(bd_list,max_filtration_value){
  pl_list <- list()
  for (i in length(bd_list)){
    bd <- bd_list[[i]]
    if (dim(bd)[1] > 0)
      bd[bd[,2]==-1,2] <- max_filtration_value #threshold infinite intervals
    pl_text <- system2(command = "./bin/ComputePersistenceLandscape", stdout = TRUE, input = paste(t(bd),collapse = " "))
    # Convert text output to list of matrices giving persistent landscape critical points and values
    lambda_label <- integer(length(pl_text)) # how many landscape functions?
    for (j in 1:length(pl_text)){
      if (substring(pl_text[j],1,1) == "#") lambda_label[j] <- 1
    }
    lambda_label_locations <- which(lambda_label == 1)
    lambda_label_locations <- c(lambda_label_locations,(length(lambda_label)+1))
    pl <- list()
    for (j in 1:(length(lambda_label_locations)-1)){
      num_crit_pts <- lambda_label_locations[j+1]-lambda_label_locations[j]-1
      lambda <- matrix(0, nrow=num_crit_pts, ncol=2)
      for (k in 1:num_crit_pts){
        lambda[k,] <- t(as.numeric(unlist(strsplit(pl_text[lambda_label_locations[j]+k],split=" "))))
      }
      pl[[j]] <- lambda
    }
    pl_list[[i]] <- pl
    return(pl_list)
  }
}

persistence_landscape_discretize <- function(pl_exact,pl_param_vals){
  pl_vector <- list()
    for (j in 1:length(pl_exact)){
      PLy <- rep(0,length(pl_param_vals))
      for (k in 1:length(pl_param_vals))
        PLy[k] <- persistence_landscape_value(pl_exact[[j]],pl_param_vals[k])
      pl_vector <- c(pl_vector,PLy)
    }
  pl_vector
}
