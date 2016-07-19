
#!/bin/bash

ana=$1
#put you the samples  you want to run
samples="data_DoubleMu2015_chunk0 data_DoubleMu2015_chunk1 data_DoubleMu2015_chunk2 
data_DoubleMu2015_chunk3 data_DoubleMu2015_chunk4 data_DoubleMu2015_chunk5 
data_DoubleMu2015_chunk6 data_DoubleMu2015_chunk7 data_DoubleMu2015_chunk8 
data_DoubleMu2015_chunk9 data_DoubleMu2015_chunk10 
MC_DYMG_chunk0 MC_DYMG_chunk1 MC_DYMG_chunk2 MC_DYMG_chunk3 
MC_DYMG_chunk4 MC_DYMG_chunk5 MC_DYMG_chunk6 MC_DYMG_chunk7 
MC_DYMG_chunk8 MC_DYMG_chunk9 MC_DYMG_chunk10"
#MC_DY_chunk0 MC_DY_chunk1 MC_DY_chunk2 MC_DY_chunk3 
#MC_DY_chunk4 MC_DY_chunk5 MC_DY_chunk6 MC_DY_chunk7 
#MC_DY_chunk8 MC_DY_chunk9

#samples="MC_WJets"
dir=$PWD

#mkdir -p $ana
#+'_PhaseII_C3'
#mkdir -p $ana+'_PhaseII_C4v2'

if [ ! -d logs ]; then
    mkdir logs
fi

for i in $samples
do
	cat setupNAF.sh | awk -v i=$i -v d=$dir -v a=$ana '{gsub(/XXXX/, a ); gsub(/YYYY/, i ); gsub(/ZZZZ/, d )}; 1'  >| reader_${ana}_$i
	sed -i "s|JBNAME|Ana$i|" reader_${ana}_$i

	qsub -o logs reader_${ana}_$i
#	rm reader_${ana}_$i
done
