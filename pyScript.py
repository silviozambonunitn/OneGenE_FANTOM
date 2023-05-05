#This is an alternative version of the c++ Script to test the efficiency and learn python
import csv
import glob
filenames = glob.glob("./*.expansion")
outputCsv = open("matrixPY.csv","wt")
csvWriter = csv.writer(outputCsv, delimiter=";", lineterminator="\n")
csvWriter.writerow(["Seed","Leaf","RelativeFrequency"])
for f in filenames:
    with open(f,"rt") as isoformFile:
        isoformReader = csv.reader(isoformFile, delimiter=",", lineterminator="\n", skipinitialspace=True)
        iso1=next(isoformReader)[0].split()[3].split("-")[0]
        next(isoformReader)
        for row in isoformReader:
            if len(row) == 5: #To check last row
                csvWriter.writerow([iso1, row[1].capitalize(), row[3]])