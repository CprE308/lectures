# CprE 308 Spring 2014 Lectures
This repo contains all lecture slides for CprE 308 - Spring 2014.

## Lecture Listings

| Week | Topic | Slides |
|:----:|:-----:|:------:|
| 1 | Introduction | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture1/lecture1.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture1/lecture1.pdf?raw=true) |
|   | Hardware Review / Process Intro | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture2/lecture2.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture2/lecture2.pdf?raw=true) |
| 2 | Process Memory Basics | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture5/lecture5.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture5/lecture5.pdf?raw=true) |
|   | Process Creation | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture6/lecture6.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture6/lecture6.pdf?raw=true) |
| 3 | File System Interface | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture7/lecture7.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture7/lecture7.pdf?raw=true) |
|   | Multiprogramming | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture8/lecture8.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture8/lecture8.pdf?raw=true) |
|   | Threads Introduction | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture9/lecture9.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture9/lecture9.pdf?raw=true) |
| 4 | Threads Implementation | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture10/lecture10.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture10/lecture10.pdf?raw=true) |
|   | User and Kernel Threads | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture12/lecture12.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture12/lecture12.pdf?raw=true) |
| 5 | Mutual Exclusion | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture13/lecture13.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture13/lecture13.pdf?raw=true) |
|   | Producer/Consumer | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture15/lecture15.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture15/lecture15.pdf?raw=true) |
| 6 | Readers/Writers | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture17/lecture17.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture17/lecture17.pdf?raw=true) |
|   | Dining Philosophers | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture18/lecture18.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture18/lecture18.pdf?raw=true) |
| 7 | Deadlocks | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture19/lecture19.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture19/lecture19.pdf?raw=true) |
| 8 | Deadlocks / Midterm |   |
| 9 | Process Scheduling | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture25/lecture25.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture25/lecture25.pdf?raw=true) |
| 10 | Spring Break |  |
| 11 | Memory Management | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture28/lecture28.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture28/lecture28.pdf?raw=true) |
|    | MM: Page Replacement | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture30/lecture30.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture30/lecture30.pdf?raw=true) |
| 12 | MM: Performance | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture32/lecture32.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture32/lecture32.pdf?raw=true) |
| 13 | File Systems Intro | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture34/lecture34.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture34/lecture34.pdf?raw=true) |
|    | FS: Open Files | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture35/lecture35.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture35/lecture35.pdf?raw=true) |
|    | FS: Implementation | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture36/lecture36.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture36/lecture36.pdf?raw=true) |
| 14 | FS: Performance | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture37/lecture37.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture37/lecture37.pdf?raw=true) |
|    | FS: Disk Scheduling | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture38/lecture38.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture38/lecture38.pdf?raw=true) |



------------

## Repo Information

### Repo Contents
Each directory contains a different set of files:

 - *lectureX.md* - lecture source
 - *lectureX.html* - lecture as presentation viewable in browser (reveal.js)
 - *lectureX.pdf* - lecture as PDF slides (beamer)

### Compiling
#### To compile reveal.js presentation:
    pandoc -t revealjs -s lectureX.md -o lectureX.html --slide-level 2 -V revealjs-url:../reveal.js --css ../slides.css
    

#### To compile PDF presentation:
    pandoc -t beamer -o lectureX.pdf lectureX.md --slide-level 2 -V theme:Berlin -V colortheme:beaver
    
Note: I currently use a custom version of pandoc to enable side-by-side columns: https://github.com/brianpeck/pandoc

### Viewing
To view the slides in a browser, click the link in the above table.

