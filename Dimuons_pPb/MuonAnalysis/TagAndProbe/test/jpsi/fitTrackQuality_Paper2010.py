import FWCore.ParameterSet.Config as cms

### USAGE:
###    cmsRun fitTrackQuality_Paper2010.py <scenario>
### scenarios:
###   - data_all (default)  
###   - signal_mc

import sys
args = sys.argv[1:]
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "data_all"
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "2.8", "3.35", "GeV/c^{2}"),
        pt = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
        eta    = cms.vstring("Probe |#eta|", "-2.5", "2.5", ""),
        phi    = cms.vstring("Probe #phi", "-3.1416", "3.1416", ""),
        pair_Nvertices = cms.vstring("Number of vertices", "0", "999", ""),
        tkPixelLay  = cms.vstring("Pixel layers", "0", "5",  ""),
        tkValidHits = cms.vstring("Pixel layers", "0", "50", ""),
        tkExpHitInNew = cms.vstring("Tk expected hits in", "-1", "50", ""),
        tkExpHitOutNew = cms.vstring("Tk expected hits in", "-1", "50", ""),
    ),

    Categories = cms.PSet(
        TM = cms.vstring("TM", "dummy[pass=1,fail=0]"),
        Track_HP   = cms.vstring("TM", "dummy[pass=1,fail=0]"),
        Track_QTF  = cms.vstring("TM", "dummy[pass=1,fail=0]"),
        Track_VBTF = cms.vstring("TM", "dummy[pass=1,fail=0]"),
        MuX_L2Mu0_L2     = cms.vstring("X", "dummy[pass=1,fail=0]"),
        tag_Mu5_L2Mu0_MU = cms.vstring("X", "dummy[pass=1,fail=0]"),
        Mu3_Track5_Jpsi_TK     = cms.vstring("ProbeTrigger_Track0", "dummy[pass=1,fail=0]"),
        tag_Mu3_Track5_Jpsi_MU = cms.vstring("ProbeTrigger_Track0", "dummy[pass=1,fail=0]"),
        mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),
    ),

    Cuts = cms.PSet(
        Cut_OnePixel = cms.vstring("OnePixe",     "tkPixelLay", "0.5"),
        Cut_TwoPixel = cms.vstring("TwoPixelLay", "tkPixelLay", "1.5"),
        Cut_gt7Hits  = cms.vstring("TenHits","tkValidHits",  "7.5"),
        Cut_gt10Hits = cms.vstring("TenHits","tkValidHits", "10.5"),
        Cut_gt11Hits = cms.vstring("TenHits","tkValidHits", "11.5"),
        Cut_0missingIn = cms.vstring("xx", "tkExpHitInNew", "0.5"),
        Cut_1missingIn = cms.vstring("xx", "tkExpHitInNew", "1.5"),
        Cut_0missingOut = cms.vstring("xx", "tkExpHitOutNew", "0.5"),
        Cut_1missingOut = cms.vstring("xx", "tkExpHitOutNew", "1.5"),
    ),

    PDFs = cms.PSet(
        cbPlusPoly = cms.vstring(
            "CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.05,0.02,0.1], alpha[3., 0.5, 5.], n[1, 0., 100.])",
            "Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-0.5,0.5]})",
            "Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-0.5,0.5]})",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        )
    ),

    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(40),

    Efficiencies = cms.PSet(), # will be filled later
)

CONSTRAINTS = cms.PSet(
    TM  = cms.bool(True),
    MuX_L2Mu0_L2     = cms.vstring("pass"),
    tag_Mu5_L2Mu0_MU = cms.vstring("pass"),
)
BIASCONSTRAINTS  = cms.PSet(
    TM  = cms.bool(True),
    Mu3_Track5_Jpsi_TK     = cms.vstring("pass"),
    tag_Mu3_Track5_Jpsi_MU = cms.vstring("pass"),
)
ONE_BIN = cms.PSet(CONSTRAINTS,
    pt  = cms.vdouble( 0, 20 ),
    eta = cms.vdouble(-2.4, 2.4),
)
ETA_BINS = ONE_BIN.clone(
   eta = cms.vdouble(-2.4, -2.1, -1.8, -1.5, -1.2, -0.9, -0.6, -0.3, 0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4)
)
PHI_BINS =  cms.PSet(CONSTRAINTS,
    pt  = cms.vdouble( 3, 20 ),
    abseta = cms.vdouble(0, 0.8, 1.4, 2.1, 2.4),
    phi = cms.vdouble(*[3.1416*i/3.0 for i in range(-3,3)]), 
)

PLATEAU_ABSETA = cms.PSet(CONSTRAINTS,
    abseta = cms.vdouble(0.0,  1.2, 2.4),
    pt     = cms.vdouble(4.0, 20.0),
)
PLATEAU_ENDCAPS21 = cms.PSet(CONSTRAINTS,
    abseta = cms.vdouble(1.2, 2.1),
    pt     = cms.vdouble(4.0, 20.0),
)
PLATEAU_ABSETA_BIAS = cms.PSet(BIASCONSTRAINTS,
    abseta = cms.vdouble(0.0,  1.2, 2.4),
    pt     = cms.vdouble(4.0, 20.0),
)
PLATEAU_ENDCAPS21_BIAS = cms.PSet(BIASCONSTRAINTS,
    abseta = cms.vdouble(1.2, 2.1),
    pt     = cms.vdouble(4.0, 20.0),
)



PREFIX="/data/gpetrucc/7TeV/tnp/2011.02.17/"
process.TnP_TrackQuality = Template.clone(
    InputFileNames = cms.vstring(PREFIX+'tnpJPsi_Data_Nov4B.root'),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("TnP_Paper2010_TrackQuality_%s.root" % scenario),
    Efficiencies = cms.PSet(),
)

if scenario == "signal_mc":
    process.TnP_TrackQuality.InputFileNames = [ PREFIX+"tnpJPsi_MC_Prompt.root" ]
if scenario == "some_mc":
    process.TnP_TrackQuality.InputFileNames = [ PREFIX+"tnpJPsi_MC_Prompt.crop.root" ]
if scenario == "beauty_mc":
    process.TnP_TrackQuality.InputFileNames = [ PREFIX+"tnpJPsi_MC_Bp.root" ]

IDS = [ "Track_HP", "Track_QTF", "Track_VBTF", "Cut_OnePixel", "Cut_TwoPixel", "Cut_gt10Hits", "Cut_gt11Hits", 
        "Cut_0missingIn", "Cut_1missingIn", "Cut_0missingOut", "Cut_1missingOut" ]
ALLBINS=[("eta",ETA_BINS),("phi",PHI_BINS)]

ALLBINS =[("plateau_abseta",PLATEAU_ABSETA),          ("plateau_endcaps21",PLATEAU_ABSETA)]
#ALLBINS+=[("plateau_abseta_bias",PLATEAU_ABSETA_BIAS),("plateau_endcaps21_bias",PLATEAU_ABSETA_BIAS)]

IDS = [ "Track_VBTF" ]
process.TnP_TrackQuality.Cuts = cms.PSet()

for ID in IDS:
    for X,B in ALLBINS:
        module = process.TnP_TrackQuality.clone(OutputFileName = cms.string("TnP_Paper2010_TrackQuality_%s_%s_%s.root" % (scenario, ID, X)))
        DEN=B.clone()
        passing = "above" if ID.startswith("Cut_") else "pass";
        if ID.startswith("Cut_") and ID.find("missing") != -1: passing = "below"
        setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(ID,passing),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = DEN,
            BinToPDFmap = cms.vstring("cbPlusPoly")
        ))
        if "mc" in scenario:
            setattr(module.Efficiencies, ID+"_"+X+"_mcTrue", cms.PSet(
                EfficiencyCategoryAndState = cms.vstring(ID,passing),
                UnbinnedVariables = cms.vstring("mass"),
                BinnedVariables = DEN.clone(mcTrue = cms.vstring("true"))
            ))
        setattr(process, "TnP_TrackQuality_"+ID+"_"+X, module)        
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))

