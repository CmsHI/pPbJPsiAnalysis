#!/bin/python
import sys,string,re,math,subprocess
from os import listdir
from operator import itemgetter

#Prefix used in Fit2DDataPbPb fitting step
if (len(sys.argv) < 3):
  print "Usage: python %s [Prefix] [Directory of result files 1] [Directory of result files 2] ..." %(sys.argv[0])
  sys.exit()
else:
  prefix=str(sys.argv[1]);

# All fit parameters will be written here
foutput = open("fit_parameters","w")
# Number of signal and background will be written here
ftable = open("fit_table","w")
# Re-format fit_table file for root macro
fcpp = open("fit_cppnumbers","w")
# Print bins with N_Sig error > N_Sig*0.1
ferror = open("fit_errorbins","w")
# ctau error range list
fctauerr = open("fit_ctauErrorRange","w")

# Number of dphibins for each rap, cent, pt bins
#dphibins = 3;
dphibins = 1;
#Parameters without error values in the output files
#nonErrParam=("NLL","nFullBinsResid","RSS","All","Signal")
nonErrParam=("NLL","nFullBinsResid","RSS","All","Signal", "UnNormChi2_mass", "nFitParam_mass", "nFullBinsPull_mass", "Dof_mass", "Chi2_mass", "theChi2Prob_mass", " UnNormChi2_time", "nFitParam_time", "nFullBinsPull_time", "Dof_time", "Chi2_time", "theChi2Prob_time", "UnNormChi2_side", "nFitParam_side", "nFullBinsPull_side", "Dof_side", "Chi2_side", "theChi2Prob_side")
#Parameters with error values but errors will be dropped in the excel file
dropErrParam=("ctauErrMin","ctauErrMax","NoCutEntry","CutEntry")


for f in range(2,len(sys.argv)):
  folder = sys.argv[f]
  print folder
  foutput.write(folder + "\n")
  ftable.write(folder + "\n")
  fctauerr.write(folder + "\n")

  # Reading 1 fitting method, various bins
  filelist = listdir(folder)
  datapar = []
  datatab = []
  datactauerr = []
  duplicateCheck = []
  for files in filelist:  # 1 bin per a line
    data = []
    table = []
    ctauerr = []

    try:
      rap = re.search('rap-?\d{1,3}\.\d{1,3}--?\d{1,3}\.\d{1,3}',files).group()
      cent = re.search('cent\d+\.?\d?-\d+\.?\d?',files).group()
      dphi = re.search('dPhi[-0-9.]+-[-0-9.]{5}',files).group()
      pt = re.search('pT\d+\.\d-\d+\.\d',files).group()
      ntrk = re.search('ntrk\d+\.?\d?-\d+\.?\d?',files).group()
      et = re.search('ET\d+\.?\d?-\d+\.?\d?',files).group()
    except:
      continue;

    # Data file name form
    filename = prefix + "_" + rap + "_" + cent + "_" + pt +"_" + dphi + "_" + ntrk + "_" + et ;
    try:
      finput = open("./"+folder+"/"+filename+".txt",'r')
    except:
      filename = prefix + "_" + rap + "_" + pt + "_" + cent + "_" + dphi + "_" + ntrk + "_" + et ;
      try:
        finput = open("./"+folder+"/"+filename+".txt",'r')
      except:
        print "./"+folder+"/"+filename+".txt doesn't exist"
        continue;
    ### KYO print to check
		#print filename

    rapN = rap.split("rap")[1]
    ptN = pt.split("pT")[1]
    centN = cent.split("cent")[1]
    dphiN = dphi.split("dPhi")[1]
    ntrkN = ntrk.split("ntrk")[1]
    etN = et.split("ET")[1]
    
    data.append(rapN)
    data.append(ptN)
    data.append(centN)
    data.append(dphiN)
    data.append(ntrkN)
    data.append(etN)
    table.append(rapN)
    table.append(ptN)
    table.append(centN)
    table.append(dphiN)
    table.append(ntrkN)
    table.append(etN)
    ctauerr.append(rapN)
    ctauerr.append(ptN)
    ctauerr.append(centN)
    ctauerr.append(dphiN)
    ctauerr.append(ntrkN)
    ctauerr.append(etN)

    header = "rap\tpT\tcent\tdPhi\tntrk\tEt\t"
    headerShort = "rap\tpT\tcent\tdPhi\tntrk\tEt\t"
    headerError = "rap\tpT\tcent\tdPhi\tntrk\tEt\t"
    ### KYO print to check
    #print header

    for line in finput: # Read one .txt file (result file)
      tmp = line.split(" ")
      
      if [check for check in dropErrParam if tmp[0] in check]:  #Drop error on NoCutEntry, CutEntry
        header = header + str(tmp[0]) + "\t"
        data.append(float(tmp[1]))
      
      for i in tmp: # tmp[x] holds 1 column: [0] variable name, [1] value, [2] error
        if i == "ctauErrMin" or i == "ctauErrMax" :
          try:
            ctauerr.append (float(tmp[1]))
          except:
            pass

        if i == "NSig" or i == "NBkg" or i == "PROMPT" or i == "NON-PROMPT" or i == "Bfraction" :
          try:
            table.append (float(tmp[-2]))
            table.append (float(tmp[-1]))
          except:
            pass
          
          #If this parameter doesn't have error values in the output file
          if [check for check in nonErrParam if str(i) in check]:
            headerShort = headerShort + str(i) + "\t"
          else:
            headerShort = headerShort + str(i) + "\t" + str(i) + "Err\t"

        #Skip dropErrParam elements, cause they were written before this loop was started
        if [check for check in dropErrParam if tmp[0] in check]:
          continue

        try:
          data.append(float(i))
        except:
          #If this parameter doesn't have error values in the output file
          if [check for check in nonErrParam if str(i) in check]:
            header = header + str(i) + "\t"
          elif [check for check in dropErrParam if str(i) in check]:
            header = header + str(i) + "\t"
          else:
            header = header + str(i) + "\t" + str(i) + "Err\t"
      # End of (for i in tmp:)
    # End of analyzing 1 line (for line in finput:)

    # Check if this bin is already analyzed or not
    thisEntryHas = rapN+" "+ptN+" "+centN+" "+dphiN+" "+ntrkN+" "+etN
    if [ check for check in duplicateCheck if str(thisEntryHas) in str(check) ]:
      continue
    else:
      # This bin wasn't analyzed yet! Put 1 line for each bin (1 .txt file)
      duplicateCheck.append(rapN+" "+ptN+" "+centN+" "+dphiN+" "+ntrkN+" "+etN)
      datapar.append(data)
      datatab.append(table)
      datactauerr.append(ctauerr)

  # sort ALL directory's results and put them into a file
  if f is 2:
    foutput.write(header+"\n")
  dataparfin = sorted(datapar, key=itemgetter(0,1,2,3,4,5))
  for i in dataparfin:
    line = ""
    for j in i:
      line = line+str(j)+"\t"
    foutput.write(line+"\n")

  # sort only useful results and put them into file
  if f is 2:
    ftable.write(headerShort+"\n")
  datatabfin = sorted(datatab, key=itemgetter(0,1,2,3,4,5))
  for i in datatabfin:
    line = ""
    for j in i:
      line = line+str(j)+"\t"
    ftable.write(line+"\n")

  # put header for a file with inclusive error bins
  ferror.write("rap\tpT\tcent\tdPhi\tntrk\tET\tNSig\tNSigErr\tsigWidth\tsigWidthErr\n");

  for i in dataparfin:
    # Check inclusive yields and if it is too large, write those bins into a error file
    if i[10]*0.15 < i[11] or ((i[34]*0.2 < i[35]) and (i[35] > 0.01)) :
      ferror.write(str(i[0])+"\t"+str(i[1])+"\t"+str(i[2])+"\t"+str(i[3])+"\t"+str(i[4])+"\t"+str(i[5])+"\t")
      ferror.write(str(i[10])+"\t"+str(i[11])+"\t")
      ferror.write(str(i[34])+"\t"+str(i[35])+"\n")

  # Ctau error range will be wrintten
  if f is 2:
    fctauerr.write("rap\tpT\tcent\tdPhi\tntrk\tET\tctauErrMin\tctauErrMax\n")
  datactauerrfin = sorted(datactauerr, key=itemgetter(0,1,2,3,4,5))
  for i in datactauerrfin:
    line = ""
    for j in i:
      line = line+str(j)+"\t"
    fctauerr.write(line+"\n");
    
  # write a file to create a root file with nSig, nPrompt, nNon-prompt 
  
  sigAry = [];
  bkgAry = [];
  prAry = [];
  npAry = [];
  bAry = [];
  sigEAry = [];
  bkgEAry = [];
  prEAry = [];
  npEAry = [];
  bEAry = [];

  for i in datatabfin:
    headerbin = "rap" + i[0] + "_pT" + i[1] + "_cent" + i[2] + "_ntrk" + i[4] + "_ET" + i[5] + "\n";
#    thisbin = "rap" + i[0] + "_cent" + i[2] +  "_pT" + i[1] +"_dPhi" + i[3];
    thisbin = "rap" + i[0] + "_pT" + i[1] + "_cent" + i[2] + "_dPhi" + i[3] + "_ntrk" + i[4] + "_ET" + i[5] ;

    sigAry.append(i[6]);
    sigEAry.append(i[7]);
    bkgAry.append(i[8]);
    bkgEAry.append(i[9]);
    prAry.append(i[10]);
    prEAry.append(i[11]);
    npAry.append(i[12]);
    npEAry.append(i[13]);
    bAry.append(i[14]);
    bEAry.append(i[15]);

    if dphibins > 1:  # Write fit_cppnumbers file if there are dphibins more than 1
      if len(sigAry) is dphibins:
        if dphibins is 3:
          sigL = str(sigAry[0]) + " " + str(sigAry[2]);
          sigEL = str(sigEAry[0]) + " " + str(sigEAry[2]);
          bkgL = str(bkgAry[0]) + " " + str(bkgAry[2]);
          bkgEL = str(bkgEAry[0]) + " " + str(bkgEAry[2]);
          prL = str(prAry[0]) + " " + str(prAry[2]);
          prEL = str(prEAry[0]) + " " + str(prEAry[2]);
          npL = str(npAry[0]) + " " + str(npAry[2]);
          npEL = str(npEAry[0]) + " " + str(npEAry[2]);
          bL = str(bAry[0]) + " " + str(bAry[2]);
          bEL = str(bEAry[0]) + " " + str(bEAry[2]);

        elif dphibins is 5:
          sigL = str(sigAry[0]) + " " + str(sigAry[2]) + " " + str(sigAry[3]) + " " + str(sigAry[4]);
          sigEL = str(sigEAry[0]) + " " + str(sigEAry[2]) + " " + str(sigEAry[3]) + " " + str(sigEAry[4]);
          bkgL = str(bkgAry[0]) + " " + str(bkgAry[2]) + " " + str(bkgAry[3]) + " " + str(bkgAry[4]);
          bkgEL = str(bkgEAry[0]) + " " + str(bkgEAry[2]) + " " + str(bkgEAry[3]) + " " + str(bkgEAry[4]);
          prL = str(prAry[0]) + " " + str(prAry[2]) + " " + str(prAry[3]) + " " + str(prAry[4]);
          prEL = str(prEAry[0]) + " " + str(prEAry[2]) + " " + str(prEAry[3]) + " " + str(prEAry[4]);
          npL = str(npAry[0]) + " " + str(npAry[2]) + " " + str(npAry[3]) + " " + str(npAry[4]);
          npEL = str(npEAry[0]) + " " + str(npEAry[2]) + " " + str(npEAry[3]) + " " + str(npEAry[4]);
          bL = str(bAry[0]) + " " + str(bAry[2]) + " " + str(bAry[3]) + " " + str(bAry[4]);
          bEL = str(bEAry[0]) + " " + str(bEAry[2]) + " " + str(bEAry[3]) + " " + str(bEAry[4]);

        # Write one rap, pt, cent bin into output files
        fcpp.write(headerbin);
        fcpp.write(str(sigAry[1])+"\n");
        fcpp.write(sigL+"\n");
        fcpp.write(sigEL+"\n");
        fcpp.write(str(bkgAry[1])+"\n");
        fcpp.write(bkgL+"\n");
        fcpp.write(bkgEL+"\n");
        fcpp.write(str(prAry[1])+"\n");
        fcpp.write(prL+"\n");
        fcpp.write(prEL+"\n");
        fcpp.write(str(npAry[1])+"\n");
        fcpp.write(npL+"\n");
        fcpp.write(npEL+"\n");
        fcpp.write(str(bAry[1])+"\n");
        fcpp.write(bL+"\n");
        fcpp.write(bEL+"\n");

        del sigAry[:]
        del bkgAry[:]
        del prAry[:]
        del npAry[:]
        del bAry[:]
        del sigEAry[:]
        del bkgEAry[:]
        del prEAry[:]
        del npEAry[:]
        del bEAry[:]

    else:
      fcpp.write("\n");
    # end of fit_cppnumbers writing

foutput.close();
ftable.close();
fcpp.close();
ferror.close();
fctauerr.close();
