
#!/bin/bash

ana=$1

echo "checking library - compiling if neccesssary (no error check)"
root -q -b reader${ana}.C+ > /dev/null 2>&1
samples="MC_TTbar MC_T1tttt_1500_100 MC_T1tttt_1200_100"
dir=$PWD

#mkdir -p $ana
#+'_PhaseII_C3'
#mkdir -p $ana+'_PhaseII_C4v2'

for i in $samples
do
	cat setupNAF.sh | awk -v i=$i -v d=$dir -v a=$ana '{gsub(/XXXX/, a ); gsub(/YYYY/, i ); gsub(/ZZZZ/, d )}; 1'  >| reader_${ana}_$i	
	qsub -l site=hh reader_${ana}_$i
	rm reader_${ana}_$i
done
