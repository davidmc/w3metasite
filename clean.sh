# clean.sh - prepares all source for upload

for i in  `cat sources`
do
cd ~/development
cd $i
echo $i
make clean
done
cd ~/development




