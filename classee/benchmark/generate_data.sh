#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "usage: $0 <num_records> <output_file>" >&2
  exit 1
fi

NUM_RECORDS=$1
OUTPUT_FILE=$2

# data arrays
FIRST_NAMES=("Ethan" "Olivia" "Liam" "Ava" "Noah" "Bernard" "Sophia" "Jackson" "Isabella" "Aiden" "Mia" "Lucas" "Harper" "Riley" "Evelyn" "Logan" "Zoe" "Mason" "Amelia" "Carter" "Aubrey")
LAST_NAMES=("Lim" "Tan" "Ng" "Chen" "Wong" "Goh" "Chua" "Cheah" "Koh" "Teo" "Ang" "Ho" "Low" "Loh" "Yeo" "Sim" "Tay" "Wee" "Lee" "Ong" "Chan")
PROGS=("Artificial Intelligence" "Business Analytics" "Business Analytics" "Computer Science" "Computer Science" "Computer Science" "Computer Science" "Cybersecurity" "Data Science" "Data Science" "Data Science" "Digital Supply Chain" "Game Development" "Information Security" "Information Security" "Network Engineering" "Software Engineering" "Software Engineering" "Software Engineering" "Software Engineering")

START_ID=1900000
END_ID=2509999
ID_SPAN=$((END_ID - START_ID))

# pre-calc sizes
NUM_FNAME=${#FIRST_NAMES[@]}
NUM_LNAME=${#LAST_NAMES[@]}
NUM_PROGS=${#PROGS[@]}

(
  echo "Database Name: INF1002-CMS"
  echo "Authors: Joseph"
  echo ""
  echo -e "Table Name: StudentRecords\nID\tName\tProgramme\tMark"

  for ((i = 0; i < NUM_RECORDS; i++)); do
    # gen large rand num (30-bit)
    BIG_RAND=$(((RANDOM * 32768) | RANDOM))

    # calc id (linear scaling, same as c)
    if [ "$NUM_RECORDS" -eq 1 ]; then
      CURR_ID=$START_ID
    else
      OFFSET=$(((i * ID_SPAN) / (NUM_RECORDS - 1)))
      CURR_ID=$((START_ID + OFFSET))
    fi

    # use mod on big_rand var
    # use diff 'offsets' (shifting) or just mod to get indices
    # note: valid cuz big_rand huge (up to 1b)
    FNAME=${FIRST_NAMES[$((BIG_RAND % NUM_FNAME))]}
    LNAME=${LAST_NAMES[$(((BIG_RAND / 100) % NUM_LNAME))]}
    PROG=${PROGS[$(((BIG_RAND / 200) % NUM_PROGS))]}

    # gen mark
    # re-roll rand for mark to ensure indep
    MARK_RAND=$(((RANDOM * 32768) | RANDOM))
    RAW_MARK=$((MARK_RAND % 1001))

    printf "%d\t%s %s\t%s\t%d.%d\n" "$CURR_ID" "$FNAME" "$LNAME" "$PROG" "$((RAW_MARK / 10))" "$((RAW_MARK % 10))"
  done
) >"$OUTPUT_FILE"

echo "generated $NUM_RECORDS records"
