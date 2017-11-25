#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_STACK_SIZE 100					//堆疊最大值
#define MAX_INPUT_SIZE 100					//最大輸入量
int stack[MAX_STACK_SIZE];					//堆疊
char string[MAX_INPUT_SIZE];				//儲存使用者輸入的字串
char string2[MAX_INPUT_SIZE];				//儲存後序的字串

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
		return '0';								//0作為數字之代表號
	switch(symbol){
	case '+': case '-': case '*': case '/': case '%': case '(': case ')': case '\0': case ' ':
		return symbol;
	default:
		return '!';							//!作為輸入錯誤之代號
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

int priority_token(char c){								//優先權，')'不需要，因為已在前面的else if處理過，在Token內'('優先權最高
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

int priority_stack(char c){								//優先權，')'不需要，因為已在前面的else if處理過，在Stack內'('優先權最低
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
	int top=-1;					//stack指標
	int n=0;					//string目前讀到的位置
	int n2=0;					//string2的指標
	char token;					//目前讀到的字元型態
	bool numspace=true;			//是否在符號前補空格
	token = get_token(string[n]);
	while(token != '\0'){
		if(token == '0'){					//數字→放到string2中
			numspace=true;						//數字可能需要補空格
			string2[n2++]=string[n];
		}else if(token == ')'){					//')'
			while(stack[top] != '('){
				numspace=false;				//先前有丟出stack內的元素，不必補空格
				string2[n2++]=S_Delete(&top);
			}
			S_Delete(&top);					//迴圈結束後要把'('拿掉;
		}else if(token != ' '){
			while(priority_stack(stack[top]) >= priority_token(token)){
				numspace=false;				//先前有丟出stack內的元素，不必補空格
				string2[n2++]=S_Delete(&top);
			}
			if(numspace && token != '(' && token != ')')
				string2[n2++]=' ';			//有新符號進stack且此字元非'()'，表示此數字完結，且接下來也是數字，加空格區分
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
			//遇到+-*/% →此數字結束
			if(count !=0){
				S_Add(&top, operandfix(count, n-1));
				count=0;
			}
			if(token != ' '){				//不是空格→運算符號，進行運算
				op2 = S_Delete(&top);		//取得最上一格的資料給op2
				op1 = S_Delete(&top);		//取得最上一格的資料給op1
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
	if(stack[0] == NULL)								//完全沒有進行運算，表示使用者未輸入運算浮號
		S_Add(&top, operandfix(count, n-1));			//將輸入的數字直接轉成答案
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