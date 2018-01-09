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
#define varLoc 4096

int reg;
int memory[26];

typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char varname;	//name of a variable for ID nodes  
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.  
	struct tnode *left,*right;	//left and right branches   
}tnode;

/*Create a node tnode*/
struct tnode* createTree(int val, int nodetype, char c, struct tnode *l, struct tnode *r);
struct tnode* createNumNode(int val);
struct tnode* createOpNode(char c, struct tnode *l, struct tnode *r);
struct tnode* createVarNode(char c);
struct tnode* createAsgNode(struct tnode *l, struct tnode *r);
struct tnode* createReadNode(struct tnode *r);
struct tnode* createWriteNode(struct tnode *r);

void printTree(struct tnode* t);
