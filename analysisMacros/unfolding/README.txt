
A) for applying inversion matrix in the actual analysis
	step1) rootAna_recoPt_genPt_pythia_diffRap.C : fill reco pt vs gen pt hist
	step2-a) matrixInversion_AN.C : draw matrix & inverted matrix plots for AN
	step2-b) matrixInversion_real.C : inverted matrix actually applied to the analysis result.


B) for closure test  (fine pT binning)
	step1) rootAna_recoPt_genPt_pythia_test.C : fill reco pt vs gen pt hist
	step2) Decomposition.C : matrix inversion & closure test 

