import os
import numpy as np
import pandas as pd

index = os.getenv('INDEX')

# python indexing is 0-based!
ind = int(index) - 1

b=3.0

mylist=pd.read_csv('Spill.txt', sep='\t').values.tolist()

mylist[ind][1]=mylist[ind][1]+b

fi=open('Spill_Pythonnew_'+index+'.txt','w')

for listitem in mylist:
        fi.write('%f  %f\n' % (listitem[0],listitem[1] ))

fi.close()

