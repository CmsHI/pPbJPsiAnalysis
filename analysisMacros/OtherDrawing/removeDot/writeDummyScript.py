from sys import argv

script, lFileName = argv
ssFileName = "dummyScript.sh"

print "reading filelist %s" % lFileName

ssFile = open(ssFileName,'w')

lFile = open(lFileName)
while True:
  line = lFile.readline()
  if not line: break

  line=line.replace('\n',' ')

  ssFile.write('mv ')
  ssFile.write(line)

  line=line.replace('.','')
  line=line.replace('pdf' , '.pdf')
  line=line.replace('root', '.root')
  line=line.replace('csh' , '.csh')
  line=line.replace('sh' , '.sh')
  line=line.replace('log' , '.log')
  line=line.replace('txt' , '.txt')

  ssFile.write(line)
  ssFile.write('\n')
  
lFile.close()
ssFile.close()

print "%s is written!" % ssFileName
