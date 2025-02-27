'''
EECS 348 Assignment 3
Program: Input a regex, output all matches
Input: A user-inputted regex, a test file
Output: A list of all matches
Collaborators: Myself
Other sources of the code: the previous assignment, the RE library
Full Name: Daul Paul Lee
Creation Date: 2/26/2025, 10:00 PM
'''

import re #the library we need for this to work

def main(): #the main program
    linelist = [] #storing all the lines from the file into an array
    try: #attempt to read the file based on the name
        filename = input("Enter the name of the input file: ") #grabs the file name
        with open(filename) as file: #open the file
            for line in file.readlines(): #for each line in the document
                try: #if we can, append the string to a list 
                    linelist.append(line) #append to the list
                except: #if we can't
                    continue #skip past this line of logic
        stringpattern = input("Enter expression here, or type exit to leave:") #enter in the regex or get out of the thing
        while stringpattern != "exit": #while the input is not a signal to exit
            try: # if the regex is valid
                strippedpattern = stringpattern.replace("/", "") #in case the person includes the forward slashes appending the string, we can remove it
                pattern = re.compile(strippedpattern) #compile the pattern for use in the findall
                for i in range(len(linelist)): #for all of the lines in the file
                    search = re.findall(pattern, linelist[i]) #search through them for all matches
                    if len(search) != 0: #if the find all list is not empty
                        print(i+1, search) #print the line number and the matches
                stringpattern = input("Enter expression here, or type exit to leave:") #loop back
            except: #if the regex is invalid
                print("Invalid regex") #if it doesn't work, 
                stringpattern = input("Enter expression here, or type exit to leave:") #loop back
    except: #if we can't read the file
        print("File is invalid") #the file doesn't work
        filename = input("Enter the name of the input file: ") #grabs the file name again and give them a chance to try again

main() #run the program