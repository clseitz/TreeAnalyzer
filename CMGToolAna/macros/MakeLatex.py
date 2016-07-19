#!/usr/bin/python


plots = ['hDRweighted.pdf','haxisMajor.pdf','haxisMinor.pdf','hbeta.pdf','hfRing0.pdf','hfRing1.pdf','hfRing2.pdf','hfRing3.pdf','hjetR.pdf','hjetRchg.pdf','hjteta.pdf','hjtpt.pdf','hmLL.pdf','hnCh.pdf','hnTot.pdf','hptD.pdf','hpull.pdf']

for plot in plots:
    print '\\begin{frame}{'+plot.replace('.pdf','')+'}'
    print '\\vskip-5pt'
    print '\\begin{figure}[h]'
    print '\\begin{flushleft}'
    print '\\includegraphics[width=0.95\\linewidth]{DataMCComp/plots_PFCHS/'+plot+'}'
    print '\\end{flushleft}'
    print '\\end{figure}'	
    print '\\end{frame}'
    print
