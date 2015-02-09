
all: *.pdf *.html

%.pdf: %.md
	pandoc -t beamer -o $@ $< --slide-level 2 -V theme:Berlin -V colortheme:beaver

%.html: %.md
	pandoc -t revealjs -s $< -o $@ --slide-level 2 -V revealjs-url:../reveal.js --css ../slides.css



