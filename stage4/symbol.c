

void insertSymbol(char *name, int type,int nodetype, int size0,int size1, int binding){
	 Gsymbol* symEntry =
              (Gsymbol*) malloc(sizeof(Gsymbol));
    symEntry->name = strdup(name);
    symEntry->type = type; 	//str or int
    symEntry->nodetype = nodetype;
    symEntry->size[0]=size0;
    symEntry->size[1]=size1;
    symEntry->binding = binding;
    symEntry->next = symtable;
    symtable = symEntry;
    return;
}

Gsymbol * lookupSymbol(char * name){
    Gsymbol * current = symtable;  // Initialize current
    while (current != NULL)
    {	
        if (strcmp(name,current->name)==0){
            return current;}
        current = current->next;
    }
    return NULL;
}

void showST(){
	struct Gsymbol* current = symtable;  // Initialize current
	printf("------------Symbol Table-------------------\n");
    while (current != NULL)
    {
        printf("Name:%s Size:%d,%d Type:%s ,Nodetype: %d,Binding:%d\n",current->name,current->size[0],current->size[1] ,typeToString(current->type), (current->nodetype), current->binding);
        current = current->next;
    }
}


char * typeToString(int type){
	switch(type){
		case intType: return "Integer";
			break;
		case stringType: return "String";
			break;
		default: return "Void";
			break;
	}

}
