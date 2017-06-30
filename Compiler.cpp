#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//Instruction Set Table
struct instr{
	char *ins;
	int op;
};

struct node{
	struct instr *i;
	struct node* left;
	struct node* right;
};

struct node* createNode(struct instr *in){
	struct node* n = (struct node*)malloc(sizeof(struct node));
	n->i = in;
	n->left = NULL;
	n->right = NULL;
	return n;
}

struct node * addNode(struct node *root, struct instr* in){
	if (root == NULL) 
		return createNode(in);
	if (strcmp(in->ins,root->i->ins) < 0)
		root->left = addNode(root->left, in);
	else if (strcmp(in->ins, root->i->ins) > 0)
		root->right = addNode(root->right, in);
	
	return root;
}

struct node* search(struct node* root, char *instruct){
	if (root == NULL){
		return NULL;
	}
	if (strcmp(instruct, root->i->ins) == 0)
		return root;
	else if (strcmp(instruct, root->i->ins) < 0)
		return search(root->left, instruct);
	else if (strcmp(instruct, root->i->ins) > 0)
		return search(root->right, instruct);
}

struct node* createBst(){

	node *root = NULL;
	instr *inst1 = (struct instr*)malloc(sizeof(struct instr));
	int i = 1;
	inst1->ins = "MOV";
	inst1->op = i;
	root = addNode(root, inst1);

	instr *inst2 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst2->ins = "MOVX";
	inst2->op = i;
	root = addNode(root, inst2);

	instr *inst3 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst3->ins = "ADD";
	inst3->op = i;
	root = addNode(root, inst3);

	instr *inst4 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst4->ins = "SUB";
	inst4->op = i;
	root = addNode(root, inst4);

	instr *inst5 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst5->ins = "MUL";
	inst5->op = i;
	root = addNode(root, inst5);

	instr *inst6 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst6->ins = "JUMP";
	inst6->op = i;
	root = addNode(root, inst6);

	instr *inst7 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst7->ins = "IF";
	inst7->op = i;
	root = addNode(root, inst7);

	instr *inst8 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst8->ins = "EQ";
	inst8->op = i;
	root = addNode(root, inst8);

	instr *inst9 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst9->ins = "LT";
	inst9->op = i;
	root = addNode(root, inst9);

	instr *inst10 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst10->ins = "GT";
	inst10->op = i;
	root = addNode(root, inst10);

	instr *inst11 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst11->ins = "LTEQ";
	inst11->op = i;
	root = addNode(root, inst11);

	instr *inst12 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst12->ins = "GTEQ";
	inst12->op = i;
	root = addNode(root, inst12);

	instr *inst13 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst13->ins = "PRINT";
	inst13->op = i;
	root = addNode(root, inst13);

	instr *inst14 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst14->ins = "READ";
	inst14->op = i;
	root = addNode(root, inst14);

	instr *inst15 = (struct instr*)malloc(sizeof(struct instr));
	i++;
	inst15->ins = "ELSE\n";
	inst15->op = i;
	root = addNode(root, inst15);

	return root;
}


//Registers
char *registers[8] = { "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX" };

int isRegister(char *inst){
	for (int i = 0; i < 8; i++){
		if (strcmp(registers[i], inst) == 0){
			return 1;
		}
	}
	return 0;
}

int getRegister(char *inst){
	for (int i = 0; i < 8; i++){
		if (strcmp(registers[i], inst) == 0){
			return i;
		}
	}
	return -1;
}

//Symbol Table
struct symbol{
	char var[100];
	int size;
	int addr;
};


struct symbol *createSymbol(char *var, int size, int prevSize, int currAddr){
	struct symbol *sym = (struct symbol*)malloc(sizeof(struct symbol));
	char name[10];
	strcpy(name, var);
	if (name[strlen(name) - 1] == '\n')
		name[strlen(name) - 1] = '\0';
	strcpy(sym->var,name);
	sym->size = size;
	sym->addr = prevSize + currAddr;
	return sym;
}
struct symbol **table = (struct symbol**)malloc(sizeof(struct symbol*));
int symInd = -1;

int isArray(char *variable){
	for (int i = 0; i < strlen(variable); i++){
		if (variable[i] == '[')
			return 1;
	}
	return 0;
}

int toNum(char *str){
	int num = 0;
	for (int i = 0; str[i] != '\0'; i++){
		num = num * 10 + (str[i] - '0');
	}
	return num;
}

int arraySize(char *variable){
	char *token = strtok(variable, "[");
	token = strtok(NULL, "[");
	char *tokens = strtok(token, "]");
	return toNum(tokens);
}

char* arrayName(char *variable){
	char res[100];
	char *tokens = strtok(variable, "[");
	strcpy(res, tokens);
	return res;
}

void addSymbol(char *variable, int flag){
	int temp = symInd;
	table = (struct symbol**)realloc(table, sizeof(struct symbol*)*(symInd + 2));
	if (isArray(variable) == 1){
		int size = arraySize(variable);
		char name[100];
		strcpy(name,arrayName(variable));
		if (temp != -1){
			table[++symInd] = createSymbol(name, size, table[temp]->size, table[temp]->addr);
		}
		else{
			table[++symInd] = createSymbol(arrayName(variable), size, 0, 0);
		}
	}
	else if (flag == 0){
		if (temp != -1){
			table[++symInd] = createSymbol(variable,0, table[temp]->size, table[temp]->addr);
		}
		else{
			table[++symInd] = createSymbol(variable, 1, 0, 0);
		}
	}
	else{
		if (temp != -1){
			table[++symInd] = createSymbol(variable, 1, table[temp]->size, table[temp]->addr);
		}
		else{
			table[++symInd] = createSymbol(variable, 1, 0, 0);
		}
	}
}

void printSymbol(){
	printf("Symbols:\tSize\t:Address\n");
	for (int i = 0; i <= symInd; i++){
		printf("%s\t\t%d\t%d\t\n", table[i]->var,table[i]->size,table[i]->addr);
	}
}

int isSymbol(char *variable){
	for (int i = 0; i <= symInd; i++){
		if (strcmp(table[i]->var, variable) == 0){
			return 1;
		}
	}
	return 0;
}

int getSymbol(char *variable){
	for (int i = 0; i <= symInd; i++){
		if (strcmp(table[i]->var, variable) == 0){
			return table[i]->addr;
		}
	}
	return 0;
}


struct icode **output = (struct icode**)malloc(sizeof(struct icode*));
int ino = 0;

//Block Tables

struct block{
	char label[50];
	int addr;
};\

struct block **blocks = (struct block**)malloc(sizeof(struct block*));
int blen = 0;

struct block* createBlock(char *label, int addr){
	struct block *b = (struct block*)malloc(sizeof(struct block));
	if (strcmp(label, "ELSE\n") == 0 || strcmp(label, "ENDIF\n") == 0){
		label[strlen(label) - 1] = '\0';
		strcpy(b->label, label);
		b->addr = addr + 1;
		return b;
	}
	label[strlen(label) - 2] = '\0';
	strcpy(b->label, label);
	b->addr = addr + 1;
	return b;
}

void addBlock(char *line){
	char label[50];
	strcpy(label, line);
	blocks = (struct block **)realloc(blocks, sizeof(struct block*)*(blen + 1));
	blocks[blen++] = createBlock(label, ino);
}

void printBlock(){
	printf("\nBlocks:\n");
	printf("Label\tAddress\t\n");
	for (int i = 0; i < blen; i++){
		printf("%s\t\t%d\t\n", blocks[i]->label, blocks[i]->addr);
	}
}

int isBlock(char *inst){
	for (int i = 0; i < blen; i++){
		if (strcmp(inst, blocks[i]->label) == 0){
			return 1;
		}
	}
	return 0;
}

int getBlock(char *inst){
	for (int i = 0; i < blen; i++){
		if (strcmp(inst, blocks[i]->label) == 0){
			return blocks[i]->addr;
		}
	}
}


//Intermediate Code Generated Table
struct icode{
	int ino;
	int opc;
	int params[4];
};

struct icode* createIcode(int opc, int *parameters, int len){
	struct icode *i = (struct icode*)malloc(sizeof(icode));
	i->ino = ino + 1;
	i->opc = opc;
	
	for (int k = 0; k < 4; k++){
		i->params[k] = -2;
	}

	for (int j = 0; j < len; j++){
		i->params[j] = parameters[j];
	}
	return i;
}

int isOper(char *instr){
	if (strcmp(instr, "ADD") == 0 || strcmp(instr, "SUB") == 0 || strcmp(instr, "MUL") == 0)
		return 1;
	return 0;
}

void addIcode(struct node *root,char *line){
	char temper[100];
	int flag = 0;
	output = (struct icode**)realloc(output, sizeof(struct icode*)*(ino + 1));
	strcpy(temper, line);
	char *inst = strtok(line, " ");
	if (inst[0] == '\t')
	{
		int i = 0;
		for (i = 0; i < strlen(inst) - 1; i++){
			inst[i] = inst[i + 1];
		}
		inst[strlen(inst) - 1] = '\0';
	}
	struct node* s = search(root,inst);
	struct node *temp;
	int opc = 0,params[4];
	if(s != NULL){
		opc = s->i->op;
	}
	int i = 0;
	char *instr = strtok(temper, " ");
	char iit[50];
	strcpy(iit, instr);
	if (strcmp(instr, "IF") == 0){
		flag = 1;
		params[3] = -1;
	}
	else if (strcmp(instr, "ELSE\n") == 0){
		params[0] = -1;
		output[ino++] = createIcode(opc, params, 1);
		flag = -1;
	}
	if (flag == 1)
		instr = strtok(NULL, " ");
	else
		instr = strtok(NULL, ",");

	while (instr != NULL){
		if(instr[strlen(instr)-1] == '\n')
			instr[strlen(instr) - 1] = '\0';
		if (isRegister(instr) == 1){
			params[i] = getRegister(instr);
		}
		else if ((temp = search(root,instr)) != NULL){
			params[i] = temp->i->op;
		}
		else if (isArray(instr) == 1){
			char c[2];
			c[0] = instr[0];
			c[1] = '\0';
			params[i] = getSymbol(c);
		}
		else if(isSymbol(instr) == 1){
			params[i] = getSymbol(instr);
		}
		else if (isBlock(instr) == 1){
			params[i] = getBlock(instr);
		}
		if (isOper(iit) == 1)
			instr = strtok(NULL, ", ");
		else
			instr = strtok(NULL, " ");
		
		i++;
	}
	if (flag != -1)
		output[ino++] = createIcode(opc, params, i);
}

void printTable(){
	printf("Intermediate Code:\n");
	for (int i = 0; i < ino; i++){
		printf("%d\t%d\t", output[i]->ino, output[i]->opc);
		for (int j = 0; j < 4; j++){
			if (output[i]->params[j] == -2)
				printf(" ");
			else
				printf("%d\t",output[i]->params[j]);
		}
		printf("\n");
	}
}

void parseIcode(){
	for (int i = 0; i < ino; i++){
		for (int j = 0; j < 4; j++){
			if (output[i]->params[j] == -1){
				if (output[i]->opc == 6){
					for (int k = 0; k < blen; k++){
						if (strcmp(blocks[k]->label,"ELSE") !=  0 || strcmp(blocks[k]->label,"ENDIF") != 0){
							output[i]->params[j] = blocks[k]->addr;
						}
					}
				}
				else if (output[i]->opc == 7){
					for (int k = 0; k < blen; k++){
						if (strcmp(blocks[k]->label,"ELSE") == 0 )//|| blocks[k]->label != "ENDIF"){
						{
							output[i]->params[j] = blocks[k]->addr;
						}
					}
				}
				else if(output[i]->opc == 15){
					for (int k = 0; k < blen; k++){
						if (strcmp(blocks[k]->label,"ENDIF") == 0)//|| blocks[k]->label != "ENDIF"){
						{
							output[i]->params[j] = blocks[k]->addr;
						}
					}
				}
			}
		}
	}
}

int mem[100];

int stack[100];

int main(){
	struct node* root = createBst();

	char fname[100], expr[100];
	FILE *source, *target;
	int ind = 0;
	source = fopen("vasavi.txt","r");
	fflush(source);

	if (source == NULL){
		printf("Error! File could Not be opened\n");
		exit(1);
	}
	int flag = 1;
	while (fgets(expr, 100, source) != NULL){
			if (strcmp(expr,"START:\n") == 0 ){
				break;
			}
			char *token = strtok(expr, " ");
			if (strcmp(token, "CONST") == 0){
				flag = 0;
			}
			token = strtok(NULL, " ");
			addSymbol(token,flag);
			if (flag == 0){
				token = strtok(NULL, " ");
				token = strtok(NULL, " ");
				token[strlen(token) - 1] = '\0';
				mem[symInd + 1] = toNum(token);
			}
	}
	printSymbol();
	int ino = 1;
	while (fgets(expr, 100, source) != NULL){
		char line[100];
		strcpy(line, expr);
		char *token = strtok(expr, " ");
		if (token[0] == '\t')
		{
			int i = 0;
			for (i = 0; i < strlen(token) - 1; i++){
				token[i] = token[i + 1];
			}
			token[strlen(token)-1] = '\0';
		}
		char label[50];
		strcpy(label, token);
		if (strcmp(label, "END\n") == 0)
			break;
		struct node* s = search(root, label);
		if (s != NULL & strcmp(token,"ELSE\n") == 0){
			addIcode(root, line);
			addBlock(label);
		}
		else if(s != NULL){
			addIcode(root, line);
		}
		else{
			addBlock(label);
		}
	}
	parseIcode();
	printTable();
	printBlock();
	system("pause");	
}
