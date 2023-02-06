#!/bin/bash
################################################################
# Filename    : update_version.sh                              #
# Description : update software version                        #
# Author      : Nicolas Lermé                                  #
# Date        : 28-02-2009                                     #
################################################################


###### Functions ######
function usage()
{
	echo "$0 new_version" && exit 1
}

function update()
{
	# update( filename, pattern, version )
	tmp_file=$RANDOM.tmp
	cat $1 | awk -F" " '{if($1 ~ /'$2'/) {gsub($3,"'$3'",$0);print $0;} else {print $0;}}' > ${tmp_file}
	mv ${tmp_file} $1
	chmod 755 $1
}


###### Program core ######
# We check parameters
[ $# -ne 1 ] && usage

# We update current version in `Makefile.common'
update "Makefile.common" "PROJECT_VERSION" $1
update "doc/doc.doxygen" "PROJECT_NUMBER" $1

exit 0
