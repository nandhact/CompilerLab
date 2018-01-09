struct tnode* makeLeafNode(int n){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->flag = 0;
    temp->op = NULL;
    temp->val = n;
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

struct tnode* makeOperatorNode(char c,struct tnode *l,struct tnode *r){
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->flag = 1;
    temp->op = malloc(sizeof(char));
    *(temp->op) = c;
    temp->left = l;
    temp->right = r;
    return temp;
}
void prefix(struct tnode *t){

	if(t->flag == 0){
		printf(" %d", t->val);
	} else {
		printf(" %c",*t->op);
		prefix(t->left);
		prefix(t->right);
	}

}


void postfix(struct tnode *t){
if(t->flag == 0){
		printf(" %d", t->val);
	} else {
		
		postfix(t->left);
		postfix(t->right);
		printf(" %c",*t->op);
	}


}
int getReg(){
	if(reg>19) {
		printf("\nOut of registers\n");
		exit(0);
	}
	else {
	int t=reg;
	reg++;
	return t;
	}
}

void freeReg(){

	if(reg>0) {
		reg--;
	}
}

int codeGen(struct tnode *t, FILE *fp){
	
	if(t->flag ==0) {
		int r;
		r = getReg();
		fprintf(fp,"MOV R%d, %d\n", r, t->val);
		return r;
	} else {
		int p,q;
		p = codeGen(t->left, fp);
		q = codeGen(t->right, fp);
		switch (*(t->op)){
		case '+':
			fprintf(fp,"ADD R%d, R%d\n",p,q);
			break;
		case '-':
			fprintf(fp,"SUB R%d, R%d\n",p,q);
			break;
		case '*':
			fprintf(fp,"MUL R%d, R%d\n",p,q);
			break;
		case '/':
			fprintf(fp,"DIV R%d, R%d\n",p,q);
			break;
		
		}
		
		freeReg();
		return p;
	
	}

}

void write_xsm(char * s, FILE * fp){
fprintf(fp,"MOV SP, 4096\nMOV R1, \"Write\"\nPUSH R1\nMOV R1, -2\nPUSH R1\nPUSH %s\nPUSH R1\nPUSH R1\nCALL 0\nPOP R0\nPOP R1\nPOP R1\nPOP R1\nPOP R1\n", s);
}

int evaluate(struct tnode *t){
    if(t->flag == 0){
    	int temp = t->val;
    	//free(t);
        return temp;
    }
    else{
        switch(*(t->op)){
            case '+' : return evaluate(t->left) + evaluate(t->right);
                       break;
            case '-' : return evaluate(t->left) - evaluate(t->right);
                       break;
            case '*' : return evaluate(t->left) * evaluate(t->right);
                       break;
            case '/' : return evaluate(t->left) / evaluate(t->right);
                       break;
        }
    }
}

