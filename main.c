#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOTICES 100
#define MAX_STUDENTS 100
#define MAX_DEPARTMENTS 10

typedef struct {
    int id;
    char title[100];
    char *body;
    char department[50];
    int semester;
    char poster[50];
    char date[11];
} Notice;

typedef struct {
    char studentID[20];
    char departments[MAX_DEPARTMENTS][50];
    int count;
} Student;

Notice *notices;
Student students[MAX_STUDENTS];

int noticeCount = 0, studentCount = 0;

void readLine(char *str, int size) {
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

void postNotice() {
    if (noticeCount >= MAX_NOTICES) {
        printf("Noticeboard is full!\n");
        return;
    }

    Notice *n = &notices[noticeCount];

    n->id = noticeCount + 1;

    printf("Enter title: ");
    readLine(n->title, sizeof(n->title));

    char body[2000];
    printf("Enter notice body: ");
    readLine(body, sizeof(body));

    n->body = malloc(strlen(body) + 1);

    if (n->body == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(n->body, body);

    printf("Enter department: ");
    readLine(n->department, sizeof(n->department));

    printf("Enter semester: ");
    scanf("%d", &n->semester);
    getchar();

    printf("Enter poster name: ");
    readLine(n->poster, sizeof(n->poster));

    printf("Enter date (YYYY-MM-DD): ");
    readLine(n->date, sizeof(n->date));

    noticeCount++;
    printf("Notice posted successfully! ID: %d\n", n->id);
}

void displayNotice(Notice n) {
    printf("\nNotice ID: %d\n", n.id);
    printf("Title: %s\n", n.title);
    printf("Body: %s\n", n.body);
    printf("Department: %s\n", n.department);
    printf("Semester: %d\n", n.semester);
    printf("Posted by: %s\n", n.poster);
    printf("Date: %s\n", n.date);
    printf("-----------------------------\n");
}

void viewNotices() {
    if (noticeCount == 0) {
        printf("No notices available.\n");
        return;
    }

    for (int i = 0; i < noticeCount; i++) {
        displayNotice(notices[i]);
    }
}

void editNotice() {
    int id;
    printf("Enter notice ID to edit: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < noticeCount; i++) {
        if (notices[i].id == id) {
            char body[2000];

            printf("Enter new title: ");
            readLine(notices[i].title, sizeof(notices[i].title));

            printf("Enter new body: ");
            readLine(body, sizeof(body));

            char *newBody = malloc(strlen(body) + 1);

            if (newBody == NULL) {
                printf("Memory allocation failed!\n");
                return;
            }

            strcpy(newBody, body);
            free(notices[i].body);
            notices[i].body = newBody;

            printf("Enter new department: ");
            readLine(notices[i].department,
                     sizeof(notices[i].department));

            printf("Enter new semester: ");
            scanf("%d", &notices[i].semester);
            getchar();

            printf("Enter new poster name: ");
            readLine(notices[i].poster, sizeof(notices[i].poster));

            printf("Enter new date (YYYY-MM-DD): ");
            readLine(notices[i].date, sizeof(notices[i].date));

            printf("Notice updated successfully!\n");
            return;
        }
    }

    printf("Notice not found.\n");
}

void deleteNotice() {
    int id;
    printf("Enter notice ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < noticeCount; i++) {
        if (notices[i].id == id) {
            free(notices[i].body);

            for (int j = i; j < noticeCount - 1; j++) {
                notices[j] = notices[j + 1];
            }

            noticeCount--;
            printf("Notice deleted successfully!\n");
            return;
        }
    }

    printf("Notice not found.\n");
}

void searchNotice() {
    char keyword[100];
    int found = 0;

    printf("Enter title keyword: ");
    readLine(keyword, sizeof(keyword));

    for (int i = 0; i < noticeCount; i++) {
        if (strstr(notices[i].title, keyword) != NULL) {
            displayNotice(notices[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No matching notices found.\n");
}

void filterNotices() {
    int choice, semester, found = 0;
    char department[50], startDate[11], endDate[11];

    printf("\n1. Filter by department\n");
    printf("2. Filter by semester\n");
    printf("3. Filter by date range\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        printf("Enter department: ");
        readLine(department, sizeof(department));
    } else if (choice == 2) {
        printf("Enter semester: ");
        scanf("%d", &semester);
        getchar();
    } else if (choice == 3) {
        printf("Enter start date (YYYY-MM-DD): ");
        readLine(startDate, sizeof(startDate));

        printf("Enter end date (YYYY-MM-DD): ");
        readLine(endDate, sizeof(endDate));
    } else {
        printf("Invalid choice.\n");
        return;
    }

    for (int i = 0; i < noticeCount; i++) {
        int match = 0;

        if (choice == 1 &&
            strcmp(notices[i].department, department) == 0)
            match = 1;

        if (choice == 2 && notices[i].semester == semester)
            match = 1;

        if (choice == 3 &&
            strcmp(notices[i].date, startDate) >= 0 &&
            strcmp(notices[i].date, endDate) <= 0)
            match = 1;

        if (match) {
            displayNotice(notices[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No matching notices found.\n");
}

void subscribeDepartment() {
    char id[20], department[50];
    int index = -1;

    printf("Enter student ID: ");
    readLine(id, sizeof(id));

    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].studentID, id) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        if (studentCount >= MAX_STUDENTS) {
            printf("Student limit reached!\n");
            return;
        }

        index = studentCount++;
        strcpy(students[index].studentID, id);
        students[index].count = 0;
    }

    printf("Enter department to subscribe to: ");
    readLine(department, sizeof(department));

    if (students[index].count >= MAX_DEPARTMENTS) {
        printf("Subscription limit reached!\n");
        return;
    }

    for (int i = 0; i < students[index].count; i++) {
        if (strcmp(students[index].departments[i], department) == 0) {
            printf("Already subscribed to this department.\n");
            return;
        }
    }

    strcpy(students[index].departments[students[index].count],
           department);
    students[index].count++;

    printf("Subscription successful!\n");
}

void showNotifications() {
    char id[20];
    int foundStudent = 0, foundNotice = 0;

    printf("Enter student ID: ");
    readLine(id, sizeof(id));

    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].studentID, id) == 0) {
            foundStudent = 1;

            printf("\nNotifications for student %s:\n", id);

            for (int j = 0; j < noticeCount; j++) {
                for (int k = 0; k < students[i].count; k++) {
                    if (strcmp(notices[j].department,
                               students[i].departments[k]) == 0) {
                        displayNotice(notices[j]);
                        foundNotice = 1;
                        break;
                    }
                }
            }
            break;
        }
    }

    if (!foundStudent)
        printf("Student has no subscriptions.\n");
    else if (!foundNotice)
        printf("No relevant notifications available.\n");
}

int main() {
    int choice;

    notices = malloc(MAX_NOTICES * sizeof(Notice));

    if (notices == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    do {
        printf("\n===== CAMPUS NOTICEBOARD SYSTEM =====\n");
        printf("1. Post Notice\n");
        printf("2. View All Notices\n");
        printf("3. Edit Notice\n");
        printf("4. Delete Notice\n");
        printf("5. Search Notice\n");
        printf("6. Filter Notices\n");
        printf("7. Subscribe to Department\n");
        printf("8. View Notifications\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }
        getchar();

        switch (choice) {
            case 1: postNotice(); break;
            case 2: viewNotices(); break;
            case 3: editNotice(); break;
            case 4: deleteNotice(); break;
            case 5: searchNotice(); break;
            case 6: filterNotices(); break;
            case 7: subscribeDepartment(); break;
            case 8: showNotifications(); break;
            case 0: printf("Exiting system...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 0);

    for (int i = 0; i < noticeCount; i++) {
        free(notices[i].body);
    }

    free(notices);

    printf("Memory released. Goodbye!\n");
    return 0;
}