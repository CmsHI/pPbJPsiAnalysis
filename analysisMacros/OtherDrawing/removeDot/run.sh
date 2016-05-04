ls *.pdf >> filename.list
python writeDummyScript.py filename.list
#sh dummyScript.sh
chmod 777 dummyScript.sh
echo ' *** run dummyScript.sh *** '
./dummyScript.sh
echo ' *** remove filename.list and dummyScript.sh *** '
rm -f filename.list dummyScript.sh

