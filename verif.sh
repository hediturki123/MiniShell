############################################################
#	Attendu le mini shell ne prend pas en compte les "
############################################################
affiche(){
	echo " ########## Résultat ########## "
	echo "$1"
	echo "_______________________________________"
	echo " ########## Résultat Attendu ##########"
	#echo "$2"
	echo "_______________________________________"
}
make clean
make
rm -f entree.txt sortie.txt
touch entree.txt sortie.txt
cmpV=0
cmpF=0
NbTest=0
for i in $(seq 4)
do
	echo "**********************************"
	echo "*   Test du fichier test$i.txt    *"
	echo "**********************************"
	header=$(./sdriver.pl -t tests/test$i.txt -s ./shell | head -3)
	echo "$header"

	resultat=$(./sdriver.pl -t tests/test$i.txt -s ./shell | head -n -1 | sed "1,3d" | tr -s " ")
	res_attendu=$(cat tests/testres$i.txt | tr -s " ")

	#affiche $resultat $res_attendu

	if [[ $resultat != $res_attendu ]]
	then
		echo "   --------- Test faux ---------"
		cmpF=$((cmpF+1))
	else
		echo "   --------- Test OK ---------"
		cmpV=$((cmpV+1))
	fi
	NbTest=$((NbTest+1))

done
# Test <
echo "**********************************************"
echo "*   Test du fichier de redirection entree    *"
echo "**********************************************"
header=$(./sdriver.pl -t tests/testentree.txt -s ./shell | head -3)
echo "$header"

echo $(./sdriver.pl -t tests/testentree.txt -s ./shell | head -n -1 | sed "1,3d") > entree.txt
echo $(cat Makefile) > tests/testresentree.txt

test=$(diff tests/testresentree.txt entree.txt)

if [[ -n $test ]]
then
	echo "   --------- Test faux ---------"
	cmpF=$((cmpF+1))
else
	echo "   --------- Test OK ---------"
	cmpV=$((cmpV+1))
fi
NbTest=$((NbTest+1))
# Test >
echo "**********************************************"
echo "*   Test du fichier de redirection sortie    *"
echo "**********************************************"
header=$(./sdriver.pl -t tests/testsortie.txt -s ./shell | head -3)
echo "$header"

echo $(./sdriver.pl -t tests/testsortie.txt -s ./shell | head -n -1 | sed "1,3d")
echo $(cat Makefile) > tests/testresentree.txt

test=$(diff sortie.txt tests/testressortie.txt)

if [[ -n $test ]]
then
	echo "   --------- Test faux ---------"
	cmpF=$((cmpF+1))
else
	echo "   --------- Test OK ---------"
	cmpV=$((cmpV+1))
fi
NbTest=$((NbTest+1))
echo
echo "Nombre total de Test : $NbTest Test Réussi : $cmpV    Test Echoué : $cmpF"
