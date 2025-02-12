#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//saja nazih asfour
//1210737
//sec 4
typedef struct node* PtrToNode;
typedef PtrToNode Stack;
int IsEmpty(Stack s);
Stack CreatStack(char ch);
void MakeEmpty(Stack s);
char Pop(Stack *s);
char Top(Stack s);
void Push(char x,Stack *s);
void DisposeStack(Stack s);
void PrintStack(Stack s);
int CheckOperator(char ch);
int Precedence(char ch);
int CheckValidity( const char* in);
void InfixToPrefix(const char* infix, char * pre);
int EvaluateThePrefix(const char * output);
void ReadFromFile(const char * name);
void PrintTheResult(int EqNumber ,char *eq);
struct node{
    char Element;
    PtrToNode Next;
};
int main()
{
    int choice;
    do{
        printf("1. Read the equations from file\n");
        printf("2. Check the valid equations\n");
        printf("3. Convert equations from infix to prefix\n");
        printf("4. Evaluate the infix equations\n");
        printf("5. Print invalid equations\n");
        printf("6. Write equations to file\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:; char name[100];//assume the file name length is 100
                    printf("Enter the name of the input file: \n");
                    scanf("%s", name);
                    FILE* in = fopen(name, "r");
                    if (in == NULL) {
                        printf("Failed to open the file.\n");
                    }
                    //assume the file has 100 line of equation
                    char line[100];
                    //to store the equation(pointer to char*)
                    char** equations = NULL;
                    //to store the valid equation
                    int* valid = NULL;
                    //to number the equation
                    int Equationsno = 0;
                    //read line line from the file
                    while (fgets(line, sizeof(line), in)) {
                        line[strcspn(line, "\n")] = '\0'; // Remove newline character
                        // Allocate memory for the equation
                        equations = (char**)realloc(equations, (Equationsno + 1) * sizeof(char*));
                        equations[Equationsno] = (char*)malloc((strlen(line) + 1) * sizeof(char));
                        strcpy(equations[Equationsno], line);
                        // Check the validity of the equation
                        if (CheckValidity(line)) {
                            //realloc used to resize the memory block pointed to a pointer that was
                            //previously allocated to the variable by the malloc() or calloc() function
                            valid = (int*)realloc(valid, (Equationsno + 1) * sizeof(int));
                            valid[Equationsno] = 1;
                        }
                        else {
                            valid = (int*)realloc(valid, (Equationsno + 1) * sizeof(int));
                            valid[Equationsno] = 0;
                        }
                        Equationsno++;
                    }
                    fclose(in);
                    break;
            case 2:for (int i = 0; i < Equationsno; i++) {
                    printf("Equation No. %d: %s ", i + 1, equations[i]);
                    if (valid[i]) {
                        printf("-> Valid\n");
                    }
                    else {
                        printf("-> Invalid\n");
                    }
                }
                break;
            case 3:
                    for (int i = 0; i < Equationsno; i++) {
                    if (valid[i]) {
                        char prefix[100];
                        InfixToPrefix(equations[i],prefix);
                        if (prefix != NULL) {
                            printf("Equation No. %d: Infix: %s -> Prefix: %s\n", i + 1, equations[i], prefix);
                            //free(prefix);
                        }
                        else {
                            printf("Equation No. %d: Invalid equation\n", i + 1);
                        }
                    }
                    else {
                        printf("Equation No. %d: Invalid equation\n", i + 1);
                    }
                }
            break;
            case 4:
                   for (int i = 0; i < Equationsno; i++) {
                    if (valid[i]) {
                        PrintTheResult(i+1,equations[i]);
                    }
                }
                break;
            case 5:
                    for ( int i = 0; i < Equationsno; i++) {
                        if (!valid[i]) {
                            printf("Equation No. %d: %s\n", i + 1, equations[i]);
                        }
                    }
                break;
            case 6:;
                FILE* out = fopen("output.txt", "w");
                for (int i = 0; i < Equationsno; i++) {
                    fprintf(out, "Equation No. %d: %s\n", i + 1, equations[i]);
                    if (valid[i]) {
                        char prefix[100] ;
                        InfixToPrefix(equations[i],prefix);
                        if (prefix != NULL) {
                            int result = EvaluateThePrefix(prefix);
                            fprintf(out, "Prefix: %s\nResult: %d\n", prefix, result);
                        }
                    }
                    else {
                        fprintf(out, "Invalid Equation\n");
                    }
                    fprintf(out, "\n");
               }
               fclose(out);
               printf("Equations written to output.txt\n");
               break;
            case 7:
                // Free memory allocated for equations
                for (int i = 0; i < Equationsno; i++) {
                    free(equations[i]);
                }
                free(equations);
                free(valid);
                exit(0);
                break;
            default:
                printf("Enter another choice \n");
        }
    }while(choice!=7);
    return 0;
}

//This function is to check if the stack is empty(return 1) or not empty(return 0)
int IsEmpty(Stack s){
    return s==NULL;
}
//This function to make a new stack
Stack CreatStack(char ch){
    Stack s;
    s=(Stack)malloc(sizeof(struct node));
    //to check if there is free space in memory
    if(s==NULL)
        printf("out of space\n");
    s->Element=ch;
    s->Next=NULL;
    return s;
}
//function to delete element from the stack
void MakeEmpty(Stack s){
    if(s==NULL)
        printf("out of space\n");
    else
        while(!IsEmpty(s))
            Pop(&s);
}
// Function to pop the top element from the stack and delete it
char Pop(Stack *s){
    PtrToNode first;
    if(IsEmpty(*s)){
        printf("the stack is empty\n");
        exit(0);
    }
    else{
        first=*s;
        char ch=first->Element;
        *s=first->Next;
        free(first);
        return ch;
    }
}
// Function to return the top element of the stack without delete it(the most recently added element)
char Top(Stack s){
    if(IsEmpty(s)){
        printf("The stack is empty \n");
        exit(0);
    }
    return s->Element;
}
// Function to push an element to the stack(insert element at top)
void Push(char x,Stack *s){
    PtrToNode temp=(Stack)malloc(sizeof(struct node));
    if(temp==NULL)
        printf("out of space\n");
    else{
        temp->Element=x;
        temp->Next=*s;
        *s=temp;
    }
}
//function to delete the stack and his element
void DisposeStack(Stack s){
    MakeEmpty(s);
    free(s);
}
//function to print the stack element
void PrintStack(Stack s){
    Stack temp=s->Next;
    while(temp!=NULL){
        printf("%d\t",temp->Element);
        temp=temp->Next;
    }
    printf("\n");
}
// Function to check if a character is an operator
int CheckOperator(char ch){
    return (ch=='+'||ch=='-'||ch=='^'||ch=='*'||ch=='/');
}
// Function to get the precedence of an operator
//1- ^ 2- * and / 3- + and -
int Precedence(char ch){
    if (ch == '+' || ch == '-')
        return 1;
    else if (ch == '*' || ch == '/')
        return 2;
    else if (ch == '^')
        return 3;
    //if the char neither + ,- ,*,/,^
    return 0;
}
// Function to check the validity of an equation
int CheckValidity( const char* in){
    //make an empty stack
    Stack st=NULL;
    //counter to track all the equation till end
    int i;
    //loop to track the equation till end
    for(i = 0 ; in[i] !='\0' ; i++ ){
        //if we have an opening symbol
        if( in[i] == '(' || in[i] == '{' || in[i] == '[' || in[i] == '<')
            //push it onto the stack
            Push(in[i],&st);
            //if we have an closing symbol
        else if( in[i] == ')' || in[i] == '}' || in[i] == ']' || in[i] == '>'){
            //if the stack is empty return 0(make invalid)
            /*if the symbol in the top of stack is not corresponding to the opining
            return 0 (invalid)
            */
            if (IsEmpty(st) || ((in[i] == ')' && Top(st) != '(') ||(in[i] == ']' && Top(st) != '[')
                                || (in[i] == '}' && Top(st) != '{') ||(in[i] == '>'&& Top(st) != '<'))) {
                return 0;
            }
            Pop(&st);
        }
        // if we have no operator between number and symbol
        //return 0(invalid)
        if (in[i] >= '0'&& in[i] <='9' && (in[i+1] == '(' || in[i+1] == '{' || in[i+1] == '[' || in[i+1] == '<'))
            return 0;
        // if we have operator and then we have another operator next of it expect + and -
        //return 0(invalid)
        if (CheckOperator(in[i]) && (in[i+1] == '*' || in[i+1]== '/' || in[i+1]=='^'))
            return 0;
        //Assuming we do not write the = in the equation
        if (in[i] == '=')
            return 0;
    }
    //if reached the end of the file and the stack is not empty then return 0(error) if it is empty return 1(valid)
    return IsEmpty(st);
}
//This function to convert the equation from infix to prefix
void InfixToPrefix(const char* infix, char * output){
    //first : make an empty stack
    Stack st=NULL;
    //this integer to know the length of the infix equation to use it when revere it
    int EqLen=strlen(infix);
    //x and y use as counters  inside the loop to reverse the infix equation
    //and use it inside loop to Iterate through the reversed infix equation
    int x,y;
    //make an array to hold the reversed infix
    char reversed[EqLen +1];
    //2:loop to reverse the infix equation:
    for(x=0,y=EqLen-1;x<EqLen;x++,y--){
        reversed[x]=infix[y];
    }
    reversed[EqLen]='\0';//here the infix is reversed
    //3-We deal with convert as we convert to postfix
    for(x=0,y=0 ; reversed[x] != '\0'; x++){
       // char c=reversed[x];
        //if we have an space
        if(reversed[x] == ' ')
            //there is no problem so continue
            continue;
            //if read an number
        else if(reversed[x] <= '9' && reversed[x] >= '0'){
            //place it into the output
            output[y++]=reversed[x];
            while(!CheckOperator(reversed[x+1]) &&reversed[x+1] <= '9' && reversed[x+1] >= '0' ){
                output[y++]=reversed[x+1];
                x++;
            }
            output[y++]=' ';
        }
            //if an operation is read
        else if(CheckOperator(reversed[x])){
            /* if the top of the stack is lower priority then insert it
               else : pop the top elements in the stack until there is no
               operator having higher priority than one we read
               then push into the stack
            */
            //here the top of stack is higher
            //if they are in the same priority , pop then push
            while(!IsEmpty(st) && ((Precedence(reversed[x]) < Precedence(Top(st)))||(Precedence(reversed[x]) == Precedence(Top(st)) ))){
                //pop it and write in output
                output[y++]=Pop(&st);
            }
            //push the operator in the stack
            Push(reversed[x],&st);
        }
            //since we reversed it so if we have an closing symbol then push into the stack
        else if (reversed[x] == ')' || reversed[x] == '}' || reversed[x] == '>' || reversed[x] == ']')
            Push(reversed[x],&st);
            //if we have an opening symbol
        else if (reversed[x] == '(' || reversed[x] == '{' || reversed[x] == '<' || reversed[x] == '['){
            while(!IsEmpty(st) && Top(st) != ')' && Top(st) != '>' && Top(st) != '}' && Top(st) != ']')
                output[y++] = Pop(&st);
            //remove closing symbol
            Pop(&st);
        }
    }
    //pop the remaining element from the stack(operator)
    while(!IsEmpty(st))
        output[y++]=Pop(&st);
    //here we have the postfix for the reversed infix
    // so we have to reversed it again to make the prefix
    output[y]='\0';
    EqLen=strlen(output);
    for(x = 0 ,y = EqLen-1 ; x<y ; x++ , y--){
        char tmp = output[x];
        output[x] = output[y];
        output[y] = tmp;
    }
}
//in this function we evaluate the prefix equation
int EvaluateThePrefix(const char * output){
    //make an empty stack
    Stack st=NULL;
    //counter to the loop Iterate through the prefix expression
    int counter;
    //we need to scan the prefix from right to left
    //Iterate through the prefix expression
    for(counter = strlen(output)- 1 ; counter >=0 ;counter--){
        //character to hold the element (from right to left(from the last index))
        char c = output[counter];
        //if the element is an number (0-9)
        if( c >='0' && c <= '9' ){
            if(output[counter-1] >='0' && output[counter-1] <= '9' ){
                    int num=0;
                    //if we have number with two digit
                    num=(output[counter-1]-'0')*10+((output[counter])-'0');
                    counter--;
                    //push all number into the stack
                    Push(num,&st);
               }
            //push into the stack(number with one digit)
            else
               Push((c-'0'),&st);
        }
        //if the element is operator
        else if( CheckOperator(c)){
            //pop the top two element from the stack
            int op1 = Pop(&st);
            int op2 = Pop(&st);
            //the cases of all operator
            if ( c == '+')
                Push(op1+op2,&st);
            else if ( c == '-'){
                Push(op1-op2 ,&st);
            }
            else if ( c == '*')
                Push(op1*op2 , &st);
            else if ( c == '/')
                Push(op1/op2 , &st);
            else if ( c == '^')
                Push((int)pow(op1,op2),&st);
        }
    }
    //pop the result
    return Pop(&st);
}
void PrintTheResult(int EqNumber ,char *eq){
    char prefix[100];
    InfixToPrefix(eq,prefix);
    if(prefix == NULL){
        printf("Equation No. %d: Invalid\n", EqNumber);
    }
    int res=EvaluateThePrefix(prefix);
    printf("Equation No. %d: %s = %d\n", EqNumber, prefix, res);
}
