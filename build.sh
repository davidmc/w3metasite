# build.sh - build all source listed

for i in `cat sources`
do
cd ~/development
echo "----------------------- building : " $i " -----------------------"
cd $i
make 
done


