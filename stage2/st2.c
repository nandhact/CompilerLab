struct tnode* createTree(int val, int nodetype, char c, struct tnode *l, struct tnode *r){
	struct tnode* t = (struct tnode*)malloc(sizeof(struct tnode));
	t->val = val;
	t->nodetype= nodetype;
	t->varname = c;
	t->left = l;
	t->right = r;
	return t;
}

struct tnode* createNumNode(int val){
	return createTree(val,tNUM, NULL, NULL, NULL);
}

struct tnode* createOpNode(char c, struct tnode *l, struct tnode *r){
	int tOP =0;
	switch (c){
	case '+': tOP=tADD;
		break;
	case '-': tOP=tSUB;
		break;
	case '*': tOP=tMUL;
		break;
	case '/': tOP=tDIV;
		break;
	}
	return createTree(NULL, tOP, NULL,l, r);
}

struct tnode* createVarNode(char c){
	return createTree(NULL,tVAR, c, NULL, NULL);
}

struct tnode* createAsgNode(struct tnode *l, struct tnode *r){
	return createTree(NULL, tASSIGN, NULL, l, r);
}
struct tnode* createReadNode(struct tnode *r){
	return createTree(NULL, tREAD, NULL, NULL, r);
}
struct tnode* createWriteNode(struct tnode *r){
	return createTree(NULL, tWRITE, NULL, NULL, r);
}

void printTree(struct tnode* t){
	if(t->nodetype == tNUM){
		printf("%d ",t->val);
	}else if(t->nodetype == tVAR){
		
		printf("var %c ",t->varname);
	}else if(t->nodetype == tCONNECT){
		printTree(t->left);
		printf(";\n");
		printTree(t->right);
	}else if(t->nodetype == tREAD){
		printf("READ ");
		printTree(t->right);
	}else if(t->nodetype == tWRITE){
		printf("WRITE ");
		printTree(t->right);
	}else if(t->nodetype == tASSIGN){
		printTree(t->left);
		printf("= ");
		printTree(t->right);
	}else{
	printTree(t->left);
	switch(t->nodetype){
		case tADD: printf("+ ");
					break;
		case tSUB: printf("- ");
					break;
		case tMUL: printf("* ");
					break;
		case tDIV: printf("/ ");
					break;
		}
	printTree(t->right);
	}
}



int getReg(){
	if(reg>19) {
		printf("\nOut of registers\n");
		exit(0);
	}
	else {
	reg++;
	return reg-1;
	}
}

void freeReg(){

	if(reg>0) {
		reg--;
	}
}

void freeAllReg(){
	reg=0;
}

int eval(struct tnode *t){
	
	int loc, reg;
	switch((t->nodetype)){
		case tNUM:
				return t->val;
		case tVAR:
				loc = t->varname-'a';
				return memory[loc];
		case tREAD:
				loc = t->right->varname-'a';
				//read into location
				printf("input:",reg);
				scanf("%d",&reg);
				memory[loc] = reg;
				return -1;
		case tWRITE:
				printf("%d\n",eval(t->right));
				return -1;
		case tCONNECT:
				eval(t->left);
				eval(t->right);
				return -1;
		case tASSIGN:
				reg = eval(t->right);
				loc = t->left->varname-'a';
				memory[loc] = reg;
				return -1;
		default:
			reg = eval(t->left);
			loc = eval(t->right);
			switch (t->nodetype){
				case tADD:
					reg=reg+loc;
					break;
				case tSUB:
					reg=reg-loc;
					break;
				case tMUL:
					reg=reg*loc;
					break;
				case tDIV:
					reg=reg/loc;
					break;
			}
			return reg;
		}

}

int codeGen(struct tnode* t,FILE *fp){
int loc, reg;
	switch((t->nodetype)){
		case tNUM:
				reg = getReg();
				fprintf(fp,"MOV R%d, %d\n", reg, t->val);
				return reg;
		case tVAR:
				reg = getReg();
				loc = varLoc-'a'+t->varname;
				fprintf(fp,"MOV R%d, [%d]\n", reg, loc);
				return reg;
		case tREAD:
				loc =  varLoc-'a'+t->right->varname;
				//read into location
				readCodeGen(loc,fp);
				return -1;
		case tWRITE:
				reg = codeGen(t->right,fp);
				writeCodeGen(reg,fp);
				return -1;
		case tCONNECT:
				codeGen(t->left,fp);
				freeAllReg();
				codeGen(t->right,fp);
				freeAllReg();
				return -1;
		case tASSIGN:
				reg = codeGen(t->right,fp);
				loc =  varLoc-'a'+t->left->varname;
				fprintf(fp,"MOV \[%d\], R%d\n",loc,reg);
				return -1;
		default:
			reg = codeGen(t->left, fp);
			loc = codeGen(t->right, fp);	//can free thiss
			switch (t->nodetype){
				case tADD:
					fprintf(fp,"ADD R%d, R%d\n",reg,loc);
					break;
				case tSUB:
					fprintf(fp,"SUB R%d, R%d\n",reg,loc);
					break;
				case tMUL:
					fprintf(fp,"MUL R%d, R%d\n",reg,loc);
					break;
				case tDIV:
					fprintf(fp,"DIV R%d, R%d\n",reg,loc);
					break;
			}
			freeReg();
			return reg;
		}
	
}

void readCodeGen(int location, FILE *fp){
	int temp = getReg();
	fprintf(fp,"MOV R%d, \"Read\"\nPUSH R%d\nMOV R%d, -1\nPUSH R%d\nMOV R%d, %d\nPUSH R%d\nPUSH R0\nPUSH R0\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",temp,temp,temp,temp,temp,location,temp,temp,temp,temp,temp,temp);
	//return val - success/fail in Rtemp?
	freeReg();
}

void writeCodeGen(int reg, FILE *fp){
	int temp = getReg();
	fprintf(fp,"MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nPUSH R%d\nPUSH R0\nPUSH R0\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",temp,temp,temp,temp,reg,temp,temp,temp,temp,temp);
	//return val - success/fail in Rtemp?
	freeReg();
}


