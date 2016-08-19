# Final plots
  - input is needed from [FittingResults/totalHist_XXX.root] and [TotalSys/TotSys_XXX.root]

#./shell_all.sh : to run everything
  - ./shell_draw_cross.sh
  - ./shell_draw_RpPb.sh : input needed from [plot_cross]
  - ./shell_draw_RFB.sh 
  - ./shell_comp.sh : input needed from [plot_cross], [plot_RpPb], [plot_RFB]
