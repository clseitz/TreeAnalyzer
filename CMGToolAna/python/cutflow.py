#!/afs/desy.de/products/python/.amd64_rhel50/2.6/bin/python
from math import sqrt
from sys import exit

# sample names according to: base/base_tag_sample.txt
#samples = ['TTbar_DiLep','TTbar_SinLep','T1tttt_1500_100','T1tttt_1200_800']
#samples = ['TTbar_DiLep','TTbar_SinLep','WJets','T1tttt_800_450','T1tttt_1500_100','T1tttt_1200_800']
samples = ['TTbar_DiLep','TTbar_SinLep','WJets','T1tttt_800_450','T1tttt_1300_100']

# we print a table without the last n lines
# with systematic error sys1 and sys2
# base is used for path/ and fileName as base/base_
# tag is NoPU,50PU,140PU
def Cutflow(path,base,tag,n,pick=[]):
	first=base+'_'+tag+'_'
	linesList=[]
	for s in samples:
		file = open(path+'/'+first+s+'.txt')
		#file = open(path+first+s+'.txt')
		linesList.append(file.readlines())
		file.close
	N=len(linesList[0])-n
	print base
#	print ('%12s '+6*'%11s ') % (tag,'dibos','ttbar','B+jets','single t','sum bgrds','STC')
	print ('%12s '+6*'%11s ') % (tag,'ttbar(ll)','ttbar(1l)','WJet','BKG','T1tttt(800,450)','T1tttt(1300,100)')
        print '\\\ \hline'
	for i in range(2,N):
		if len(pick)>0 and not i in pick: continue
		d=[] # data
		s=[] # data
		for k in range(len(linesList)):
                        val = float(linesList[k][i].split('\t')[2])
                        ival = float(linesList[k][i].split('\t')[4])
                        d.append(val)
                        s.append(ival)
		what=linesList[0][i].split('\t')[0][6:]
#		d=map(lambda x: x/10.,d)
		nbgrd=d[0]+d[1]+d[2]
                
		nbgrd_SYS=sqrt(pow(s[0],2)+pow(s[1],2)+pow(s[2],2))
                #print ('%2i: %12s  %0.2f') % (i,what,"&",d[0],"+-",s[0],d[1],s[1],d[2],s[2],nbgrd,nbgrd_SYS,d[3],s[3],d[4],s[4])
		print what, "&", '%.2f' % d[0] , "+-", '%.2f' % s[0], "&",'%.2f' % d[1],"+-",'%.2f' % s[1],"&",'%.2f' % d[2],"+-",'%.2f' % s[2],"&",'%.2f' % nbgrd,"+-",'%.2f' % nbgrd_SYS,"&",'%.2f' % d[3],"+-",'%.2f' % s[3],"&",'%.2f' % d[4],"+-",'%.2f' % s[4]
#		print ("%f +-% f") % (d[0], s[0],d[1],s[1]) 
                print '\\\ \hline'
############################################################################################################
#print '-----------------------------------------------------------------------------'
print '  \\begin{table}[htdp]'
print '  \caption{bla}'
print '  \\begin{center}'
print '  \\begin{tabular}{|l|c|c|c|c|c|}  \hline'


Cutflow('Tables', 'CMG_cutflow','MC',0)
#Cutflow('Tables/BKG', 'CMG_cutflow','MC',0)
#Cutflow('Tables/BKG/4Jet', 'CMG_cutflow','MC',0)

print '  \end{tabular}'
print '  \end{center}'
print '  \end{table}'
#print '-----------------------------------------------------------------------------'
