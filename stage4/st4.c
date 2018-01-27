struct tnode* createTree(int val,int type, char* c, int nodetype, struct Gsymbol * st, struct tnode *l,struct tnode *m, struct tnode *r){
	struct tnode* t = (struct tnode*)malloc(sizeof(struct tnode));
	t->val = val;
	t->type = type;
	t->varname = c;
	t->nodetype= nodetype;
	t->Gentry=st;
	t->left = l;
	t->middle = m;
	t->right = r;
	return t;
}


struct tnode* createVarNode(char* c){
		return createTree(NULL,NULL, c, tVAR, NULL,NULL, NULL ,NULL);
		
}

struct tnode* createLiteralNode(char* c){
		return createTree(NULL,stringType, c, tLIT, NULL,NULL, NULL ,NULL);
		
}

struct tnode * createTypeNode(int type){
	return createTree(NULL,type, NULL,NULL,NULL,NULL,NULL,NULL);
}

struct tnode * assignTypeDecl(struct tnode *typeNode, struct tnode *varlist){
	int i=10;
	Gsymbol * t = NULL;
	struct tnode *head=varlist;
	while(varlist!=NULL){
		switch(varlist->nodetype){
		
			case tPVAR: varlist->type=typeNode->type + 10;		///MAGIC NUMBER DON'T REARRANGE HEADER
						break;
			default: varlist->type=typeNode->type;
						break;
		}
				//intType, strType
		t = lookupSymbol(varlist->varname);
		i--;
		if(i==0){
			break;
		}
		if(t!=NULL){
			t->type = varlist->type;
		} else {
			break;
		}
		varlist=varlist->middle;
	}
	return head;
}


//p=&q
//*p=*p+1
//a=*p
struct tnode* createAsgNode(struct tnode *l, struct tnode *r){
	printf("%d,%d,%d,%d\n",l->nodetype,r->nodetype,l->type,l->type);
	if((l->nodetype==tPVAR && r->nodetype==tREF) &&
		((l->type == (r->type)))){
			return createTree(NULL,NULL, NULL,tASSIGN,NULL, l, NULL,r);
		}
	else if((l->type == r->type) && (l->nodetype==tVAR||  l->nodetype==tARR || l->nodetype==tDARR || l->nodetype==tPVAR)){//assignment
		return createTree(NULL,NULL, NULL,tASSIGN,NULL, l, NULL,r);
	}else{
		yyerror("Type mismatch - cannot assign \n");
		exit(1);
		}
}



struct tnode* createNumNode(int val){
	return createTree(val,intType, NULL,tNUM, NULL,NULL,NULL,NULL);
}

struct tnode* createOpNode(int nodetype, struct tnode *l, struct tnode *r){
	if ((l->type == intType) && (r->type==intType)){
		switch(nodetype){
			case tADD:
			case tSUB:
			case tMUL:
			case tDIV:
					return createTree(NULL, intType, NULL,nodetype,NULL, l, NULL, r);
					break;
			case tLT:
			case tGT:
			case tLE:
			case tGE:
			case tEQ:
			case tNE:
					return createTree(NULL, boolType, NULL,nodetype,NULL, l, NULL, r);
					break;
			}
		}
	else{
			yyerror("Type mismatch\n");
			exit(1);
		}
}


struct tnode* createReadNode(struct tnode *r){
	return createTree(NULL, NULL ,NULL, tREAD,NULL, NULL, NULL, r);
}

struct tnode* createWriteNode(struct tnode *r){
	if(((r->type) == intType) || ((r->type) == stringType)){
		return createTree(NULL,NULL, NULL, tWRITE,NULL, NULL, NULL, r);
	}else {
		yyerror("Type mismatch- can't write\n");
		exit(1);
		}
}

struct tnode* createIfNode(struct tnode *l, struct tnode *m, struct tnode *r){
	if((l->type) == boolType)
		return createTree(NULL,NULL, NULL, tIF,NULL, l, m, r);
	else{
		yyerror("Type mismatch - guard of if must be a conditional\n");
		exit(1);
		}
}

struct tnode* createWhileNode(struct tnode *l, struct tnode *r){
	if((l->type) == boolType){
		return createTree(NULL,NULL, NULL,tWHILE,NULL, l, NULL,r);
	}else{
		yyerror("Type mismatch - guard of while must be a conditional\n");
		exit(1);
		}
}

struct tnode* createDoWhileNode(struct tnode *l, struct tnode *r){
	if((r->type) == boolType){
		return createTree(NULL,NULL, NULL,tDOWHILE,NULL, l, NULL,r);
	}else{
		yyerror("Type mismatch - guard of do while must be a conditional\n");
		exit(1);
		}
}

struct tnode* createRepeatNode(struct tnode *l, struct tnode *r){
	if((r->type) == boolType){
		return createTree(NULL,NULL, NULL,tREPEAT,NULL, l, NULL,r);
	}else{
		yyerror("Type mismatch - guard of repeat must be a conditional\n");
		exit(1);
		}
}
struct tnode* createBreakNode(){
	return createTree(NULL, NULL ,NULL, tBREAK,NULL, NULL, NULL, NULL);
}
struct tnode* createContinueNode(){
	return createTree(NULL, NULL ,NULL, tCONTINUE,NULL, NULL, NULL, NULL);
}



