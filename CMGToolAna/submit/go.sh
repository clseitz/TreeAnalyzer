
#!/bin/bash

ana=$1
#put you the samples  you want to run
#samples="MC_T1tttt_1500_100 MC_T1tttt_1200_800 MC_TTbar MC_WJets"
samples="MC_WJets"
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
