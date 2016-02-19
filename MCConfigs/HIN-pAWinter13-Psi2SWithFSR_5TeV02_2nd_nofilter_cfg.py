# Auto generated configuration file
# using: 
# Revision: 1.381.2.28 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/HI/PYTHIA6_Psi2SWithFSR_tuneD6T_5TeV02_nofilter_cff.py --fileout file:HIN-pAWinter13-Psi2SWithFSR_5TeV02_2nd_nofilter.root --mc --eventcontent RAWSIM --pileup NoPileUp --datatier GEN --conditions STARTHI53_V27::All --beamspot Realistic5TeVPPbBoostReversed --step GEN --datamix NODATAMIXER --python_filename HIN-pAWinter13-Psi2SWithFSR_5TeV02_2nd_nofilter_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring,GeneratorInterface/HiGenCommon/customiseSwapVtxSmearAndProdFilter_cfi.customiseSwapVtxSmearAndProdFilter -n 100
import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.HiGenCommon.VtxSmearedRealistic5TeVPPbBoostReversed_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('Winter13: Pythia6 generation of prompt Psi2S, 5.023TeV, D6T tune'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_Psi2SWithFSR_tuneD6T_8TeV_cff.py,v $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:HIN-pAWinter13-Psi2SWithFSR_5TeV02_2nd_nofilter.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'STARTHI53_V27::All', '')

process.oniafilter = cms.EDFilter("PythiaFilter",
    MaxEta = cms.untracked.double(1e+100),
    Status = cms.untracked.int32(2),
    MinEta = cms.untracked.double(-1e+100),
    MinPt = cms.untracked.double(0.0),
    ParticleID = cms.untracked.int32(100443)
)


process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
            use_default_decay = cms.untracked.bool(False),
            operates_on_particles = cms.vint32(100443),
            particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
            user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/Onia_mumu.dec'),
            list_forced_decays = cms.vstring('Mypsi(2S)'),
            use_internal_pythia = cms.untracked.bool(False),
            decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC')
        ),
        parameterSets = cms.vstring('EvtGen')
    ),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.0271),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
    crossSection = cms.untracked.double(45915.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model', 
            'MSTU(21)=1     ! Check on possible errors during program execution', 
            'PARP(82)=1.8387   ! pt cutoff for multiparton interactions', 
            'PARP(89)=1960. ! sqrts for which PARP82 is set', 
            'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
            'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
            'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
            'PARP(67)=2.5    ! amount of initial-state radiation', 
            'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
            'PARP(62)=1.25   ! ', 
            'PARP(64)=0.2    ! ', 
            'MSTP(91)=1      !', 
            'PARP(91)=2.1   ! kt distribution', 
            'PARP(93)=15.0  ! '),
        processParameters = cms.vstring('MSEL=61          ! Quarkonia', 
            'KFPR(421,1)  = 100443    ! change 421 to Psi(2S) + g', 
            'PMAS(362,1)  = 3.70000   ! change cc~ mass larger than Psi(2S) 3.68600', 
            'PMAS(363,1)  = 3.70000   ! change cc~ mass larger than Psi(2S) 3.68600', 
            'PMAS(364,1)  = 3.70000   ! change cc~ mass larger than Psi(2S) 3.68600', 
            'KFDP(4211,1) = 100443    ! cc~ -> Psi(2S)', 
            'KFDP(4212,1) = 100443    ! cc~ -> Psi(2S)', 
            'KFDP(4213,1) = 100443    ! cc~ -> Psi(2S)', 
            'PARP(141)=0.76   ! New values for COM matrix elements', 
            'PARP(142)=0.0050 ! New values for COM matrix elements', 
            'PARP(143)=0.0042 ! New values for COM matrix elements', 
            'PARP(144)=0.0042 ! New values for COM matrix elements', 
            'PARP(145)=0      ! New values for COM matrix elements', 
            'MDME(1567,1) = 0 ! 0.008300    e-              e+', 
            'MDME(1568,1) = 1 ! 0.008300    mu-             mu+', 
            'MDME(1569,1) = 0 ! 0.186600    rndmflav        rndmflavbar', 
            'MDME(1570,1) = 0 ! 0.324000    J/psi           pi+             pi-', 
            'MDME(1571,1) = 0 ! 0.184000    J/psi           pi0             pi0', 
            'MDME(1572,1) = 0 ! 0.027000    J/psi           eta', 
            'MDME(1573,1) = 0 ! 0.001000    J/psi           pi0', 
            'MDME(1574,1) = 0 ! 0.093000    chi_0c          gamma', 
            'MDME(1575,1) = 0 ! 0.087000    chi_1c          gamma', 
            'MDME(1576,1) = 0 ! 0.078000    chi_2c          gamma', 
            'MDME(1577,1) = 0 ! 0.002800    eta_c           gamma', 
            'MSTP(142)=2      ! turns on the PYEVWT Pt re-weighting routine', 
            'PARJ(13)=0.750   ! probability that a c or b meson has S=1', 
            'PARJ(14)=0.162   ! probability that a meson with S=0 is produced with L=1, J=1', 
            'PARJ(15)=0.018   ! probability that a meson with S=1 is produced with L=1, J=0', 
            'PARJ(16)=0.054   ! probability that a meson with S=1 is produced with L=1, J=1', 
            'MSTP(145)=0      ! choice of polarization', 
            'MSTP(146)=0      ! choice of polarization frame ONLY when mstp(145)=1', 
            'MSTP(147)=0      ! particular helicity or density matrix component when mstp(145)=1', 
            'MSTP(148)=1      ! possibility to allow for final-state shower evolution, extreme case !', 
            'MSTP(149)=1      ! if mstp(148)=1, it determines the kinematics of the QQ~3S1(8)->QQ~3S1(8)+g branching'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters', 
            'CSAParameters'),
        CSAParameters = cms.vstring('CSAMODE = 6     ! cross-section reweighted quarkonia')
    )
)


process.ProductionFilterSequence = cms.Sequence(process.generator+process.oniafilter)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from GeneratorInterface.HiGenCommon.customiseSwapVtxSmearAndProdFilter_cfi
from GeneratorInterface.HiGenCommon.customiseSwapVtxSmearAndProdFilter_cfi import customiseSwapVtxSmearAndProdFilter 

#call to customisation function customiseSwapVtxSmearAndProdFilter imported from GeneratorInterface.HiGenCommon.customiseSwapVtxSmearAndProdFilter_cfi
process = customiseSwapVtxSmearAndProdFilter(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
