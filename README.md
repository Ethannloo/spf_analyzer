Project Description
This project is a C program designed to analyze student performance factors based on data from two datasets in CSV and YAML formats. The program processes, merges, and filters this data to produce specific outputs based on various tasks. It is developed as part of SENG 265 - Software Development Methods, focusing on file I/O, struct-based data storage, and static memory allocation in C.

Objective
The goal of this project is to develop a C program called spf_analyzer.c that reads and processes data from:
a1-data-curricular.csv → Curricular performance factors (e.g., exam scores, attendance, hours studied).
a1-data-extracurricular.yaml → Extracurricular factors (e.g., sleep hours, physical activity, participation in extracurricular activities).
The program performs various analysis tasks and outputs results in CSV format.

Key Tasks Implemented
The program supports 6 tasks:
Task Number
Description
Output
1
Generate a CSV file with Record_ID and Exam_Score for students who scored above 90 on the final exam.
output.csv
2
Convert the extracurricular YAML dataset into CSV format.
output.csv
3
Merge the curricular and extracurricular datasets based on Record_ID and output students with Exam_Score > 90.
output.csv
4
Generate a CSV file with Record_ID and Exam_Score for students with 100% attendance.
output.csv
5
Generate a CSV file with Record_ID and Exam_Score for students whose Sleep_Hours >= Hours_Studied.
output.csv
6
Generate a CSV file with Record_ID, Exam_Score, and Extracurricular_Activities for students who scored below 60.
output.csv


Program Structure
CurricularData struct → Stores CSV student performance data.
ExtracurricularData struct → Stores YAML extracurricular data.
Static Memory Allocation → No dynamic memory allocation (e.g., malloc) is used.
File Processing:
CSV Parsing → Read curricular data using fgets() and sscanf().
YAML Parsing → Read extracurricular data using fgets() and custom parsing logic.
Output Files → Results are written to output.csv.

How to Run
bash
CopyEdit
gcc spf_analyzer.c -std=c99 -o spf_analyzer
./spf_analyzer --TASK=1  # Replace "1" with any task number (1 to 6)


Files
spf_analyzer.c → Main C program containing all logic.
data/a1-data-curricular.csv → Curricular dataset (input file).
data/a1-data-extracurricular.yaml → Extracurricular dataset (input file).
output.csv → Output file generated after running a task.

