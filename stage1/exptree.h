typedef struct tnode{
	int flag;	//leaf or operator?
	int val; 	//value of leaf
	char *op; //indicates the opertor branch
	struct tnode *left,*right; //left and right branches
} tnode;

/*Make a leaf tnode and set the value of val field*/
struct tnode* makeLeafNode(int n);


int reg;

	
/*Make a tnode with opertor, left and right branches set*/
struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r);

/*To print prefix notation of expression tree*/
void prefix(struct tnode *t);

/*Toprint postfix notation of expression tree*/
void postfix(struct tnode *t);

int getReg();
void freeReg();

int codeGen(struct tnode *t, FILE * fp);

void write_xsm(char * s, FILE * fp);

/*To evaluate an expression tree*/
int evaluate(struct tnode *t);
