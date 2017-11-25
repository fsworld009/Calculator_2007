#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_STACK_SIZE 100					//���|�̤j��
#define MAX_INPUT_SIZE 100					//�̤j��J�q
int stack[MAX_STACK_SIZE];					//���|
char string[MAX_INPUT_SIZE];				//�x�s�ϥΪ̿�J���r��
char string2[MAX_INPUT_SIZE];				//�x�s��Ǫ��r��

bool S_IsEmpty(int top){
	if (top<0)
		return true;
	return false;
}

bool S_IsFull(int top){
	if (top>=MAX_STACK_SIZE-1)
		return true;
	return false;
}

void S_Add(int* top,int token){
	if (!S_IsFull(*top)){
		(*top)++;
		stack[*top]=token;
	}
	return;
}

int S_Delete(int* top){
	if (!S_IsEmpty(*top)){
		return stack[(*top)--];
	}
	return 0;
}

char get_token(char symbol){
	if(symbol>='0' && symbol<='9')
		return '0';								//0�@���Ʀr���N��
	switch(symbol){
	case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '\0': case ' ':
		return symbol;
	default:
		return '!';							//!�@����J���~���N��
	}
}

bool errorcheck(){
	int n=0;								//positon of string[]
	char token;								//character type of string[n]
	int lparen=0, rparen=0, sign=0, num=0;	//counter for lparen, rparen, sign, number
	token = get_token(string[n]);
	while(token != '\0'){
		switch(token){
		case '!':
			return false;
		case '(':
			lparen++;sign=0;break;
		case ')':
			rparen++;sign=0;break;
		case '0':
			if(num==0)
				num++;
			sign=0;
			break;
		case '+': case '-': case '*': case '/': case '%':
			num=0;sign++;break;
		case ' ':
			num++;
			break;
		}
		if(sign > 1 || (num>1 && token != ' '))
			return false;
		token = get_token(string[++n]);
	}
	if(lparen != rparen)
		return false;
	return true;
}

int priority_token(char c){								//�u���v�A')'���ݭn�A�]���w�b�e����else if�B�z�L�A�bToken��'('�u���v�̰�
	switch(c){
	case '(':
		return 4;
	case '*': case '/': case '%':
		return 3;
	case '+': case '-':
		return 2;
	default:
		return 0;
	}
}

int priority_stack(char c){								//�u���v�A')'���ݭn�A�]���w�b�e����else if�B�z�L�A�bStack��'('�u���v�̧C
	switch(c){
	case '*': case '/': case '%':
		return 3;
	case '+': case '-':
		return 2;
	case '(':
		return 1;
	default:
		return 0;
	}
}

void postfix(){
	int top=-1;					//stack����
	int n=0;					//string�ثeŪ�쪺��m
	int n2=0;					//string2������
	char token;					//�ثeŪ�쪺�r�����A
	bool numspace=true;			//�O�_�b�Ÿ��e�ɪŮ�
	token = get_token(string[n]);
	while(token != '\0'){
		if(token == '0'){					//�Ʀr�����string2��
			numspace=true;						//�Ʀr�i��ݭn�ɪŮ�
			string2[n2++]=string[n];
		}else if(token == ')'){					//')'
			while(stack[top] != '('){
				numspace=false;				//���e����Xstack���������A�����ɪŮ�
				string2[n2++]=S_Delete(&top);
			}
			S_Delete(&top);					//�j�鵲����n��'('����;
		}else if(token != ' '){
			while(priority_stack(stack[top]) >= priority_token(token)){
				numspace=false;				//���e����Xstack���������A�����ɪŮ�
				string2[n2++]=S_Delete(&top);
			}
			if(numspace && token != '(' && token != ')')
				string2[n2++]=' ';			//���s�Ÿ��istack�B���r���D'()'�A��ܦ��Ʀr�����A�B���U�Ӥ]�O�Ʀr�A�[�Ů�Ϥ�
			S_Add(&top,token);
		}
		
		token = get_token(string[++n]);
	}
	while(!S_IsEmpty(top)){
		string2[n2++]=stack[top];
		S_Delete(&top);
	}
}

int operandfix(int count,int n){
	double a=0;
	for(int i=0;i<count;i++)
		a = a+(string2[n--]-'0')*pow(10,i);
	return a;
}

void evaluate(){
	char token;
	int op1, op2;
	int n=0, count=0;
	int top=-1;
	token = get_token(string2[n]);
	while(token != '\0'){
		if(token == '0')
			count++;
		else if(token != '\0'){
			//�J��+-*/% �����Ʀr����
			if(count !=0){
				S_Add(&top, operandfix(count, n-1));
				count=0;
			}
			if(token != ' '){				//���O�Ů���B��Ÿ��A�i��B��
				op2 = S_Delete(&top);		//���o�̤W�@�檺��Ƶ�op2
				op1 = S_Delete(&top);		//���o�̤W�@�檺��Ƶ�op1
				switch(token){
				case '+':
					S_Add(&top,op1+op2);break;
				case '-':
					S_Add(&top,op1-op2);break;
				case '*':
					S_Add(&top,op1*op2);break;
				case '/':
					S_Add(&top,op1/op2);break;
				case '%':
					S_Add(&top,op1%op2);break;
				}
			}
		}
		token = get_token(string2[++n]);


	}
	if(stack[0] == NULL)								//�����S���i��B��A��ܨϥΪ̥���J�B��B��
		S_Add(&top, operandfix(count, n-1));			//�N��J���Ʀr�����ন����
}

void main(){
	int i;
	printf("(Type \"Q\" if you want to quit the program)\n");
	while(true){
		for(i=0;i<MAX_INPUT_SIZE;i++)			//reset the string2
			string2[i]=0;
		for(i=0;i<MAX_STACK_SIZE;i++)
			stack[i]=0;							//reset the stack
		printf("Input Expression:\n");
		gets(string);
		if(!strcmp(string,"Q"))
			break;
		if(!errorcheck()){
			printf("Input ERROR!! This Expression is illegal.\n\n");
			continue;
		}
		postfix();
		evaluate();
		printf("Answer:\n%d\n\n",stack[0]);
	}
}