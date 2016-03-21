import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


# Track selections for Trigger efficiency study
#everyCut = cms.vstring("TRACK_MUON","pass", "PADoubleMuOpen","pass")
everyCut = cms.vstring("TRACK_MUON","pass","PADoubleMuOpen","pass")

ptBinning1 = cms.vdouble(3.3, 3.8, 4.3, 5, 6, 7, 8, 10, 30)
ptBinning2 = cms.vdouble(3.3, 3.5, 3.9, 4.3, 5, 6, 7, 30)
ptBinning3 = cms.vdouble(1.13, 1.5, 2.16, 3.0, 3.6, 4.3, 5, 6, 7, 8, 30)
ptBinning4 = cms.vdouble(0.7, 1.2, 1.6, 2.2, 2.5, 2.8, 3.5, 4.5, 5, 6, 9, 30)
ptBinning5 = cms.vdouble(0.63, 1.3, 1.8, 2.2, 2.8, 3.4, 4.8, 6, 30)

process.TnP_Trigger = cms.EDAnalyzer("TagProbeFitTreeAnalyzer", 
    ## Input, output 
                                     InputFileNames = cms.vstring("file:/home/kilee/CMSSW_5_3_20/UserCode/skim/Data/tnpTree_data_allDirection_v27.root"),
                                     OutputFileName = cms.string("../ResultsFit/outputEveryCut_data_allDirection_v27_nominalCond_5etaBins_cutG_all_nominal_v3.root"),
                                     InputDirectoryName = cms.string("tpTree"),
                                     InputTreeName = cms.string("fitter_tree"),
                                     ## vertex weighitng
  #                                   WeightVariable = cms.string("vtxWeight"),
                                     ## Variables for binning
                                     Variables = cms.PSet(
        mass   = cms.vstring("Tag-Probe Mass", "2.6", "3.5", "GeV/c^{2}"),
        pt     = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
        eta    = cms.vstring("Probe #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
#        vtxWeight = cms.vstring("vtx_z weight","0","1000",""),
        event_PrimaryVertex_z = cms.vstring("V_z", "-10", "10", "cm"),
        tag_pt   = cms.vstring("tag_pt", "0", "1000", "GeV"),
        tag_eta   = cms.vstring("tag_eta", "-2.4", "2.4", ""),
        pair_pt   = cms.vstring("pair_pt", "0", "1000", "GeV"),
        #tag_nValHits   = cms.vstring("tag_nValHits", "-1.", "200.", "Counts"),
        ),
                                     ## Flags you want to use to define numerator and possibly denominator
                                     Categories = cms.PSet(
	PADoubleMuOpen = cms.vstring("PADoubleMuOpen", "dummy[pass=1,fail=0]"),
        tag_PAMu3_standard = cms.vstring("tag_PAMu3_standard", "dummy[pass=1,fail=0]"),
        Acc_JPsi= cms.vstring("Acc_JPsi", "dummy[pass=1,fail=0]"),	
        Acc_JPsiOLD= cms.vstring("Acc_JPsiOLD", "dummy[pass=1,fail=0]"),	
        Calo= cms.vstring("Calo", "dummy[pass=1,fail=0]"),	
        #        TrackCuts= cms.vstring("TrackCuts", "dummy[pass=1,fail=0]"),
        TRACK_MUON= cms.vstring("TRACK_MUON", "dummy[pass=1,fail=0]"),
        MuonID= cms.vstring("MuonID", "dummy[pass=1,fail=0]"),
        ),
                                     ## What to fit
                                     Efficiencies = cms.PSet(
        ptBin_eta1_0 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(3.3, 3.8),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_0"), ## PDF to use, as defined below
            ),
        ptBin_eta1_1 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(3.8, 4.3),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_1"), ## PDF to use, as defined below
            ),
        ptBin_eta1_2 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(4.3, 5.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_2"), ## PDF to use, as defined below
            ),
        ptBin_eta1_3 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(5.0, 6.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_3"), ## PDF to use, as defined below
            ),
        ptBin_eta1_4 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(6.0, 7.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_4"), ## PDF to use, as defined below
            ),
        ptBin_eta1_5 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(7.0, 8.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_5"), ## PDF to use, as defined below
            ),
        ptBin_eta1_6 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(8.0, 10.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_6"), ## PDF to use, as defined below
            ),
        ptBin_eta1_7 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0,0.9),
                pt = cms.vdouble(10.0, 30.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo1_7"), ## PDF to use, as defined below
            ),
        ptBin_eta2_0 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(3.3, 3.5),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_0"), ## PDF to use, as defined below
            ),
        ptBin_eta2_1 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(3.5, 3.9),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_1"), ## PDF to use, as defined below
            ),
        ptBin_eta2_2 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(3.9, 4.3),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_2"), ## PDF to use, as defined below
            ),
        ptBin_eta2_3 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(4.3, 5.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_3"), ## PDF to use, as defined below
            ),
        ptBin_eta2_4 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(5.0, 6.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_4"), ## PDF to use, as defined below
            ),
        ptBin_eta2_5 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(6.0, 7.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_5"), ## PDF to use, as defined below
            ),
        ptBin_eta2_6 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(0.9, 1.2),
                pt = cms.vdouble(7.0, 30.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo2_6"), ## PDF to use, as defined below
            ),
        ptBin_eta3_0 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(1.13, 1.5),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_0"), ## PDF to use, as defined below
            ),
        ptBin_eta3_1 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(1.5, 2.16),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_1"), ## PDF to use, as defined below
            ),
        ptBin_eta3_2 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(2.16, 3.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_2"), ## PDF to use, as defined below
            ),
        ptBin_eta3_3 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(3.0, 3.6),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_3"), ## PDF to use, as defined below
            ),
        ptBin_eta3_4 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(3.6, 4.3),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_4"), ## PDF to use, as defined below
            ),
        ptBin_eta3_5 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(4.3, 5.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_5"), ## PDF to use, as defined below
            ),
        ptBin_eta3_6 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(5.0, 6.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_6"), ## PDF to use, as defined below
            ),
        ptBin_eta3_7 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(6.0, 7.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_7"), ## PDF to use, as defined below
            ),
        ptBin_eta3_8 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(7.0, 8.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_8"), ## PDF to use, as defined below
            ),
        ptBin_eta3_9 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.2, 1.6),
                pt = cms.vdouble(8.0, 30.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo3_9"), ## PDF to use, as defined below
            ),
        ptBin_eta4_0 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(0.7, 1.2),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_0"), ## PDF to use, as defined below
            ),
        ptBin_eta4_1 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(1.2, 1.6),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_1"), ## PDF to use, as defined below
            ),
        ptBin_eta4_2 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(1.6, 2.2),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_2"), ## PDF to use, as defined below
            ),
        ptBin_eta4_3 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(2.2, 2.5),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_3"), ## PDF to use, as defined below
            ),
        ptBin_eta4_4 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(2.5, 2.8),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_4"), ## PDF to use, as defined below
            ),
        ptBin_eta4_5 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(2.8, 3.5),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_5"), ## PDF to use, as defined below
            ),
        ptBin_eta4_6 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(3.5, 4.5),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_6"), ## PDF to use, as defined below
            ),
        ptBin_eta4_7 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(4.5, 5.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_7"), ## PDF to use, as defined below
            ),
        ptBin_eta4_8 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(5.0, 6.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_8"), ## PDF to use, as defined below
            ),
        ptBin_eta4_9 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(6.0, 9.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_9"), ## PDF to use, as defined below
            ),
        ptBin_eta4_10 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(1.6, 2.1),
                pt = cms.vdouble(9.0, 30.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo4_10"), ## PDF to use, as defined below
            ),
        ptBin_eta5_0 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(0.63, 1.3),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_0"), ## PDF to use, as defined below
            ),
        ptBin_eta5_1 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(1.3, 1.8),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_1"), ## PDF to use, as defined below
            ),
        ptBin_eta5_2 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(1.8, 2.2),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_2"), ## PDF to use, as defined below
            ),
        ptBin_eta5_3 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(2.2, 2.8),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_3"), ## PDF to use, as defined below
            ),
        ptBin_eta5_4 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(2.8, 3.4),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_4"), ## PDF to use, as defined below
            ),
        ptBin_eta5_5 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(3.4, 4.8),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_5"), ## PDF to use, as defined below
            ),
        ptBin_eta5_6 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(4.8, 6.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_6"), ## PDF to use, as defined below
            ),
        ptBin_eta5_7 = cms.PSet(
            EfficiencyCategoryAndState = everyCut,  ## Numerator definition
            UnbinnedVariables = cms.vstring("mass"),               # for mc
            BinnedVariables = cms.PSet(
                ## Binning in continuous variables
                abseta = cms.vdouble(2.1, 2.4),
                pt = cms.vdouble(6.0, 30.0),
                event_PrimaryVertex_z =cms.vdouble(-10, 10),
                tag_pt =cms.vdouble(3.0,1000),
                tag_eta =cms.vdouble(-2.4,2.4),
                tag_PAMu3_standard = cms.vstring("pass"),
                Acc_JPsiOLD = cms.vstring("pass"),
                Calo = cms.vstring("pass"),
                ),
            BinToPDFmap = cms.vstring("cbGaussPlusExpo5_7"), ## PDF to use, as defined below
            ),
        ),
                                     
                                     
                                     ## PDF for signal and background (double voigtian + exponential background)
                                     PDFs = cms.PSet(
        
        cbGaussPlusExpo1_0 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.1, 100.0], n[1, 0.1, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.1, 100.0], n[1, 0.1, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_2 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[10., 0.1, 100.0], n[1, 0.0, 30.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_3 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.1, 100.0], n[1, 0.1, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_4 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.1, 100.0], n[1, 0.0, 5.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_5 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.0, 100.0], n[1, 0.1, 5.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_6 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.0, 100.0], n[1, 0.0, 3.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-1.,0.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo1_7 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.1, 100.0], n[1, 0.1, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_0 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.1, 100.0], n[1, 0.1, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.1, 100.0], n[1, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.,5.])",
            "Exponential::backgroundFail(mass, lf[0,-5.,5.])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_2 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 100.0], n[1.0, 0.0, 50.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,0.1])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_3 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[10.0, 0.1, 100.0], n[1.0, 0.0, 300.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_4 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 100.0], n[1, 0.0, 50.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_5 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.1, 100.0], n[1, 0.0, 50.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo2_6 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.1, 100.0], n[1, 0.1, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_0 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 100.0], n[1, 0.0, 200.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 100.0], n[1, 0.0, 200.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_2 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 100.0], n[1, 0.0, 200.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_3 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[0.1, 0.0, 100.0], n[0.1, 0.0, 20.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.1,0.013,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_4 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[0.1, 0.0, 300.0], n[0.1, 0.0, 20.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_5 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[10., 0.0, 100.0], n[0.1, 0.0, 300000.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_6 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.1,0.017,2.0], alpha[1., 0.0, 100.0], n[1, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-0.0,1.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_7 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.0, 100.0], n[1, 0.0, 300.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-1.0,1.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_8 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.05,0.01,0.07], alpha[0.1, 0.0, 20.0], n[0.1, 0.0, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,0.1])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-1.0,0.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo3_9 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.0, 100.0], n[1, 0.0, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_0 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.05,0.015,2.0], alpha[5.0,0.0, 120.0], n[0, -1.0, 1.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,2.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-500.0,500.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.05,0.015,2.0], alpha[5.0,0.0, 120.0], n[0, -1.0, 1.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,2.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-500.0,500.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_2 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.05,0.015,2.0], alpha[5.0,0.0, 120.0], n[0, -1.0, 1.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,2.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-500.0,500.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_3 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[10.0,0.0, 30.0], n[20, -10.0, 20.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-15.0,15.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_4 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[2.0,0.0, 20.0], n[50.0, 0.0, 60.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-15.0,15.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_5 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[20.0,0.0,45.0], n[0, 0.0, 300.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_6 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0,0.0, 100.0], n[1, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_7 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0,0.0, 100.0], n[1, 0.0, 300.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_8 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0,0.1, 100.0], n[1, 0.1, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_9 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0,0.0, 100.0], n[1, 0.0, 5.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo4_10 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0,0.0, 1.0], n[1, 0.0, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-0.0,4.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_0 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[25., 0.0, 200.0], n[50, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[25., 0.0, 200.0], n[50, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_2 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[25., 0.0, 200.0], n[50, 0.0, 100.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_3 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[20.0, 0.0, 50.0], n[5, 0.0, 100.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_4 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[20., 0.0, 40.0], n[0, 0.0, 200.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-10.0,10.0])",
            "Exponential::backgroundFail(mass, lf[0,-10.0,10.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_5 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[0.1, 0.0, 0.5], n[0, -2.0, 5.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-15.0,15.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_6 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1., 0.0, 30.0], n[1, 0.0, 2.])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-15.0,15.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusExpo5_7 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,1.0], alpha[1.0, 0.0, 2.0], n[1, 0.0, 5.0])",
            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,1.0])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Exponential::backgroundPass(mass, lp[0,-5.0,5.0])",
            "Exponential::backgroundFail(mass, lf[0,-5.0,5.0])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        cbGaussPlusPoly1 = cms.vstring(
            "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,0.1], alpha[0.5, 0.2, 3.0], n[2, 0.5, 100.])",

            "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,0.1])",
            "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
            "Chebychev::backgroundPass(mass,{cP1[-0.5,-1.0,1.0],cP2[-0.1,-1.0,1.0]})",
            "Chebychev::backgroundFail(mass,{cF1[-0.5,-1.0,1.0],cF2[-0.1,-1.0,1.0]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
            ),
        ),
                                     
                                     
                                     ## How to do the fit
                                     binnedFit = cms.bool(True),
                                     binsForFit = cms.uint32(45),
                                     saveDistributionsPlot = cms.bool(True),
                                     NumCPU = cms.uint32(1), ## leave to 1 for now, RooFit gives funny results otherwise
                                     SaveWorkspace = cms.bool(True),
                                     )

#process.TnP_Trigger_mc = process.TnP_Trigger.clone()
#process.TnP_Trigger_mc.InputFileNames = cms.vstring("file:rootFiles/tnpTree_mc_allDirection_weightAdded_v13.root")
#process.TnP_Trigger_mc.OutputFileName = cms.string("ResultsFit/outputEveryCut_mc_allDirection_v2.root")


process.p = cms.Path(process.TnP_Trigger)# * process.TnP_Trigger_mc )

#  LocalWords:  cms
