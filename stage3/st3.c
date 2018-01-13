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
	if((r->type) == intType){
		return createTree(NULL,NULL, NULL,tASSIGN, l, NULL,r);
	}else{
		yyerror("Type mismatch - cannot assign boolean values \n");
		exit(1);
		}
}
struct tnode* createReadNode(struct tnode *r){
	return createTree(NULL, NULL ,NULL, tREAD, NULL, NULL, r);
}
struct tnode* createWriteNode(struct tnode *r){
	if((r->type) == intType){
		return createTree(NULL,NULL, NULL, tWRITE, NULL, NULL, r);
	}else{
		yyerror("Type mismatch- can't print boolean expressions\n");
		exit(1);
		}
}

struct tnode* createIfNode(struct tnode *l, struct tnode *m, struct tnode *r){
	if((l->type) == boolType)
		return createTree(NULL,NULL, NULL, tIF, l, m, r);
	else{
		yyerror("Type mismatch - guard of if must be a conditional\n");
		exit(1);
		}
}

struct tnode* createWhileNode(struct tnode *l, struct tnode *r){
	if((l->type) == boolType){
		return createTree(NULL,NULL, NULL,tWHILE, l, NULL,r);
	}else{
		yyerror("Type mismatch - guard of while must be a conditional\n");
		exit(1);
		}
}

int getLabel(){
	return label++;
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
		case tWHILE:
				p=eval(t->left);
				while(p==1){
					eval(t->right);
					p = eval(t->left);
				}
				return -1;
				break;
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
	int loc, reg,p,q;
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
		case tIF:{
			int label_1 = getLabel();
			int label_2 = getLabel();
			p=codeGen(t->left,fp);	//the expr eval is here
			fprintf (fp, "JZ R%d, L%d\n", p, label_1); // GOTO ELSE
			codeGen(t->middle,fp);
			if(t->right==NULL){
				fprintf (fp, "L%d:\n", label_1);
				return -1;
			}else{
			fprintf (fp, "JMP L%d\n", label_2);
			//ELSE
			fprintf (fp, "L%d:\n", label_1);			
			codeGen(t->right,fp);
			fprintf (fp, "L%d:\n", label_2);}
			return -1;
			}
		case tWHILE:{
			int label_1 = getLabel();
			int label_2 = getLabel();
			fprintf (fp, "L%d:\n", label_1); // Place the first label here.
			p=codeGen(t->left,fp);
			fprintf (fp, "JZ R%d, L%d\n", p, label_2);//if zero, jump to label_2 // loop exit
			codeGen(t->right,fp);
			fprintf(fp, "JMP L%d\n", label_1); // return to the beginning of the loop.
			fprintf(fp, "L%d:\n", label_2); // Place the second label here
			return -1;
			}
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
				case tLT:
					fprintf(fp,"LT R%d, R%d\n",reg,loc);
					break;
				case tGT:
					fprintf(fp,"GT R%d, R%d\n",reg,loc);
					break;
				case tLE:
					fprintf(fp,"LE R%d, R%d\n",reg,loc);
					break;
				case tGE:
					fprintf(fp,"GE R%d, R%d\n",reg,loc);
					break;
				case tEQ:
					fprintf(fp,"EQ R%d, R%d\n",reg,loc);
					break;
				case tNE:
					fprintf(fp,"NE R%d, R%d\n",reg,loc);
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
	//issue with lib
	fprintf(fp,"PUSH R0\nMOV R0,R%d\n",reg);
	fprintf(fp,"MOV R%d, \"Write\"\nPUSH R%d\nMOV R%d, -2\nPUSH R%d\nPUSH R%d\nPUSH R0\nPUSH R0\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",temp,temp,temp,temp,0,temp,temp,temp,temp,temp);
	//return val - success/fail in Rtemp?
	fprintf(fp,"POP R0\n",reg);
	freeReg();
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
	}else if(t->nodetype == tWHILE){
		printf("while is true\n");
		printTree(t->left);
		printf("do this");
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
		case tEQ: printf("== ");
					break;
		}
	printTree(t->right);
	}
}

