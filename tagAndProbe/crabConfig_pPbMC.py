from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'pPbMC-v28'
config.section_('JobType')
config.JobType.psetName = 'jpsi_tnp_mc_cfg_v28.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['tnpJPsi_MC.root']
config.JobType.pyCfgParams = ['noprint']
config.section_('Data')
#Official sample
#config.Data.inputDataset = '/JPsiWithFSR_tuneD6T_5TeV02/pAWinter13DR53X-pa_2nd_run_STARTHI53_V27-v1/GEN-SIM-RECO'
#private sample
config.Data.inputDataset = '/PromptJpsi_PYTHIAboosted_2nd_GENSIM_STARTHI53_V27/kyolee-PromptJpsi_PYTHIAboosted_2nd_RECO_STARTHI53_V27_v2-21b49fbef3a467e97fcda7a60a51a4ff/USER'
config.Data.publishDBS = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet'
config.Data.publication = False
config.Data.unitsPerJob = 10
config.Data.inputDBS = 'phys03'
#config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'
#config.Data.publishDataName = 'Onia2MuMu_PsiMuMu_pA_noPileUpRej-v4'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_KR_KNU'
