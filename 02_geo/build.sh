#!/bin/bash

function build()
{
for f in *.cpp; do
    echo  "Checking $f";
    if [ $(basename $f .cpp) -nt $f ]; then
       echo "==> Up to date"
    else
	   
	CMD=$(cat $f |grep "Compile" |cut -d":" -f2-)
	echo "==> Compiling: $CMD."
	if $CMD; then
	    echo "==> OK"
	else
	    echo "==> FAILED"
	fi
    fi
    
done
}

function clean()
{
    for f in *.cpp; do
	rm -f $(basename $f .cpp) 
    done
}




case $1 in
    "build")
	build
	;;
    "clean")
	clean
	;;
    *)
	echo "use build or clean as first arg"
	;;
esac

	
    





#    [ \$(basename $f .cpp) -nt $f ] || g++ -Wall -o \$(basename $f .cpp) $f; done
