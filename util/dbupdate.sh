#!/bin/sh

R_USER=user
R_HOST=127.0.0.1

cd $HOME/dbupdate

CHANGED=0
D=./tmp
OUT=output.sql


for i in switches.txt clients.txt
do

 touch $D/$i

 ssh ${R_USER}@${R_HOST} "cat radsecproxy/$i" > $D/$i.new

 if [ -s $D/$i.new ]
 then

	if ! cmp -s $D/$i.new $D/$i
	then
	  cp $D/$i.new $D/$i
          CHANGED=1
	fi
 fi

done

if [ "$CHANGED" = 1 ]
then

echo "begin transaction;" > $OUT
echo "delete from called_station;" >> $OUT


cat $D/switches.txt | awk '{ gsub("#.*$", "") ;  print } ' | awk '

         NF==2 {
                 printf "insert into called_station values ('"'"'%s'"'"', '"'"'%s'"'"'); \n" , $1 , $2

 }' >> $OUT

echo "delete from calling_station;" >> $OUT
cat $D/clients.txt | awk '{ gsub("#.*$", "") ;  print } ' | awk '

         NF==1 {
                 printf "insert into calling_station values ('"'"'%s'"'"', '"'"'auth'"'"'); \n" , $1 

 }' >> $OUT

echo "commit;" >> $OUT

sqlite3 /var/lib/radsecproxy/data.db < $OUT

fi
