#!/bin/bash
# fabricates cms.log & cms.tlog from data file to sim bst build

DATA_FILE="../data/P3_7-CMS.txt"
TLOG_FILE="../data/cms.tlog"
LOG_FILE="../data/cms.log"

[ ! -f "$DATA_FILE" ] && { echo "error: $DATA_FILE not found"; exit 1; }

# read raw unsorted data from file (skip header)
mapfile -t unsorted_records < <(tail -n +6 "$DATA_FILE")
# explicit sort array & store in new sorted array
# more robust than pipe direct to 1st mapfile
mapfile -t records < <(printf "%s\n" "${unsorted_records[@]}" | sort -t$'\t' -n -k1)

# create headers
echo "change_id,command,id,name,programme,mark,time" > "$TLOG_FILE"
echo "change_id,command,time,is_change" > "$LOG_FILE"

change_id=0
timestamp=$(($(date +%s) - ${#records[@]} - 1))

# log init open cmd
((change_id++))
((timestamp++))
echo "$change_id,OPEN,$timestamp,0" >> "$LOG_FILE"

# recurse func to process sorted records in pre-order
process_records() {
  [ "$1" -gt "$2" ] && return # base case: invalid range

  local mid=$((($1 + $2) / 2))
  local line="${records[$mid]}"

  ((change_id++)); ((timestamp++))
  IFS=$'\t' read -r id name prog mark <<< "$line"

  # write to cms.log w/ full desc cmd str
  echo "$change_id,insert ID=$id Name=$name Programme=$prog Mark=$mark,$timestamp,1" >> "$LOG_FILE"
  # write to cms.tlog w/ struct fmt
  echo "$change_id,insert,$id,$name,$prog,$mark,$timestamp" >> "$TLOG_FILE"

  process_records "$1" $((mid - 1)) # recurse left
  process_records $((mid + 1)) "$2" # recurse right
}

echo "Fabricating logs from ${#records[@]} records..."
process_records 0 $((${#records[@]} - 1))
echo "Done. Created $LOG_FILE and $TLOG_FILE."
