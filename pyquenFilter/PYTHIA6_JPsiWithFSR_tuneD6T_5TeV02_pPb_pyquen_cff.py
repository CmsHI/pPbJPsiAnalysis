import FWCore.ParameterSet.Config as cms

configurationMetadata = cms.untracked.PSet(
	version = cms.untracked.string('$Revision: 1.1 $'),
	name = cms.untracked.string('$Source: /local/reps/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/PYTHIA6_JPsiWithFSR_tuneD6T_8TeV_cff.py,v $'),
	annotation = cms.untracked.string('Winter13: Pythia6 generation of prompt JPsi, 5.023TeV, D6T tune')
)

from Configuration.Generator.PythiaUESettings_cfi import *

process.VtxSmeared.Beta = cms.double(0.434)   # pPb

#generator = cms.EDFilter("Pythia6GeneratorFilter",
process.generator = cms.EDFilter("PyquenGeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(5023.0),
    # PYTHIA xSec: 15.12 mb
    crossSection = cms.untracked.double(13775390),
    filterEfficiency = cms.untracked.double(0.0463),
    maxEventsToPrint = cms.untracked.int32(0),
    ExternalDecays = cms.PSet(
        EvtGen = cms.untracked.PSet(
            operates_on_particles = cms.vint32( 0 ), # 0 (zero) means default list (hardcoded)
                                                     # you can put here the list of particles (PDG IDs)
                                                     # that you want decayed by EvtGen
            use_default_decay = cms.untracked.bool(False),
            decay_table = cms.FileInPath('GeneratorInterface/ExternalDecays/data/DECAY_NOLONGLIFE.DEC'),
            particle_property_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/evt.pdl'),
            user_decay_file = cms.FileInPath('GeneratorInterface/ExternalDecays/data/Onia_mumu.dec'),
            list_forced_decays = cms.vstring('MyJ/psi'),
        ),
        parameterSets = cms.vstring('EvtGen')
    ),                             

		filterType = cms.untracked.string('HadronDecayGenEvtSelector'),
		hadrons = cms.vint32(443),
		hadronStatus = cms.vint32(2),
		hadronEtaMin = cms.vdouble(-1e+100),
		hadronEtaMax = cms.vdouble(1e+100),
		hadronPtMin = cms.vdouble(0.0),
		hadronPtMax = cms.vdouble(50.0),
		hadronPMin = cms.vdouble(0.0),
		decays = cms.int32(13),
		decayStatus = cms.int32(1),
		decayPtMin = cms.double(0.0),
		decayPtMax = cms.double(999),
		decayEtaMin = cms.double(-2.97),
		decayEtaMax = cms.double(2.03),
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
        processParameters = cms.vstring('MSEL=61          ! Quarkonia', 
            'MDME(858,1) = 0  ! 0.060200    e-    e+', 
            'MDME(859,1) = 1  ! 0.060100    mu-  mu+', 
            'MDME(860,1) = 0  ! 0.879700    rndmflav        rndmflavbar', 
            'MSTP(142)=2      ! turns on the PYEVWT Pt re-weighting routine', 
            'PARJ(13)=0.750   ! probability that a c or b meson has S=1', 
            'PARJ(14)=0.162   ! probability that a meson with S=0 is produced with L=1, J=1', 
            'PARJ(15)=0.018   ! probability that a meson with S=1 is produced with L=1, J=0', 
            'PARJ(16)=0.054   ! probability that a meson with S=1 is produced with L=1, J=1', 
            'MSTP(145)=0      ! choice of polarization', 
            'MSTP(146)=0      ! choice of polarization frame ONLY when mstp(145)=1', 
            'MSTP(147)=0      ! particular helicity or density matrix component when mstp(145)=1', 
            'MSTP(148)=1      ! possibility to allow for final-state shower evolution, extreme case !', 
            'MSTP(149)=1      ! if mstp(148)=1, it determines the kinematics of the QQ~3S1(8)->QQ~3S1(8)+g branching', 
            'PARP(141)=1.16   ! New values for COM matrix elements', 
            'PARP(142)=0.0119 ! New values for COM matrix elements', 
            'PARP(143)=0.01   ! New values for COM matrix elements', 
            'PARP(144)=0.01   ! New values for COM matrix elements', 
            'PARP(145)=0.05   ! New values for COM matrix elements', 
            'PARP(146)=9.28   ! New values for COM matrix elements', 
            'PARP(147)=0.15   ! New values for COM matrix elements', 
            'PARP(148)=0.02   ! New values for COM matrix elements', 
            'PARP(149)=0.02   ! New values for COM matrix elements', 
            'PARP(150)=0.085  ! New values for COM matrix elements', 
            'BRAT(861)=0.202  ! chi_2c->J/psi gamma', 
            'BRAT(862)=0.798  ! chi_2c->rndmflav rndmflavbar', 
            'BRAT(1501)=0.013 ! chi_0c->J/psi gamma', 
            'BRAT(1502)=0.987 ! chi_0c->rndmflav rndmflavbar', 
            'BRAT(1555)=0.356 ! chi_1c->J/psi gamma', 
            'BRAT(1556)=0.644 ! chi_1c->rndmflav rndmflavbar'),
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters', 
            'CSAParameters'),
        CSAParameters = cms.vstring('CSAMODE = 6     ! cross-section reweighted quarkonia')
    )
)

#oniafilter = cms.EDFilter("PythiaFilter",
#    Status = cms.untracked.int32(2),
#    MaxEta = cms.untracked.double(1e100),
#    MinEta = cms.untracked.double(-1e100),
#    MinPt = cms.untracked.double(0.0),
#    ParticleID = cms.untracked.int32(443)
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

#ProductionFilterSequence = cms.Sequence(generator*oniafilter*mumugenfilter)
ProductionFilterSequence = cms.Sequence(generator)

