#!/bin/bash
TEMPO=250
DURATION=$TEMPO
CURRENT_OCTAVE=0
INPUT=
OUTPUT=
SCORE_NAME="My music score"
N_TEMPO=0
BLOCK_COUNTER=0

# Play a note, given its name, octave and length
play()
{
	if [ -z "$1" ]; then
		return
	fi

	if [ "$1" = "TEMPO" ]
	then
		TEMPO="$2"

		if [ ${N_TEMPO} -eq 1 ]; then
			echo "<score tempo=\"${TEMPO}\" name=\"${SCORE_NAME}\">" >> "${OUTPUT}"
		else
			if [ ${BLOCK_COUNTER} -eq 0 ]; then
				echo "<score name=\"${SCORE_NAME}\">" >> "${OUTPUT}"
			else
				echo "</block>" >> "${OUTPUT}"
			fi

			echo "<block tempo=\"${TEMPO}\">" >> "${OUTPUT}"
			BLOCK_COUNTER=$((BLOCK_COUNTER+1))
		fi

		DURATION="1"
		return
	fi

	if [ -n "$2" ]; then
		CURRENT_OCTAVE="$2"
	fi

	if [ -n "$3" ] 
	then
		DURATION=`echo "scale=2; $3" | bc -l`
		RES=`echo "${DURATION}" | grep -q "^\..*$"`

		if [ $? == 0 ]; then
			DURATION="0${DURATION}"
		fi
	fi
	
	if [ "$1" = "_" ]
	then
		echo "<pause duration=\"${DURATION}\" />" >> "${OUTPUT}"
	else
		echo "<note value=\"$1,${CURRENT_OCTAVE}\" duration=\"${DURATION}\" />" >> "${OUTPUT}"
	fi
}

# Read the music sheet
readfile()
{
	N_TEMPO=`cat "${INPUT}" | grep "TEMPO" | wc -l`

	echo "<?xml version=\"1.0\" encoding=\"ISO-8859-15\"?>" > "${OUTPUT}"

	while read LINE
	do
		play `echo ${LINE} | awk '{ print $1; print $2; print $3}'` 
	done

	if [ ${BLOCK_COUNTER} -gt 0 ]; then
		echo "</block>" >> "${OUTPUT}"
	fi

	echo "</score>" >> "${OUTPUT}"
}

usage()
{
	echo "usage: `basename $0` input output [score_name]"
}


################# Core #################
if [[ $# -lt 2 ]] || [[ $# -gt 3 ]]; then
	usage && exit 1
fi

INPUT="$1"
OUTPUT="$2"

if [ $# -eq 3 ]; then
	if [ -z "$3" ]; then
		echo "error: the score name is empty"
		exit 1
	fi

	SCORE_NAME="$3"
fi

if [ ! -f "${INPUT}" ]; then
	echo "error: unable to open input file '${INPUT}'" && exit 1
fi

# We ask for overwriting
until [ ! -f "${OUTPUT}" ]
do
	echo -n "warning: output file already exists. Overwrite it? (y/n): "
	read answer

	echo "${answer}" | grep -qi "y"

	[ $? == 0 ] && break

	echo -n "question: please, give another valid output file name: "
	read OUTPUT
done

# We replace notes
perl -pi -e "s/DO_/C#/g" "${INPUT}"
perl -pi -e "s/DO/C/g" "${INPUT}"
perl -pi -e "s/RE_/D#/g" "${INPUT}"
perl -pi -e "s/RE/D/g" "${INPUT}"
perl -pi -e "s/MI/E/g" "${INPUT}"
perl -pi -e "s/FA_/F#/g" "${INPUT}"
perl -pi -e "s/FA/F/g" "${INPUT}"
perl -pi -e "s/SOL_/G#/g" "${INPUT}"
perl -pi -e "s/SOL/G/g" "${INPUT}"
perl -pi -e "s/LA_/A#/g" "${INPUT}"
perl -pi -e "s/LA/A/g" "${INPUT}"
perl -pi -e "s/SI/B/g" "${INPUT}"

# Process
cat "${INPUT}" | readfile

# Indentation
OUTPUT_TMP="${OUTPUT}_$$_${RANDOM}_$$.tmp"
\xmllint "${OUTPUT}" > "${OUTPUT_TMP}" --format
\mv -f "${OUTPUT_TMP}" "${OUTPUT}"

# Rights
chmod +x "${OUTPUT}"

exit 0
