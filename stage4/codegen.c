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

int getHeapSpace(int size){
	heapSize+=size;
	return heapSize-size;
}

int getLocReg(struct tnode* t,FILE *fp){
	
	int loc = t->Gentry->binding;
	
	if(t->Gentry!=NULL){
		switch(t->nodetype){
		case tREF:{
			int reg = getReg();
			fprintf(fp,"MOV R%d, %d\n",reg,loc);
			return reg;
		}
		case tPVAR:
		case tVAR:{ 
				int reg = getReg();
				fprintf(fp,"MOV R%d, %d\n",reg,loc);
				return reg;
				}
		case tARR:{
				//this will geta new register anyway
				int offsetReg = codeGen(t->middle,fp);
				fprintf(fp,"ADD R%d, %d\n",offsetReg,loc);
				return offsetReg;
				}
			
		case tDARR:{
					int offsetReg = codeGen(t->middle,fp);
					int  offsetCol = codeGen(t->right,fp);
					//mul size[0] with row, add col
					fprintf(fp,"MUL R%d, %d\n",offsetReg, t->Gentry->size[0]);
					//now add offsetCol to offsetReg
					fprintf(fp,"ADD R%d, R%d\n", offsetReg, offsetCol);
					fprintf(fp,"ADD R%d, %d\n",offsetReg,loc);
					return offsetReg;
		}
		default: printf("getLoc failed %d\n",t->nodetype);	
		}
	} else {
		printf("No binding in ST\n");
	}
}

int codeGen(struct tnode* t,FILE *fp){
	int loc, reg,p,q;
	switch((t->nodetype)){
		case tLIT:
				reg = getReg();
				fprintf(fp,"MOV R%d, %s\n", reg, t->varname);
				return reg;
		case tNUM:
				reg = getReg();
				fprintf(fp,"MOV R%d, %d\n", reg, t->val);
				return reg;
		case tVAR://gets the value in a reg
		case tPVAR:
		case tARR:
		case tDARR:
				reg = getReg();
				loc = getLocReg(t,fp);
				fprintf(fp,"MOV R%d, [R%d]\n", reg, loc);
				return reg;
		
		case tREF://p=&q;
				//reg has the location in it
				loc = getLocReg(t,fp);
				return loc;
		case tDEREF://q=*p;
					//*p=q+1;
				//value in p is the address who's value we need
				reg=getReg();
				t->nodetype=tPVAR;
				loc = codeGen(t,fp);
				fprintf(fp,"MOV R%d, [R%d]\n",reg,loc);
				return reg;
		case tBRKP: 
				fprintf(fp,"BRKP\n");
				return -1;
		case tREAD:
				loc =  getLocReg(t->right,fp);
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
				if(t->left->nodetype==tPVAR && t->left->type!=pIntType && t->left->type!=pStringType){	//*p=...
					loc = codeGen(t->left,fp);	//now address of q is in loc (in a register)
				}else {
					loc = getLocReg(t->left,fp);
				}
				fprintf(fp,"MOV [R%d], R%d\n", loc, reg);
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
			push(&breakstack,label_2);
			push(&contstack,label_1);
			codeGen(t->right,fp);
			fprintf(fp, "JMP L%d\n", label_1); // return to the beginning of the loop.
			fprintf(fp, "L%d:\n", label_2); 	// Place the second label here
			return -1;
			}
		case tBREAK:{
			fprintf(fp, "JMP L%d\n", pop(&breakstack));
			pop(&contstack);
			return -1;
		}
		case tCONTINUE:{
			fprintf(fp, "JMP L%d\n", pop(&contstack));
			return -1;
		}
		case tDOWHILE:{
			int label_1 = getLabel();
			int label_2 = getLabel();
			int label_3 = getLabel();
			fprintf (fp, "L%d:\n", label_1); // Place the first label here.
			push(&breakstack,label_2);
			push(&contstack,label_3);
			codeGen(t->left,fp);
			fprintf(fp, "L%d:\n", label_3); //continue causes evalutions
			p=codeGen(t->right,fp);
			fprintf (fp, "JZ R%d, L%d\n", p, label_2);//if zero, jump to label_2 // loop exit
			fprintf(fp, "JMP L%d\n", label_1); // return to the beginning of the loop.
			fprintf(fp, "L%d:\n", label_2); 	// Place the second label here
			return -1;
		}
		case tREPEAT:{
			int label_1 = getLabel();
			int label_2 = getLabel();
			int label_3 = getLabel();
			fprintf (fp, "L%d:\n", label_1); // Place the first label here.
			push(&breakstack,label_2);
			push(&contstack,label_3);
			codeGen(t->left,fp);
			fprintf(fp, "L%d:\n", label_3); //continue causes evalutions
			p=codeGen(t->right,fp);
			fprintf (fp, "JNZ R%d, L%d\n", p, label_2);//if true, not zero, jump to label_2 // loop exit
			fprintf(fp, "JMP L%d\n", label_1); // return to the beginning of the loop.
			fprintf(fp, "L%d:\n", label_2); 	// Place the second label here
			return -1;
		}
		default:
			reg = codeGen(t->left, fp);
			loc = codeGen(t->right, fp);	//can free this
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
	fprintf(fp,"MOV R%d, \"Read\"\n",temp);
	fprintf(fp,"PUSH R%d\n",temp);
	fprintf(fp,"MOV R%d, -1\n",temp);
	fprintf(fp,"PUSH R%d\n",temp);
	fprintf(fp,"MOV R%d, R%d\n",temp,location);
	fprintf(fp,"PUSH R%d\n",temp);
	fprintf(fp,"PUSH R0\nPUSH R0\nCALL 0\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\nPOP R%d\n",temp,temp,temp,temp,temp);
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


