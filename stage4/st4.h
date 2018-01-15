#define tNUM 0
#define tADD 1
#define tSUB 2
#define tMUL 3
#define tDIV 4
#define tREAD 5
#define tWRITE 6
#define tASSIGN 7
#define tVAR 8
#define tCONNECT 9
#define tLT 10
#define tGT 11
#define tLE 12
#define tGE 13
#define tEQ 14
#define tNE 15
#define tIF 16
#define tWHILE 17
#define tBREAK 18
#define tCONTINUE 19
#define tDOWHILE 20
#define intType 21
#define boolType 22
#define stringType 23
#define tREPEAT 24
#define heapBase 4096
#define tDCONNECT 25
#define tDECL 26
#define tVCONNECT 27

typedef struct Gsymbol {
	char* name;	// name of the variable
	int type;	// type of the variable
	int size;	// size of the type of the variable
	int binding;	// stores the static memory address allocated to the variable
	struct Gsymbol *next;
}	Gsymbol;

int reg;
int label;
int memory[26];
struct StackNode* breakstack;
struct StackNode* contstack;
Gsymbol * symtable = NULL;
int heapSize = 4096;
char * typeToString(int type);

struct Gsymbol *lookup(char * name); // Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
void install(char *name, int type, int size, int binding); // Creates a symbol table entry.


// A structure to represent a stack
struct StackNode
{
    int data;
    struct StackNode* next;
};
typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char* varname;	//name of a variable for ID nodes  
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc. 
	struct Gsymbol *Gentry; //pointerto GST entry for global variables and functions 
	struct tnode *left,*middle,*right;	//left and right branches   
}tnode;

/*Create a node tnode*/
struct tnode* createTree(int val,int type, char* c, int nodetype, struct tnode *l,struct tnode *m, struct tnode *r);
struct tnode* createNumNode(int val);
struct tnode* createOpNode(int nodetype, struct tnode *l, struct tnode *r);
struct tnode* createVarNode(char* c);
struct tnode* createAsgNode(struct tnode *l, struct tnode *r);
struct tnode* createReadNode(struct tnode *r);
struct tnode* createWriteNode(struct tnode *r);
struct tnode* createIfNode(struct tnode *l, struct tnode *m, struct tnode *r);
struct tnode* createWhileNode(struct tnode *l, struct tnode *r);
struct tnode* createBreakNode();
struct tnode* createContinueNode();
struct tnode* createDoWhileNode(struct tnode *l, struct tnode *r);
struct tnode* createRepeatNode(struct tnode *l, struct tnode *r);
void printTree(struct tnode* t);
