#!/bin/bash
## Helper Variables
INPUT_FILE="$1"
WORK_DIR="$(dirname $0)"
TEMP_G_FILE=$(mktemp "${WORK_DIR}/gfile_XXXX")
TEMP_F_FILE=$(mktemp "${WORK_DIR}/ffile_XXXX")
TIME_FILE=$(mktemp "${WORK_DIR}/timings_XXXX")
TIME_1_FILE=$(mktemp "${WORK_DIR}/timings_XXXX")
SUPPORT_VALUES="5 10 25 50 95"
NUM_GRAPHS=$(grep '^#' ${INPUT_FILE} | wc -l | tr -d ' ')

## Translate input file to required formats
echo "Translating input file for gSpan, fsg and gaston formats"
python3 translate_file.py "${INPUT_FILE}" > "${TEMP_G_FILE}"
cat "${TEMP_G_FILE}" | sed 's/e/u/g'      > "${TEMP_F_FILE}"

## Time all binaries
for val in ${SUPPORT_VALUES}; do
    echo "Running gSpan for support $val"
    \time -f "%E" -o ${TIME_1_FILE} ./libraries/gSpan -f ${TEMP_G_FILE} -s 0$(bc -l <<< "$val/100") &> /dev/null
    cat ${TIME_1_FILE} >> ${TIME_FILE}
done
for val in ${SUPPORT_VALUES}; do
    echo "Running fsg for support $val"
    \time -f "%E" -o ${TIME_1_FILE} ./libraries/fsg -s $val ${TEMP_F_FILE} &> /dev/null
    cat ${TIME_1_FILE} >> ${TIME_FILE}
done
for val in ${SUPPORT_VALUES}; do
    echo "Running gaston for support $val"
    \time -f "%E" -o ${TIME_1_FILE} ./libraries/gaston $(bc -l <<< "$val*${NUM_GRAPHS}/100.0")  ${TEMP_G_FILE} &> /dev/null
    cat ${TIME_1_FILE} >> ${TIME_FILE}
done

## Plot graphs for time values
python3 plot.py ${TIME_FILE}

## Cleanup
rm -f "${TEMP_G_FILE}"* "${TEMP_F_FILE}"* "${TIME_1_FILE}" "${TIME_FILE}"
