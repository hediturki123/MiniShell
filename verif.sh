############################################################
#	Attendu le mini shell ne prend pas en compte les "
############################################################

make clean
make
for i in $(seq 1)
do
	echo "**************************"
	echo "Test du fichier test$i.txt"
	resultat=$(./sdriver.pl -t tests/test$i.txt -s ./shell | head -n -1 | sed "1,3d" | tr -s " ")
	res_attendu=$(cat tests/testres$i.txt | tr -s " ")

	if [[ $resultat != $res_attendu ]]
	then
		echo "Test faux"
	else
		echo "Test OK"
	fi

done
