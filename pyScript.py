#This is an alternative version of the C++ Script to test the efficiency and correctness of the other
import csv
import glob

filenames = glob.glob("./*.expansion") #getting all the isoforms filenames

#Opening the output file
outputCsv = open("FANTOM_RelativeFrequencyMatrix_PY.csv","wt")
csvWriter = csv.writer(outputCsv, delimiter=";", lineterminator="\n")
csvWriter.writerow(["Seed","Leaf","RelativeFrequency"])

#Processing the files
for f in filenames:
    with open(f,"rt") as isoformFile:
        isoformReader = csv.reader(isoformFile, delimiter=",", lineterminator="\n", skipinitialspace=True) #Opening the files
        iso1=next(isoformReader)[0].split()[3].split("-")[0] #Getting the seed name
        next(isoformReader) #Skipping the header
        for row in isoformReader:
            if len(row) == 5: #To check last row
                csvWriter.writerow([iso1, row[1].capitalize(), row[3]])
        isoformFile.close()