./sclp $1
spim -f $1.s
rm $1.s
rm gmon.out
