with open('grammar.txt','r') as f:
    unique = []
    for line in f.readlines():
        for word in (line.strip().split(' ')):
            if word != '->':
                if word not in unique:
                    print(word, end = ',')
                    unique.append(word)
count = 0
count_ = 0
for word in unique:
    if word.islower():
        count+=1
    if word.isupper():
        count_+=1
count
count_
for word in unique:
    if word.islower():
        print(word,end = ',')

for word in unique:
    if word.isupper():
        print(word,end = ',')
