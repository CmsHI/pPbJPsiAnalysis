import FWCore.ParameterSet.Config as cms

configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.1 $'),
        name = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_inclB0toJpsiMuMu_8TeV_cff.py,v $'),
        annotation = cms.untracked.string('Winter13: Pythia6+EvtGen generation of B0->Jpsi->MuMu, 5.023TeV, Z2star tune')
)

from Configuration.Generator.PythiaUEZ2starSettings_cfi import *

#generator = cms.EDFilter("Pythia6GeneratorFilter",
process.generator = cms.EDFilter("PyquenGeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
#   BF(B0->JPsiX) * BF(JPsi->mumu) = 0.0185 * 0.0593
    crossSection = cms.untracked.double(54402709.5),
    filterEfficiency = cms.untracked.double(0.0003114),
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
		
		filterType = cms.untracked.string('PartonHadronDecayGenEvtSelector'),
		partons = cms.vint32(5),
		partonStatus = cms.vint32(2),
		partonEtaMax = cms.vdouble(999.),
		partonPtMin  = cms.vdouble(0.),
		hadrons = cms.vint32(443,100443),
		hadronStatus = cms.vint32(2,2),
		hadronEtaMin = cms.vdouble(-1000.0,-1000.0),
		hadronEtaMax = cms.vdouble(1000.0,1000.0),
		hadronPtMin = cms.vdouble(0.0,0.0),
		hadronPtMax = cms.vdouble(50.0,50.0),
		hadronPMin = cms.vdouble(0.0,0.0),
		decays = cms.int32(13),
		decayStatus = cms.int32(1),
		decayPtMin = cms.double(0.0),
		decayPtMax = cms.double(999),
		decayEtaMin = cms.double(-2.03),
		decayEtaMax = cms.double(2.97),
		decayPMin = cms.double(1.7),
#		decayEtaMin = cms.double(-2.5),
#		decayEtaMax = cms.double(2.5),
#		decayPMin = cms.double(2.5),
		decayNtrig = cms.int32(2),

 		aBeamTarget = cms.double(208.0), ## beam/target atomic number
		angularSpectrumSelector = cms.int32(0), ## angular emitted gluon spectrum
		cFlag = cms.int32(0), ## centrality flag
		bMin = cms.double(0.0),  ## min impact param (fm); valid only if cflag_!=0
		bMax = cms.double(0.0),  ## max impact param (fm); valid only if cflag_!=0
		bFixed = cms.double(0.0), ## fixed impact param (fm); valid only if cflag_=0
		embeddingMode = cms.bool(False),
		doQuench = cms.bool(False),
		doIsospin = cms.bool(True),
		doCollisionalEnLoss = cms.bool(False),
		doRadiativeEnLoss = cms.bool(True),
		qgpNumQuarkFlavor = cms.int32(0), ## number of active quark flavors in qgp; allowed values: 0,1,2,3
		qgpProperTimeFormation = cms.double(0.1), ## proper time of QGP formation; allowed range [0.01,10.0]fm/c;
		qgpInitialTemperature = cms.double(1.0), ## initial temperature of QGP; allowed range [0.2,2.0]GeV;

    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
        bbbarSettings = cms.vstring('MSEL = 1'), 
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring(
             'pythiaUESettings',
             'bbbarSettings')
    )
)

#bfilter = cms.EDFilter("PythiaFilter",
#    ParticleID = cms.untracked.int32(5)
#)

## oniafilter = cms.EDFilter("PythiaFilter",
##    Status = cms.untracked.int32(2),
##    MaxEta = cms.untracked.double(1000.0),
##    MinEta = cms.untracked.double(-1000.0),
##    MinPt = cms.untracked.double(0.0),
##    ParticleID = cms.untracked.int32(443)
##)

#oniafilter = cms.EDFilter("MCSingleParticleFilter",
#    Status = cms.untracked.vint32(     2,    2),
#    ParticleID = cms.untracked.vint32(443, 100443),
##    ParticleID = cms.untracked.vint32(443),
#    MinPt = cms.untracked.vdouble(    0.0, 0.0),
#    MaxEta = cms.untracked.vdouble(   1000.0, 1000.0),
#    MinEta = cms.untracked.vdouble(   -1000.0, -1000.0),
#)

#mumugenfilter = cms.EDFilter("MCParticlePairFilter",
#    Status = cms.untracked.vint32(1, 1),
#    MinP = cms.untracked.vdouble(2.5, 2.5),
#    MaxEta = cms.untracked.vdouble(2.5, 2.5),
#    MinEta = cms.untracked.vdouble(-2.5, -2.5),
#    ParticleCharge = cms.untracked.int32(-1),
#    ParticleID1 = cms.untracked.vint32(13),
#    ParticleID2 = cms.untracked.vint32(13)
#)


#ProductionFilterSequence = cms.Sequence(generator*bfilter*oniafilter*mumugenfilter)
ProductionFilterSequence = cms.Sequence(generator)
#ProductionFilterSequence = cms.Sequence(generator*VtxSmeared*bfilter*oniafilter*mumugenfilter)


