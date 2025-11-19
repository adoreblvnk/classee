#!/bin/bash
# fabricates cms.log and cms.tlog from a data file to simulate a bst build.

DATA_FILE="../data/P3_7-CMS.txt"
TLOG_FILE="../data/cms.tlog"
LOG_FILE="../data/cms.log"

[ ! -f "$DATA_FILE" ] && { echo "error: $DATA_FILE not found"; exit 1; }

# --- DEFINITIVE FIX V2 ---
# Step 1: Read the raw, unsorted data from the file (skipping header).
mapfile -t unsorted_records < <(tail -n +6 "$DATA_FILE")
# Step 2: Explicitly sort the array and store it in a new, sorted array.
# This is more robust than piping directly into the first mapfile.
mapfile -t records < <(printf "%s\n" "${unsorted_records[@]}" | sort -t$'\t' -n -k1)

# create headers for both files
echo "change_id,command,id,name,programme,mark,time" > "$TLOG_FILE"
echo "change_id,command,time,is_change" > "$LOG_FILE"

change_id=0
timestamp=$(($(date +%s) - ${#records[@]} - 1))

# log the initial OPEN command
((change_id++))
((timestamp++))
echo "$change_id,OPEN,$timestamp,0" >> "$LOG_FILE"

# recursive func to process the SORTED records in pre-order traversal
process_records() {
  [ "$1" -gt "$2" ] && return # base case: invalid range

  local mid=$((($1 + $2) / 2))
  local line="${records[$mid]}"

  ((change_id++)); ((timestamp++))
  IFS=$'\t' read -r id name prog mark <<< "$line"

  # write to cms.log with the full, descriptive command string
  echo "$change_id,insert ID=$id Name=$name Programme=$prog Mark=$mark,$timestamp,1" >> "$LOG_FILE"
  # write to cms.tlog with its structured format
  echo "$change_id,insert,$id,$name,$prog,$mark,$timestamp" >> "$TLOG_FILE"

  process_records "$1" $((mid - 1)) # recurse on left half
  process_records $((mid + 1)) "$2" # recurse on right half
}

echo "Fabricating logs from ${#records[@]} records..."
process_records 0 $((${#records[@]} - 1))
echo "Done. Created $LOG_FILE and $TLOG_FILE."
