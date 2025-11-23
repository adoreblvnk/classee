#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s <num_records> <output_file>\n", argv[0]);
    return 1;
  }

  FILE *file = fopen(argv[2], "w");
  if (!file) {
    perror("error opening file");
    return 1;
  }

  int num_records = atoi(argv[1]);
  if (num_records <= 0) {
    fprintf(stderr, "num_records must be positive\n");
    return 1;
  }

  const char *first_names[] = {"Ethan",  "Olivia",  "Liam",     "Ava",    "Noah", "Bernard",
                               "Sophia", "Jackson", "Isabella", "Aiden",  "Mia",
                               "Lucas",  "Harper",  "Riley",    "Evelyn", "Logan",
                               "Zoe",    "Mason",   "Amelia",   "Carter", "Aubrey"};
  const char *last_names[] = {"Lim", "Tan", "Ng",  "Chen", "Wong", "Goh", "Chua", "Cheah",
                              "Koh", "Teo", "Ang", "Ho",   "Low",  "Loh", "Yeo",
                              "Sim", "Tay", "Wee", "Lee",  "Ong",  "Chan"};

  const char *progs[] = {
      "Artificial Intelligence",
      "Business Analytics",
      "Business Analytics",
      "Computer Science",
      "Computer Science",
      "Computer Science",
      "Computer Science",
      "Cybersecurity",
      "Data Science",
      "Data Science",
      "Data Science",
      "Digital Supply Chain",
      "Game Development",
      "Information Security",
      "Information Security",
      "Network Engineering",
      "Software Engineering",
      "Software Engineering",
      "Software Engineering",
      "Software Engineering",
  };

  int num_progs = sizeof(progs) / sizeof(progs[0]);

  fprintf(file, "Database Name: INF1002-CMS\nAuthors: Joseph\n\n");
  fprintf(file, "Table Name: StudentRecords\nID\tName\tProgramme\tMark\n");

  const long START_ID = 1900000;
  const long END_ID = 2509999; // IDs from 19xxxxx to 25xxxxx

  srand(time(NULL));
  for (int i = 0; i < num_records; i++) {
    const char *first_name = first_names[rand() % (sizeof(first_names) / sizeof(first_names[0]))];
    const char *last_name = last_names[rand() % (sizeof(last_names) / sizeof(last_names[0]))];
    long id;

    // scale ID proportionally across all years
    if (num_records == 1) {
      id = START_ID;
    } else {
      long id_span = END_ID - START_ID;
      id = START_ID + (long)((double)i * id_span / (num_records - 1));
    }

    // %ld for long int
    fprintf(file, "%ld\t%s %s\t%s\t%.1f\n", id, first_name, last_name, progs[rand() % num_progs],
            (float)(rand() % 1001) / 10.0);
  }

  fclose(file);
  printf("generated %d records in '%s'\n", num_records, argv[2]);
  return 0;
}
