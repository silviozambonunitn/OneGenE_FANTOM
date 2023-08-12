import csv

#output file
outputCsv = open("FANTOM_RelativeFrequencyMatrix_names.csv","wt")
csvWriter = csv.writer(outputCsv, delimiter=";", lineterminator="\n")
csvWriter.writerow(['Seed;Leaf;RelativeFrequency;PearsonCorrelation;relfreq-|pearson|'])

#dictionary
with open('tcode-gene.csv', mode='r') as dictFile:
    readerDict = csv.reader(dictFile, delimiter='|')
    mydict = {rows[0]:rows[2] for rows in readerDict}
    with open('FANTOM_unified.csv', mode='r') as matrixFile:
        readerMatrix = csv.reader(matrixFile, delimiter=';')
        next(readerMatrix, None) #Skipping header? Must check
        for r in readerMatrix:
            csvWriter.writerow([mydict[r[0]], mydict[r[1]], r[2], r[3], r[4]])
