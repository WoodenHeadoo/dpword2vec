import re
import random
import math

# Inputs:
# threshold for subsampling of frequent words:
sample_thres = 5e-3
# size of word context windows:
win_size = 10
# ratios of the description corpus and the crowdsourced corpus:
ratio_des,ratio_qa = 1,0
# file path of the description corpus word tokens (one document per line, the word tokens are delimited by " "):
file_des_words = './raw data/dpdes_words.txt'
# file path of the description corpus pattern tokens (one document per line, the pattern tokens have suffix "designpattern"):
file_des_pats = './raw data/dpdes_pats.txt'
# file path of the crowdsourced corpus word tokens (one document per line, the word tokens are delimited by " "):
file_qa_words = './raw data/q_a_words.txt'
# file path of the crowdsourced corpus pattern tokens (one document per line, the pattern tokens have suffix "designpattern", delimited by "<"):
file_qa_pats = './raw data/q_a_pats.txt'

# Outputs:
# vocabulary of words and design patterns:
file_pats_vocab = './data/pats_vocab.txt'
# co-occurrence pairs and counts (for GloVe):
file_pair_occs = './data/words_pats_pairs.txt'

vocab = {}
tokensall = 0
fr = open(file_qa_words,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    tokens = row.split()
    for token in tokens:
        if vocab.has_key(token):
            vocab[token] += 1
        else:
            vocab[token] = 1
        tokensall += 1
    row = fr.readline()
fr.close()
fr = open(file_des_words,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    tokens = row.split()
    for token in tokens:
        if vocab.has_key(token):
            vocab[token] += 1
        else:
            vocab[token] = 1
        tokensall += 1
    row = fr.readline()
fr.close()

pats = {}
fr = open(file_qa_pats,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    tokens = row.split('<')
    for token in tokens:
        if token in pats:
            pats[token] += 1
        else:
            pats[token] = 1
    row = fr.readline()
fr.close()
fr = open(file_des_pats,'r')
row = fr.readline()
while row:
    row = row.strip('\n')
    if row in pats:
        pats[row] += 1
    else:
        pats[row] = 1
    row = fr.readline()
fr.close()

temp = vocab.keys()
for word in temp:
    if (vocab[word] < 5):
        tokensall -= vocab[word]
        del vocab[word]

GV = {}
count = 0
fw = open(file_pats_vocab,'w')
for pat in pats:
    fw.write(pat+'\n')
    GV[pat] = count
    count += 1
print 'pat_count: ' + str(count)
pat_count = count
for word in vocab:
    fw.write(word+'\n')
    GV[word] = count
    count += 1
print 'vocab_size: ' + str(count)
fw.close()

tokensact = 0
count = 0
Tokens = []
Pats = []
fr1 = open(file_des_words,'r')
fr2 = open(file_des_pats,'r')
row1 = fr1.readline()
row2 = fr2.readline()
while row1:
    row1 = row1.strip('\n')
    tokens = row1.split()
    doc = []
    for token in tokens:
        if token in GV:
            prob = math.sqrt(sample_thres*tokensall/float(vocab[token]))
            if prob > random.random():
                doc.append(GV[token])
                tokensact += 1
    if len(doc) > 0:
        Tokens.append(doc)
        row2 = row2.strip('\n')
        tokens = row2.split('<')
        doc = []
        for token in tokens:
            doc.append(GV[token])
        Pats.append(doc)
        count += 1
    row1 = fr1.readline()
    row2 = fr2.readline()
fr1.close()
fr2.close()
if row2:
    print 'Error Des'
print 'des_count: ' + str(count)
des_count = count
fr1 = open(file_qa_words,'r')
fr2 = open(file_qa_pats,'r')
row1 = fr1.readline()
row2 = fr2.readline()
while row1:
    row1 = row1.strip('\n')
    tokens = row1.split()
    doc = []
    for token in tokens:
        if token in GV:
            prob = math.sqrt(sample_thres*tokensall/float(vocab[token]))
            if prob > random.random():
                doc.append(GV[token])
                tokensact += 1
    if len(doc) > 10:
        Tokens.append(doc)
        row2 = row2.strip('\n')
        tokens = row2.split('<')
        doc = []
        for token in tokens:
            doc.append(GV[token])
        Pats.append(doc)
        count += 1
    row1 = fr1.readline()
    row2 = fr2.readline()
fr1.close()
fr2.close()
if row2:
    print 'Error Q&A'
print 'doc_count: ' + str(count)
print 'tokens_ratio: ' + str(tokensact/float(tokensall))

Pairs = {}
for d in range(len(Tokens)):
    lt = len(Tokens[d])
    lp = len(Pats[d])
    alpha = ratio_des if d < des_count else ratio_qa
    if alpha > 0:
        for i in range(lt):
            l = i-win_size if i > win_size else 0
            r = i+win_size+1 if i+win_size+1 < lt else lt
            for j in range(l,r):
                if not i == j:
                    little = Tokens[d][i] if Tokens[d][i]<Tokens[d][j] else Tokens[d][j]
                    big = Tokens[d][i] if Tokens[d][i]>Tokens[d][j] else Tokens[d][j]
                    idx = str(little)+' '+str(big)
                    if Pairs.has_key(idx):
                        Pairs[idx] += 0.5*alpha
                    else:
                        Pairs[idx] = 0.5*alpha
            for j in range(lp):
                idx = str(Pats[d][j])+' '+str(Tokens[d][i])
                if Pairs.has_key(idx):
                    Pairs[idx] += alpha
                else:
                    Pairs[idx] = alpha
        for i in range(lp):
            for j in range(i):
                little = Pats[d][i] if Pats[d][i]<Pats[d][j] else Pats[d][j]
                big = Pats[d][i] if Pats[d][i]>Pats[d][j] else Pats[d][j]
                idx = str(little)+' '+str(big)
                if Pairs.has_key(idx):
                    Pairs[idx] += alpha
                else:
                    Pairs[idx] = alpha

fw = open(file_pair_occs,'w')
for pair in Pairs:
    fw.write(pair+' ')
    fw.write(str(int(Pairs[pair]))+'\n')
fw.close()
