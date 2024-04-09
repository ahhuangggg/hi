#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODE 10
#define MAX_NAME 51
#define MAX_LINE 256
#define MAX_COURSE_ID 11
#define MAX_ID 11
#define MAX_COURSES 5
#define MAX_COURSE_NAME 30
#define MAX_GRADE 5
#define MAX_ATTENDANCE 21

// Structure definition
typedef struct {
    char courseCode[MAX_CODE];
    char courseName[MAX_NAME];
    int numStudentsEnrolled;
    char lecturerName[MAX_NAME];
} Course;

typedef struct {
    char studentID[MAX_ID];
    char name[MAX_NAME];
    int numCoursesEnrolled;
    char coursesEnrolled[MAX_COURSES][MAX_COURSE_NAME];
} Student;

// Function prototypes
void lecturerMenu();
void viewCourses();
void promptAndUpdateCourse();
void viewAllStudents();
void viewGrades();
void modifyGrades();
void viewAttendance();
void updateAttendance();


int main() {
    lecturerMenu();
    return 0;
}

void lecturerMenu() {
    int choice;
    do {
         printf("\n********** Lecturer Menu **********\n");
        printf("1. View Courses\n");
        printf("2. Update Course\n"); 
        printf("3. View All Students\n");
        printf("4. View Grades\n");
        printf("5. Modify Grades\n");
        printf("6. View Attendance\n");
        printf("7. Update Attendance\n");
        printf("8. Logout\n");
        printf("***********************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); 

        switch(choice) {
            case 1:
                viewCourses();
                break;
            case 2:
                promptAndUpdateCourse();
                break;
            case 3:
                viewAllStudents();
                break;
            case 4:
                viewGrades();
                break;
            case 5:
                modifyGrades();
                break;
            case 6:
                viewAttendance();
                break;
            case 7:
                updateAttendance();
                break;
            case 8:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
                break;
        }
    } while(choice != 8);
}


void viewCourses() {
    FILE *file;
    char currentCode[MAX_CODE];
    char currentName[MAX_NAME];
    int currentStudents;
    char currentLecturer[MAX_NAME];
    const char *filename = "C:\\Degree Year 1 Sem 2\\C programming\\courses.txt"; 

    file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    printf("Courses available:\n");
    printf("Code\tName\t\t\tEnrolled\tLecturer\n");
    printf("------------------------------------------------------------\n");

    while (fscanf(file, "%9[^,],%49[^,],%d,%49[^\n]\n", currentCode, currentName, &currentStudents, currentLecturer) == 4) {
        printf("%s\t%-25s\t%d\t\t%s\n", currentCode, currentName, currentStudents, currentLecturer);
    }

    fclose(file);
}

void promptAndUpdateCourse() {
    Course courseToUpdate;
    printf("Enter course code to update: ");
    scanf("%9s", courseToUpdate.courseCode);
    getchar(); // consume newline
    printf("Enter new course name: ");
    fgets(courseToUpdate.courseName, MAX_NAME, stdin);
    courseToUpdate.courseName[strcspn(courseToUpdate.courseName, "\n")] = 0; // Remove newline character
    printf("Enter new number of students enrolled: ");
    scanf("%d", &courseToUpdate.numStudentsEnrolled);
    getchar(); // consume newline
    printf("Enter new lecturer name: ");
    fgets(courseToUpdate.lecturerName, MAX_NAME, stdin);
    courseToUpdate.lecturerName[strcspn(courseToUpdate.lecturerName, "\n")] = 0; // Remove newline character

    FILE *file = fopen("C:\\Degree Year 1 Sem 2\\C programming\\courses.txt", "r");
    if (!file) {
        perror("Unable to open the file for reading");
        return;
    }

    FILE *tempFile = fopen("temp_courses.txt", "w");
    if (!tempFile) {
        perror("Unable to open temporary file for writing");
        fclose(file); 
        return;
    }
    
    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, file) != NULL) {
        Course currentCourse;
        sscanf(line, "%[^,],%[^,],%d,%[^\n]", currentCourse.courseCode, currentCourse.courseName, &currentCourse.numStudentsEnrolled, currentCourse.lecturerName);
        
        if (strcmp(currentCourse.courseCode, courseToUpdate.courseCode) == 0) {
            // Match found, write updated course info to temp file
            fprintf(tempFile, "%s,%s,%d,%s\n", courseToUpdate.courseCode, courseToUpdate.courseName, courseToUpdate.numStudentsEnrolled, courseToUpdate.lecturerName);
        } else {
            // No match, write current line to temp file
            fprintf(tempFile, "%s", line); 
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with updated file
    remove("courses.txt");
    rename("temp_courses.txt", "courses.txt");
    printf("Course updated successfully.\n");
}


void viewAllStudents() {
    FILE *file = fopen("student profiles.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    printf("Student Profiles:\n");
    printf("ID\tName\t\tCourses Enrolled\n");

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file) != NULL) {
        Student student;
        char *token;
        char *rest = line;
        
        // Parse the student ID
        token = strtok_r(rest, ",", &rest);
        if (token) strncpy(student.studentID, token, MAX_ID);
        
        // Parse the student name
        token = strtok_r(rest, ",", &rest);
        if (token) strncpy(student.name, token, MAX_NAME);
        
        // Parse the courses enrolled
        student.numCoursesEnrolled = 0;
        while ((token = strtok_r(rest, ",", &rest)) && student.numCoursesEnrolled < MAX_COURSES) {
            strncpy(student.coursesEnrolled[student.numCoursesEnrolled], token, MAX_COURSE_NAME);
            student.numCoursesEnrolled++;
        }

        // Print the student profile
        printf("%s\t%s\t\t", student.studentID, student.name);
        for (int i = 0; i < student.numCoursesEnrolled; i++) {
            printf("%s", student.coursesEnrolled[i]);
            if (i < student.numCoursesEnrolled - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    fclose(file);
}

void viewGrades() {
    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    printf("ID\tStudent Name\t\t\tCourse ID\tGrade\n");
    printf("----------------------------------------------------------------\n");

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        char studentID[MAX_ID], studentName[MAX_NAME], courseCode[MAX_COURSE_ID], grade[MAX_GRADE];
        if (sscanf(line, "%[^,],%[^,],%[^,],%s", studentID, studentName, courseCode, grade) == 4) {
            printf("%s\t%-20s\t\t%s\t\t%s\n", studentID, studentName, courseCode, grade);
        }
    }

    fclose(file);
}

void modifyGrades() {
    char targetStudentID[MAX_ID], targetCourseCode[MAX_CODE];
    char newGrade[10];
    printf("Enter Student ID to modify grade: ");
    scanf("%s", targetStudentID);
    printf("Enter Course Code: ");
    scanf("%s", targetCourseCode);
    printf("Enter new Grade: ");
    scanf("%s", newGrade);

    FILE* file = fopen("grades.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    FILE* tempFile = fopen("temp_grades.txt", "w");
    if (!tempFile) {
        perror("Unable to open temporary file for writing");
        fclose(file);
        return;
    }

    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char studentID[MAX_ID], courseCode[MAX_CODE], grade[10];
        sscanf(line, "%[^,],%[^,],%s", studentID, courseCode, grade);

        // Check if this line matches the grade to be modified
        if (strcmp(studentID, targetStudentID) == 0 && strcmp(courseCode, targetCourseCode) == 0) {
            fprintf(tempFile, "%s,%s,%s\n", studentID, courseCode, newGrade);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace original file with updated file
    remove("grades.txt");
    rename("temp_grades.txt", "grades.txt");

    if (found) {
        printf("Grade updated successfully.\n");
    } else {
        printf("No matching grade found to update.\n");
    }
}


void viewAttendance() {
    FILE* file = fopen("attendance.txt", "r");
    if (!file) {
        perror("Unable to open the file");
        return;
    }

    printf("Attendance Records:\n");
    printf("ID\tStudent Name\t\tCourse ID\tAttendance\n");
    printf("----------------------------------------------------------\n");

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        char studentID[MAX_ID], studentName[MAX_NAME], courseID[MAX_COURSE_ID], attendanceStatus[MAX_ATTENDANCE];
        if (sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", studentID, studentName, courseID, attendanceStatus) == 4) {
            printf("%s\t%-20s\t%s\t\t%s\n", studentID, studentName, courseID, attendanceStatus);
        }
    }

    fclose(file);
}

void updateAttendance() {
    char targetStudentID[MAX_ID], targetCourseID[MAX_COURSE_ID];
    char newAttendanceStatus[MAX_ATTENDANCE];
    
    printf("Enter Student ID to update attendance: ");
    scanf("%10s", targetStudentID);
    while (getchar() != '\n');  

    printf("Enter Course ID: ");
    scanf("%10s", targetCourseID);
    while (getchar() != '\n');  

    printf("Enter new Attendance Status (Present/Absent): ");
    fgets(newAttendanceStatus, MAX_ATTENDANCE, stdin);
    newAttendanceStatus[strcspn(newAttendanceStatus, "\n")] = 0; 
    FILE *file = fopen("attendance.txt", "r");
    if (!file) {
        perror("Unable to open the file for reading");
        return;
    }

    FILE *tempFile = fopen("temp_attendance.txt", "w");
    if (!tempFile) {
        perror("Unable to open temporary file for writing");
        fclose(file); 
        return;
    }
    
    char line[MAX_LINE];
    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char studentID[MAX_ID], studentName[MAX_NAME], courseID[MAX_COURSE_ID], attendanceStatus[MAX_ATTENDANCE];
        sscanf(line, "%[^,],%[^,],%[^,],%[^\n]", studentID, studentName, courseID, attendanceStatus);

        if (strcmp(studentID, targetStudentID) == 0 && strcmp(courseID, targetCourseID) == 0) {
            fprintf(tempFile, "%s,%s,%s,%s\n", studentID, studentName, courseID, newAttendanceStatus);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("attendance.txt");
        rename("temp_attendance.txt", "attendance.txt");
        printf("Attendance updated successfully.\n");
    } else {
        printf("No matching attendance record found to update.\n");
    }
}

