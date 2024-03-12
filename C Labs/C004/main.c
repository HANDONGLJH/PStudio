#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct st_class {
  int code;      // class code
  char name[30]; // class name
  int unit;      // credits
  int grading;   // grading (1:A+~F, 2:P/F)
};

char kname[2][10] = {"A+~F", "P/F"}; // String for grading

// 1 ok

int loadData(struct st_class *c[]) {
  int count = 0;
  FILE *file;

  file = fopen("classes.txt", "r");
  while (!feof(file)) {
    c[count] = (struct st_class *)malloc(sizeof(struct st_class));
    int r = fscanf(file, "%d %s %d %d", &(c[count]->code), c[count]->name,
                   &(c[count]->unit), &(c[count]->grading));
    if (r < 4)
      break;
    count++;
  }
  fclose(file);
  return count;
}

// ok
void printAllClasses(struct st_class *c[], int csize) {
  for (int i = 0; i < csize; i++) {
    printf("[%d] %s [credit %d - %s]\n", c[i]->code, c[i]->name, c[i]->unit,
           kname[c[i]->grading - 1]);
  }
}

// 7 ok
void saveAllClasses(struct st_class *c[], int csize) {
  FILE *file;
  file = fopen("classes.txt", "w");
  for (int i = 0; i < csize; i++) {
    fprintf(file, "%d %s %d %d\n", c[i]->code, c[i]->name, c[i]->unit,
            c[i]->grading);
  }
  fclose(file);
}

// 4  ok
void findClasses(char *name, struct st_class *c[], int csize) {
  int count = 0;
  printf("Searching (keyword: %s)\n", name);
  for (int i = 0; i < csize; i++) {
    if (strstr(c[i]->name, name)) {
      printf("[%d] %s [credit %d - %s]\n", c[i]->code, c[i]->name, c[i]->unit,
             kname[c[i]->grading - 1]);
      count++;
    }
  }
  printf("%d classes found.\n", count);
}

/////////////////////////////////////////////////////////////////////////////////

// 2 
int addNewClass(struct st_class *c[], int csize) {
  struct st_class *p = (struct st_class *)malloc(sizeof(struct st_class));

  printf(">> code number > ");
  scanf("%d", &(p->code));


  // Check if the code already exists
  for (int i = 0; i < csize; i++) {
    if (c[i]->code == p->code) {
      printf("Class with this code already exists. Cannot add.\n");
      return csize;
    }
  }

  
  printf(">> class name > ");
  scanf("%s", p->name);
  printf(">> credits > ");
  scanf("%d", &(p->unit));
  printf(">> grading (1: A+~F, 2: P/F) > ");
  scanf("%d", &(p->grading));

  c[csize] = p;
  return csize + 1;
}

// 3 
void editClass(struct st_class *c[], int csize) {
  struct st_class* p;
  int code;
  printf(">> Enter a code of class > ");
  scanf("%d", &code);


  /////////////
  int found = 0;
  for (int i = 0; i < csize; i++) {
    if (c[i]->code == code) {
      found = 1;
      printf("> Current: [%d] %s [credits %d - %s]\n", c[i]->code, c[i]->name,
             c[i]->unit, kname[c[i]->grading - 1]);
      printf("> Enter new class name > ");
      scanf("%s", c[i]->name);
      printf("> Enter new credits > ");
      scanf("%d", &(c[i]->unit));
      printf("> Enter new grading(1: Grade, 2: P/F) > ");
      scanf("%d", &(c[i]->grading));
      printf("> Modified.\n");
      break;
    }
  }

  if (!found) {
    printf("Class with code %d not found. Cannot edit.\n", code);
  }
}

// 5 
int applyMyClasses(int my[], int msize, struct st_class *c[], int csize) {
  int code;
  printf("Enter the class code to apply (0 to finish): ");

  while (1) {
    scanf("%d", &code);
    if (code == 0) {
      break;
    }

    int exists = 0;
    for (int i = 0; i < csize; i++) {
      if (c[i]->code == code) {
        exists = 1;
        my[msize] = code;
        msize++;
        printf("Class %s applied.\n", c[i]->name);
        break;
      }
    }

    if (!exists) {
      printf("Class with code %d does not exist. Cannot apply.\n", code);
    }
  }

  return msize;
}

// 6 
void printMyClasses(int my[], int msize, struct st_class *c[], int csize) {
  printf("My classes:\n");
  for (int i = 0; i < msize; i++) {
    for (int j = 0; j < csize; j++) {
      if (my[i] == c[j]->code) {
        printf("[%d] %s [credit %d - %s]\n", c[j]->code, c[j]->name, c[j]->unit,
               kname[c[j]->grading - 1]);
      }
    }
  }
}

// 7
void saveMyClass(int my[], int msize, struct st_class *c[], int csize) {
  FILE *file;
  file = fopen("my_classes.txt", "w");

  int total_credits = 0;
  int total_classes = msize;
  int credits_ApF = 0;
  int credits_PassF = 0;

  for (int i = 0; i < msize; i++) {
    for (int j = 0; j < csize; j++) {
      if (my[i] == c[j]->code) {
        fprintf(file, "%d %s %d %d\n", c[j]->code, c[j]->name, c[j]->unit,
                c[j]->grading);
        total_credits += c[j]->unit;
        if (c[j]->grading == 1) {
          credits_ApF += c[j]->unit;
        } else if (c[j]->grading == 2) {
          credits_PassF += c[j]->unit;
        }
        break;
      }
    }
  }

  fprintf(file, "Total credits: %d\n", total_credits);
  fprintf(file, "Total classes: %d\n", total_classes);
  fprintf(file, "Total credits for A+~F: %d\n", credits_ApF);
  fprintf(file, "Total credits for P/F: %d\n", credits_PassF);

  fclose(file);
}

////////////////////////////////////////////////////////////////////////////////

int main(void) {
  int no;
  struct st_class *classes[50];
  int myclass[10];
  int mycount = 0;
  srand(time(0));
  int count = loadData(classes);
  printf("> Load %d classes.\n", count);

  while (1) {
    printf("\n> Menu 1.List 2.Add 3.Modify 4.Search 5.Apply 6.My classes "
           "7.Save 0.Quit\n");
    printf(">> Menu? > ");
    scanf("%d", &no);

    if (no == 1) {
      printf("> 1.Class list (%d classes)\n", count);
      printAllClasses(classes, count);
    } else if (no == 2) {
      printf("> 2.Add a Class\n");
      count = addNewClass(classes, count);
    } else if (no == 3) {
      printf("> 3.Modify a Class\n");
      editClass(classes, count);
    } else if (no == 4) {
      printf("> 4.Search a Class\n");
      printf(">> Enter class name > ");
      char name[30];
      scanf("%s", name);
      findClasses(name, classes, count);
    } else if (no == 5) {
      printf("> 5.Apply a class\n");
      mycount = applyMyClasses(myclass, mycount, classes, count);
      printf("%d classes has been applied.\n", mycount);
    } else if (no == 6) {
      printf("> 6.My classes\n");
      printMyClasses(myclass, mycount, classes, count);
    } else if (no == 7) {
      printf("> 7.Save\n");
      saveMyClass(myclass, mycount, classes, count);
      printf("\n> All my classes ware saved to my_classes.txt.\n");
      saveAllClasses(classes, count);
      printf("\n> All of class list ware saved to classes.txt.\n");
    } else
      break;
  }
  return 0;
}
