# DLX File Encoder
This is a project for CSCI3330 - Computer Architecture
## The purpose
This is a file encoder for [DLX Assember](https://en.wikipedia.org/wiki/DLX) essentially it takes a text file with instruction and gives the hexadecimal output for that file.

### Usage
To use the project compile using:
```
g++ *.cpp -o Main
```

Then to properly use the program
```
./Main <inputfile> <outputfile>
```

### Features
In order to parse the file properly, I used Regular Expression. It allowed me to efficiently get matches of the file without having to do redudant matches of regular c++ string parsing. I learned how to use Regex by using this [Website](https://regexr.com/).
