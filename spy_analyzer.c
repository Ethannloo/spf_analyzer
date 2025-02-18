#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 6608

/*
* Struct: CurricularData
* ----------------------
* @brief Represents a row from the a1-data-curricular.csv file.
*/
typedef struct {
   int record_id;           // Maps to the "Record_ID" column
   int hours_studied;       // Maps to the "Hours_Studied" column
   int attendance;          // Maps to the "Attendance" column
   int tutoring_sessions;   // Maps to the "Tutoring_Sessions" column
   int exam_score;          // Maps to the "Exam_Score" column
} CurricularData;

/*
* Struct: ExtracurricularData
*
* Reperesents a block containing student extracurriclar activities in a1-extracurricular.yaml.
*/
typedef struct {
    char extracurricular_activities[16];
    int physical_activity;
    int record_id;
    int sleep_hours;
} ExtracurricularData;

/*
* Struct: MergedData
*
* Represents the merged data from a1-curricular.csv and a1-extracurricular.yaml
*/
typedef struct {
    int record_id;
    int hours_studied;
    int attendance;
    int tutoring_sessions;
    int exam_score;
    char extracurricular_activities[16];
    int physical_activity;
    int sleep_hours;
} MergedData;


/**
* Function: read_csv_file
* -----------------------
* @brief Reads data from the a1-data-curricular.csv file and populates an array of CurricularData structs.
*
* @param filename The name of the CSV file to read.
* @param data Array of CurricularData where the CSV data will be stored.
* @return int The number of records successfully read.
*/
int read_csv_file(const char *filename, CurricularData data[]) {
   FILE *file = fopen(filename, "r");
   if (file == NULL) {
       printf("Error: Could not open file %s\n", filename);
       return -1;
   }

   // Skip the header line
   char buffer[256];
   fgets(buffer, sizeof(buffer), file);

   int index = 0;
   while (fgets(buffer, sizeof(buffer), file) != NULL && index < MAX_RECORDS) {
       sscanf(buffer, "%d,%d,%d,%d,%d",
              &data[index].record_id,
              &data[index].hours_studied,
              &data[index].attendance,
              &data[index].tutoring_sessions,
              &data[index].exam_score);
       index++;
   }

   fclose(file);
   return index;
}

/**
* Function: print_data
* --------------------
* @brief Prints the contents of the array of CurricularData.
*
* @param data Array of CurricularData to print.
* @param size The number of records in the array.
*/
void print_data(const CurricularData data[], int size) {
   printf("Record_ID | Hours_Studied | Attendance | Tutoring_Sessions | Exam_Score\n");
   printf("-----------------------------------------------------------------------\n");
   for (int i = 0; i < size; i++) {
       printf("%10d | %13d | %10d | %17d | %10d\n",
           data[i].record_id,
           data[i].hours_studied,
           data[i].attendance,
           data[i].tutoring_sessions,
           data[i].exam_score);
   }
}


/**
 * Function: task_1
 * ----------------
 * Writes to output.csv the Record_ID and Exam_Score for all students whose Exam_Score is greater than 90.
 *
 * @param data Array of CurricularData.
 * @param size Number of records.
 */

void task_1(CurricularData data[], int size) {
    FILE *file;
    file = fopen("output.csv", "w");

    if (file == NULL) {
       printf("Error opening file.\n");
       return;
    }
    // Write csv header
    fprintf(file, "Record_ID,Exam_Score\n");
    for (int i = 0; i < size; i++) {
        if (data[i].exam_score > 90) {
            fprintf(file, "%d,%d\n", data[i].record_id, data[i].exam_score);
        }
       //write Record_id and exam score into output file
    }

   fclose(file);
   printf("Output has been written to output.csv\n");
}

/*
* Function: ltrim
*
* Removes leading spaces and tabs from string
*/
static char* ltrim(char* str) {
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    return str;
}

/*
* Function: parse_extracurricular_data
*
* Parses the data in extracurricular yaml file into an array
*/
static int parse_extracurricular_data(FILE *in, ExtracurricularData data[]) {
    char line[256];
    int count = 0, reading = 0;
    ExtracurricularData current;
    memset(&current, 0, sizeof(current));

    while (fgets(line, sizeof(line), in)) {
        // Remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        // Trim leading spaces
        char *trimmed = ltrim(line);
        if (!trimmed[0]) continue; // skip empty

        // Skip "records:" line
        if (strcmp(trimmed, "records:") == 0) {
            continue;
        }

        // Checks if line starts with '-'
        if (trimmed[0] == '-') {
            if (reading) {
                data[count++] = current;
                memset(&current, 0, sizeof(current));
            }
            reading = 1;
            
            // Move past the dash
            trimmed++;  
            while (*trimmed == ' ' || *trimmed == '\t') {
                trimmed++;
            }
            // If there's nothing left (just a dash), go to next line
            if (*trimmed == '\0') {
                continue;
            }
        }

        // Parse the Key: Value from trimmed if present
        char *colonPos = strchr(trimmed, ':');
        if (!colonPos) {
            continue;
        }
        *colonPos = '\0';
        char *key = trimmed;
        char *value = colonPos + 1;

        // remove leading space in value if any
        if (*value == ' ') value++;

        // strip quotes if present
        size_t len = strlen(value);
        if (len >= 2 &&
           ((value[0] == '"'  && value[len - 1] == '"') ||
            (value[0] == '\'' && value[len - 1] == '\''))) {
            value[len - 1] = '\0';
            value++;
        }

        // map key to records
        if (strcmp(key, "Extracurricular_Activities") == 0) {
            strncpy(current.extracurricular_activities, value,
                    sizeof(current.extracurricular_activities) - 1);
        } else if (strcmp(key, "Physical_Activity") == 0) {
            current.physical_activity = atoi(value);
        } else if (strcmp(key, "Record_ID") == 0) {
            current.record_id = atoi(value);
        } else if (strcmp(key, "Sleep_Hours") == 0) {
            current.sleep_hours = atoi(value);
        }
    }

    if (reading) {
        data[count++] = current;
    }

    return count;
}

static void write_extracurricular_data_to_csv(FILE *out, const ExtracurricularData data[], int count) {
    // Write everything to CSV
    for (int i = 0; i < count; i++) {
        fprintf(out, "%s,%d,%d,%d\n",
            data[i].extracurricular_activities,
            data[i].physical_activity,
            data[i].record_id,
            data[i].sleep_hours);
    }
}

/**
 * Function: task_2
 * ----------------
 * Parses the YAML file for extracurricular data and writes it to output.csv.
 *
 * @param yamlFilename The name of the YAML file.
 * @param data         Array where the parsed data is stored.
 * @return             The number of records parsed, or -1 on error.
 */
int task_2(const char *yamlFilename, ExtracurricularData data[]) {
    FILE *in = fopen(yamlFilename, "r");
    if (!in) {
        printf("Error opening %s\n", yamlFilename);
        return -1;
    }

    FILE *out = fopen("output.csv", "w");
    if (!out) {
        printf("Error opening output.csv\n");
        fclose(in);
        return -1;
    }

    // Write header line
    fprintf(out, "Extracurricular_Activities,Physical_Activity,Record_ID,Sleep_Hours\n");

    // Parse YAML and store in data array
    int count = parse_extracurricular_data(in, data);

    // Write results to CSV
    write_extracurricular_data_to_csv(out, data, count);

    fclose(in);
    fclose(out);

    printf("Parsed %d records.\n", count);
    return count;
}

/**
 * Function: task_3
 * ----------------
 * Merges curricular and extracurricular data based on Record_ID.
 * Only records with Exam_Score > 90 are included.
 *
 * @param curricular_data Array of CurricularData.
 * @param curr_size       Number of curricular records.
 * @param extra_data      Array of ExtracurricularData.
 * @param extra_size      Number of extracurricular records.
 */


void task_3(CurricularData curricular_data[], int curr_size, ExtracurricularData extra_data[], int extra_size) {
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    //Write CSV header
    fprintf(file, "Record_ID,Hours_Studied,Attendance,Tutoring_Sessions,Exam_Score,Extracurricular_Activities,Physical_Activity,Sleep_Hours\n");

    //Merge datasets using Record_ID
    for (int i = 0; i < curr_size; i++) {
        if (curricular_data[i].exam_score <= 90) continue;  //Only include high scorers

        //Find matching ExtracurricularData
        for (int j = 0; j < extra_size; j++) {
            if (curricular_data[i].record_id == extra_data[j].record_id) {
                //Write merged data to CSV
                fprintf(file, "%d,%d,%d,%d,%d,%s,%d,%d\n",
                    curricular_data[i].record_id,
                    curricular_data[i].hours_studied,
                    curricular_data[i].attendance,
                    curricular_data[i].tutoring_sessions,
                    curricular_data[i].exam_score,
                    extra_data[j].extracurricular_activities,
                    extra_data[j].physical_activity,
                    extra_data[j].sleep_hours);
                break;  //Stop searching once match is found
            }
        }
    }

    fclose(file);
    printf("Task 3 completed. Merged records where Exam_Score > 90 written to output.csv\n");
}

/**
 * Function: task_4
 * ----------------
 * Writes the Record_ID and Exam_Score for students with 100% attendance to output.csv.
 *
 * @param curricular_data Array of CurricularData.
 * @param curr_size       Number of records.
 */
void task_4(CurricularData curricular_data[], int curr_size) {
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(file, "Record_ID,Exam_Score\n");

    int records = 0;

    for (int i = 0; i < curr_size; i++) {
        if (curricular_data[i].attendance == 100) {
            fprintf(file, "%d,%d\n", 
                curricular_data[i].record_id,
                curricular_data[i].exam_score);
            records = 1;
        }
    }

    fclose(file);

    if (!records) {
        printf("Error: No students with 100%% attendance\n");
    } else {
        printf("Task 4 complete\n");
    }
}

/**
 * Function: task_5
 * ----------------
 * Writes to output.csv the Record_ID and Exam_Score for records where the student's Sleep_Hours
 * is greater than or equal to Hours_Studied.
 *
 * @param curricular_data Array of CurricularData.
 * @param curr_size       Number of curricular records.
 * @param extra_data      Array of ExtracurricularData.
 * @param extra_size      Number of extracurricular records.
 */
void task_5(CurricularData curricular_data[], int curr_size, 
            ExtracurricularData extra_data[], int extra_size)
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header line to CSV
    fprintf(file, "Record_ID,Exam_Score\n");

    // For each record in curricular_data
    for (int i = 0; i < curr_size; i++) {
        
        // Find the matching record in extra_data by Record_ID
        for (int j = 0; j < extra_size; j++) {
            if (curricular_data[i].record_id == extra_data[j].record_id) {
                
                // Check if Sleep_Hours >= Hours_Studied
                if (extra_data[j].sleep_hours >= curricular_data[i].hours_studied) {
                    fprintf(file, "%d,%d\n",
                            curricular_data[i].record_id,
                            curricular_data[i].exam_score);
                }
                break;
            }
        }
    }

    fclose(file);
    printf("Task 5 completed. Output has been written to output.csv\n");
}

/**
 * Function: task_6
 * ----------------
 * Writes to output.csv the Record_ID, Exam_Score, and Extracurricular_Activities for
 * records where Exam_Score is below 60.
 *
 * @param curricular_data Array of CurricularData.
 * @param curr_size       Number of curricular records.
 * @param extra_data      Array of ExtracurricularData.
 * @param extra_size      Number of extracurricular records.
 */
void task_6(CurricularData curricular_data[], int curr_size, 
            ExtracurricularData extra_data[], int extra_size)
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header line
    fprintf(file, "Record_ID,Exam_Score,Extracurricular_Activities\n");

    // Loop through curricular data
    for (int i = 0; i < curr_size; i++) {

        // students who scored below 60
        if (curricular_data[i].exam_score < 60) {
            
            // Find matching record in extracurricular data
            for (int j = 0; j < extra_size; j++) {
                if (curricular_data[i].record_id == extra_data[j].record_id) {
                    
                    // Write the required fields to CSV
                    fprintf(file, "%d,%d,%s\n",
                            curricular_data[i].record_id,
                            curricular_data[i].exam_score,
                            extra_data[j].extracurricular_activities);              
                    break;
                }
            }
        }
    }

    fclose(file);
    printf("Task 6 completed. Output has been written to output.csv\n");
}



/**
* Function: main
* --------------
* @brief The main function and entry point of the program.
*
* @return int 0: No errors; 1: Errors produced.
*/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s --TASK=\"<task_number>\"\n", argv[0]);
        return 1; // Exit with an error code     
    }

    int task = 0;
    if (strncmp(argv[1], "--TASK=", 7) == 0) {
        task = atoi(argv[1] + 7);  // Convert string after "--TASK=" to integer
    } else {
        printf("Error: Invalid argument format. Use --TASK=\"<task_number>\"\n");
        return 1;
    }

    // Validate task number
    if (task < 1 || task > 6) {
        printf("Error: Task number must be between 1 and 6.\n");
        return 1;
    }

    CurricularData data[MAX_RECORDS];  // Array to hold all rows from the CSV
    const char *filename = "data/a1-data-curricular.csv";

    int num_records = read_csv_file(filename, data);
    if (num_records < 0) {
        return 1;
    }

    printf("\nSuccessfully read %d records from %s\n\n", num_records, filename);

    switch (task) {
        case 1:
            printf("Executing Task 1...\n");
            task_1(data, num_records);
            break;
        case 2:
            printf("Executing Task 2...\n");
            {
                ExtracurricularData extraData[MAX_RECORDS];
                int count = task_2("data/a1-data-extracurricular.yaml", extraData);
                if (count > 0) {
                    printf("Task 2 completed. %d records written to output.csv\n", count);
                } else {
                    printf("Task 2 failed or no records were written.\n");
                }
            }
            break; 
        case 3:
            printf("Executing Task 3...\n");
            {
                ExtracurricularData extra_data[MAX_RECORDS];
                int extra_size = task_2("data/a1-data-extracurricular.yaml", extra_data);

                if (extra_size < 0) {
                    printf("Failed to load extracurricular data.\n");
                    return 1;
                }

                task_3(data, num_records, extra_data, extra_size);
            }
            break;
        case 4:
            printf("Executing Task 4...\n");
            task_4(data, num_records);
            break;
        case 5: {
            printf("Executing Task 5...\n");

            ExtracurricularData extraData5[MAX_RECORDS];
            int extra_size5 = task_2("data/a1-data-extracurricular.yaml", extraData5);
            if (extra_size5 < 0) {
                printf("Failed to load extracurricular data for Task 5.\n");
                return 1;
            }


            task_5(data, num_records, extraData5, extra_size5);
            break;
        }

        case 6: {
            printf("Executing Task 6...\n");

            ExtracurricularData extraData6[MAX_RECORDS];
            int extra_size6 = task_2("data/a1-data-extracurricular.yaml", extraData6);
            if (extra_size6 < 0) {
                printf("Failed to load extracurricular data for Task 6.\n");
                return 1;
            }

            task_6(data, num_records, extraData6, extra_size6);
            break;
        }
    }
    return 0;
}
