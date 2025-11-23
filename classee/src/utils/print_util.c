#include "../../include/utils/print_util.h"
#include <stdio.h>

void printMenu() {
  printf("classee Initialized.\n");
  printf("classee Commands:\n");
  printf("  OPEN                - Open the database file.\n");
  printf("  SHOW ALL            - Display all student records.\n");
  printf("  SHOW SUMMARY ALL    - Display summary statistics for all courses.\n");
  printf(
      "  SHOW SUMMARY [course_name] - Display summary statistics. Optionally filter by course.\n");
  printf("  SHOW LOG            - Display the command log.\n");
  printf("  SHOW TLOG           - Display the transaction log for rollbacks.\n");
  printf("  INSERT [student id] - Insert a new student record.\n"); // TODO: combine it with
                                                                    // joseph's insert feature.
  printf("  QUERY  [student id] - Search for a student record.\n"); // TODO: add a query feature.
  printf("  UPDATE [student id] - Update an existing student record. \n"); // TODO: add an update
                                                                           // feature.
  printf("  DELETE [student id] - Delete an existing student record. \n"); // TODO: add a delete
                                                                           // feature.
  printf("  SAVE                - Save the current database to file.\n");
  printf("  ROLLBACK <change_id>- Rollback database to a previous state based on change ID.\n");
  printf("  HELP                - Show this help message.\n");
  printf("  EXIT                - Exit the program.\n");
  printf("\n");
  printf("Type open to load the database.\n");
}

void printDeclaration() {
  printf("                                 Declaration\n");
  printf("        SIT's policy on copying does not allow the students to copy source code as\n");
  printf("        well as assessment solutions from another person AI or other places. It is\n");
  printf("        the students' responsibility to guarantee that their assessment solutions are\n");
  printf("        their own work. Meanwhile, the students must also ensure that their work is\n");
  printf("        not accessible by others. Where such plagiarism is detected, both of the\n");
  printf("        assessments involved will receive ZERO mark.\n\n");

  printf("                               We hereby declare that:\n");
  printf("        - We fully understand and agree to the abovementioned plagiarism policy.\n");
  printf("        - We did not copy any code from others or from other places.\n");
  printf("        - We did not share our codes with others or upload to any other places for\n");
  printf("        - public access and will not do that in the future.\n");
  printf("        - We agree that our project will receive Zero mark if there is any\n");
  printf("        - plagiarism detected.\n");
  printf("        - We agree that we will not disclose any information or material of the group\n");
  printf("        - project to others or upload to any other places for public access.\n");
  printf("        - We agree that we did not copy any code directly from AI generated sources\n\n");

  printf("               Declared by: Group Name (please insert your group name)\n");
  printf("                               Team members:\n");
  printf("                                   1. XXX\n");
  printf("                                   2. XXX\n");
  printf("                                   3. XXX\n");
  printf("                                   4. XXX\n");
  printf("                                   5. XXX\n\n");
  printf("               Date: (please insert the date when you submit your group project).\n\n\n");
}
