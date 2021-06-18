import os
import string 
import subprocess
import sys


# sys.stdout = open('log.txt', 'w') //changes stdout to logfile 

os.chdir('..') #changes process' directory to parent directory

testFile = sys.argv[1] #takes in the first argument as the testFile

#runs the exe with the given test file and saves the output in output.txt
subprocess.run('languagEd.exe testCases/' + testFile + ' > testCases/output.txt', shell = True) #runs the exe in the parent directory


#reads the output of the exe and stores it in outputLines var
file = open('testCases/output.txt', 'r')
outputLines = file.readlines()
file.close();

#takes the 2nd arg as the answer file, reads it, and stores the lines in ansLines
ansFile = sys.argv[2]
file = open('testCases/' + ansFile, 'r')
ansLines = file.readlines()
file.close()

#sets the list that has the most lines equal to 'line' var and the one with least lines to 'otherLines' var
minLines = min(len(outputLines), len(ansLines))
maxLines = max(len(outputLines), len(ansLines))
lines = []
otherLines = []
if maxLines == len(outputLines):
    lines = outputLines
    otherLines = ansLines
else:
    lines = ansLines
    otherLines = outputLines

match = True

#if the number of lines do not match 
if len(ansLines) != len(outputLines):
    match = False
    print('ERROR NUMBER OF LINES DO NOT MATCH')
    print('\tYour output has ' + str(len(outputLines)))
    print('\tThe answer has ' + str(len(ansLines)))
    print('\tThese extra lines are: ')
    


    for i in range(minLines, maxLines):
        print('\t' + lines[i], end = '')

print()

#checks to see if the lines that are present match or not 
for index, filelines in enumerate(otherLines):
    if filelines != lines[index]:
        match = False
        print('ERROR MISMATCH')
        print('\tYour line: ' + outputLines[index], end = '')
        print('\tAnswer line: ' + ansLines[index], end = '')
    
print()

if match:
    print('PASSED LINE TEST')

sys.stdout.close()



