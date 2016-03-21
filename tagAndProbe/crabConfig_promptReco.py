from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.requestName = 'promptReco-v27'
config.section_('JobType')
config.JobType.psetName = 'jpsi_tnp_dataPromptReco_cfg_v27.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['tnpJPsi_dataPromtReco.root']
config.JobType.pyCfgParams = ['noprint']
config.section_('Data')
config.Data.inputDataset = '/PAMuon/HIRun2013-PromptReco-v1/RECO'
config.Data.publishDBS = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet'
config.Data.publication = False
config.Data.unitsPerJob = 20
config.Data.splitting = 'LumiBased'
config.Data.lumiMask = 'Cert_210498-211631_HI_PromptReco_Collisions13_JSON_MuonPhys_v2.txt'
#config.Data.publishDataName = 'Onia2MuMu_PsiMuMu_pA_noPileUpRej-v4'
config.Data.runRange = '210676-211631'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_KR_KNU'
