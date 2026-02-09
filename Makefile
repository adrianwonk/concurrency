# target : prerequisites -> Terminal: ~/make identifier
	# command starts with TAB (a.k.a target recipe)
# make will ensure prerequisites are up to date before the target is up to date
# recipe will run iff. target file missing || target file is older than prerequisite files
#
# interesting shit:
# $^ : All prerequisites
# $@ : target
#
#
# speed program compilation with object files
SRCS := $(wildcard *.c)
out: $(SRCS)
	gcc -o $@ -g -O0 $^ -I.

clean:
	rm -f out
