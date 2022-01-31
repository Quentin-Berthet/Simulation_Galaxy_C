OPTIONS = --toc
OPTIONS += --filter=pandoc-numbering
OPTIONS += --filter=pandoc-crossref

PDFOPTIONS = --highlight-style kate
PDFOPTIONS += --pdf-engine pdflatex
PDFOPTIONS += --number-sections
PDFOPTIONS += --template=./default.latex

HTMLOPTIONS += -t html5
HTMLOPTIONS += -c css/prism.css
HTMLOPTIONS += --self-contained
HTMLOPTIONS += --mathjax=MathJax.js

MD=$(wildcard *.md)
HTML=$(MD:%.md=%.html)
PDF=$(MD:%.md=%.pdf)


all: $(HTML) $(PDF)

%.pdf: %.md Makefile
	sed -e 's/language-c/C/g' -e 's/language-bash/bash/g' $< > $*_tex.md
	pandoc -s $(OPTIONS) $(PDFOPTIONS) -o $@ $*_tex.md
	rm $*_tex.md

%.html: %.md Makefile
	pandoc -s $(OPTIONS) $(HTMLOPTIONS) -o $@ $<

deploy: all
	mkdir -p simulation_galaxie
	cp galaxie.html simulation_galaxie/index.html
	cp galaxie.pdf simulation_galaxie/galaxie.pdf

clean:
	rm -rf *.html *.pdf *_tex.*
