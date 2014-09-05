# Auto generated configuration file
# using: 
# Revision: 1.232.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/PYTHIA6_inclB0toJpsiMuMu_8TeV_cff.py -s GEN:ProductionFilterSequence --eventcontent RECOSIM --datatier GEN-SIM --conditions STARTHI53_V17HI::All --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.HiGenCommon.VtxSmearedRealisticPPbBoost8TeVCollision_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_inclB0toJpsiMuMu_8TeV_cff.py,v $'),
    annotation = cms.untracked.string('Winter13: Pythia6+EvtGen generation of B0->Jpsi->MuMu, 5.023TeV, Z2star tune'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(350)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(True)
)

# boost direction (2nd run pPb)
process.VtxSmeared.Beta=cms.double(0.434)

# seeds
#from IOMC.RandomEngine.RandomServiceHelper import RandomNumberServiceHelper
#randSvc = RandomNumberServiceHelper(process.RandomNumberGeneratorService)
#randSvc.populate()
#print process.RandomNumberGeneratorService.dumpConfig()

##from Configuration.AlCa.GlobalTag import GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'STARTHI53_V27::All'

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'STARTHI53_V27::All', '')

from Configuration.Generator.PythiaUEZ2starSettings_cfi import *

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
    #   BF(B0->JPsiX) * BF(JPsi->mumu) = 0.0185 * 0.0593
    crossSection = cms.untracked.double(54402709.5),#8978000.0
    filterEfficiency = cms.untracked.double(0.0003114),#0.0513
    maxEventsToPrint = cms.untracked.int32(0),
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
             operates_on_particles = cms.vint32( 0 ), # 0 (zero) means default list (hardcoded)
                                                      # you can put here the list of particles (PDG IDs)
                                                      # that you want decayed by EvtGen
             use_default_decay = cms.untracked.bool(False),
             decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),
             #decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY.DEC'),
             particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
             #user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/Validation.dec'),
             user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/incl_BtoJpsi_mumu.dec'),
             list_forced_decays = cms.vstring('MyB0',
                                              'Myanti-B0',
                                              'MyB+',
                                              'MyB-',
                                              'MyB_s0',
                                              'Myanti-B_s0',
                                              'MyLambda_b0',
                                              'Myanti-Lambda_b0'),
        ),
        parameterSets = cms.vstring('EvtGen')
    ),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        bbbarSettings = cms.vstring('MSEL = 1'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring(
             'pythiaUESettings',
             'bbbarSettings')
    )
)

## for filters

process.bfilter = cms.EDFilter("PythiaFilter",
    ParticleID = cms.untracked.int32(5)
)

process.oniafilter = cms.EDFilter("MCSingleParticleFilter",
    Status = cms.untracked.vint32(     2,    2),
#    ParticleID = cms.untracked.vint32(443, 100443),
    ParticleID = cms.untracked.vint32(443),
    MinPt = cms.untracked.vdouble(    0.0, 0.0),
    MaxEta = cms.untracked.vdouble(   1000.0, 1000.0),
    MinEta = cms.untracked.vdouble(   -1000.0, -1000.0),
)

process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinP = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(-1),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)

#process.ProductionFilterSequence = cms.Sequence(process.generator*process.VtxSmeared*process.bfilter*process.oniafilter*process.mumugenfilter)
process.ProductionFilterSequence = cms.Sequence(process.generator*process.bfilter*process.oniafilter*process.VtxSmeared*process.mumugenfilter)
# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
		outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string(
	'B2Jpsi_PYTHIA_2ndBoost_502TeV_GENSIM.root'
	),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)


# Path and EndPath definitions
process.generation_step = cms.Path( process.ProductionFilterSequence * process.genParticles)
process.simulation_step = cms.Path(process.psim)

process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# avoid problem with unknown pdg code
process.hiGenParticles.abortOnUnknownPDGCode = cms.untracked.bool(False)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq

