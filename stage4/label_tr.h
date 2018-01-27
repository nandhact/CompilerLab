#define SIZE 20

typedef struct labelItem {
   int label;   
   int address;
} labelItem;


struct labelItem* hashArray[SIZE];
void incLine();
void fstrcpy(FILE *fp, char* s);
