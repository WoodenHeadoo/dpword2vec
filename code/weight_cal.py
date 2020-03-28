import re
import math

# Inputs:
# vocabulary of words and design patterns:
file_pats_vocab = './data/pats_vocab.txt'
# co-occurrence pairs and counts (for GloVe):
file_pair_occs = './data/words_pats_pairs.txt'

# Outputs:
# weights for dp-word pairs (for GloVe):
file_weights = './data/words_weight.txt'

pat_count,word_count = 0,0
fr = open(file_pats_vocab,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    if re.match(r'.+designpattern$',row):
        pat_count += 1
    else:
        word_count += 1
    row = fr.readline()
fr.close()
token_count = pat_count+word_count

weight = [0]*token_count
fr = open(file_pair_occs,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    temp = row.split(' ')
    i = int(temp[0])
    j = int(temp[1])
    if i < pat_count and j >= pat_count:
        weight[i] += 1
        weight[j] += 1
    row = fr.readline()
fr.close()

count = 0
mp,mw = word_count,pat_count
for i in range(token_count):
    if weight[i] != 0:
        if i < pat_count:
            weight[i] = math.log(word_count/float(weight[i]))
            if weight[i] < mp and weight[i] != 0.0:
                mp = weight[i]
        else:
            if weight[i] != 0:
                weight[i] = math.log(pat_count/float(weight[i]))
                count += 1
                if weight[i] < mw and weight[i] != 0.0:
                    mw = weight[i]
sp = sum(weight[0:pat_count])
sw = sum(weight[pat_count:token_count])

fw = open(file_weights,'w')
fw.write(str(pat_count)+'\n')
for i in range(token_count):
    if i < pat_count:
        if weight[i] == 0:
            fw.write(str(mp/sp*pat_count)+'\n')
        else:
            fw.write(str(weight[i]/sp*pat_count)+'\n')
    else:
        if weight[i] == 0:
            fw.write(str(mw/sw*count)+'\n')
        else:
            fw.write(str(weight[i]/sw*count)+'\n')
fw.close()
