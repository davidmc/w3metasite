# /bin/sh
for i in *Form.cpp
do cat $i | sed s/"formButtons()"/"formButtons(args)"/g > New/$i
done
for i in *Form.cpp
do
  echo $i
  diff $i New/$i
done