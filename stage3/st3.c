struct tnode* createTree(int val,int type, char c, int nodetype, struct tnode *l,struct tnode *m, struct tnode *r){
	struct tnode* t = (struct tnode*)malloc(sizeof(struct tnode));
	t->val = val;
	t->type = type;
	t->varname = c;
	t->nodetype= nodetype;
	t->left = l;
	t->middle = m;
	t->right = r;
	return t;
}

struct tnode* createNumNode(int val){
	return createTree(val,intType, NULL,tNUM, NULL,NULL,NULL);
}

struct tnode* createOpNode(int nodetype, struct tnode *l, struct tnode *r){
	if ((l->type == intType) && (r->type==intType)){
		switch(nodetype){
			case tADD:
			case tSUB:
			case tMUL:
			case tDIV:
					return createTree(NULL, intType, NULL,nodetype, l, NULL, r);
					break;
			case tLT:
			case tGT:
			case tLE:
			case tGE:
			case tEQ:
			case tNE:
					return createTree(NULL, boolType, NULL,nodetype, l, NULL, r);
					break;
			}
		}
	else{
			yyerror("Type mismatch\n");
			exit(1);
		}
}

struct tnode* createVarNode(char c){
	return createTree(NULL,intType, c, tVAR, NULL, NULL,NULL);
}
struct tnode* createAsgNode(struct tnode *l, struct tnode *r){
	return createTree(NULL,NULL, NULL,tASSIGN, l, NULL,r);
}
struct tnode* createReadNode(struct tnode *r){
	return createTree(NULL, NULL ,NULL, tREAD, NULL, NULL, r);
}
struct tnode* createWriteNode(struct tnode *r){
	return createTree(NULL,NULL, NULL, tWRITE, NULL, NULL, r);
}

struct tnode* createIfNode(struct tnode *l, struct tnode *m, struct tnode *r){
	if((l->type) == boolType)
		return createTree(NULL,NULL, NULL, tIF, l, m, r);
	else{
		yyerror("Type mismatch\n");
		exit(1);
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
	
	int loc, reg,p,q;
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
		case tIF:
				p=eval(t->left);
				if(p){
				return eval(t->middle);
				} else {
				return eval(t->right);
				}
		default:
			p = eval(t->left);
			q = eval(t->right);
			switch (t->nodetype){
				case tADD:
					p=p+q;
					break;
				case tSUB:
					p=p-q;
					break;
				case tMUL:
					p=p*q;
					break;
				case tDIV:
					p=p/q;
					break;
				case tLT:
					p=(p<q);
					break;
				case tGT:
					p=(p>q);
					break;
				case tLE:
					p=(p<=q);
					break;
				case tGE:
					p=(p>=q);
					break;
				case tEQ:
					p=(p==q);
					break;
				case tNE:
					p=(p!=q);
					break;
			}
			return p;
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


