# CprE 308 Spring 2014 Lectures
This repo contains all lecture slides for CprE 308 - Spring 2014.

## Lecture Listings

| Week | Topic | Slides |
|:----:|:-----:|:------:|
| 1 | Introduction | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture1/lecture1.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture1/lecture1.pdf?raw=true) |
|   | Hardware Review / Process Intro | [HTML](https://rawgithub.com/CprE308/lectures/master/lecture2/lecture2.html) / [PDF](https://github.com/CprE308/lectures/blob/master/lecture2/lecture2.pdf?raw=true) |

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

### Viewing
To view the slides in a browser, click the link in the above table.

