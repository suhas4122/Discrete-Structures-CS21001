#include <bits/stdc++.h>
using namespace std;

#define MAX 10000

/**********************************************************/
/************** Stack Datatype and Operations *************/
/**********************************************************/

// stack structure declaration
typedef struct stackADT {
    char opr[MAX];
    int top;
} Stack;

// initialize stack
void initStack (Stack *stack)
{
    stack->top = -1;
}

// check whether stack is empty
int isEmptyStack (Stack *stack)
{
    return (stack->top == -1);
}

// check whether stack is full
int isFullStack (Stack *stack)
{
    return (stack->top == MAX-1);
}

// push an element into stack
void push (Stack *stack, char x)
{
    if (isFullStack(stack))
        printf("Error: Stack is Full!\n");
    else
        stack->opr[++(stack->top)] = x;
}

// pop an element from stack
char pop (Stack *stack)
{
    char x;
    if (isEmptyStack(stack))
        printf("Error: Stack is Empty!\n");
    else
        x = stack->opr[(stack->top)--];
    return x;
}

/**********************************************************/


/**********************************************************/
/*********** Binary Tree Datatype and Operations **********/
/**********************************************************/

// binary tree structure declaration
typedef struct binTree {
    char element;
    struct binTree *leftChild;
    struct binTree *rightChild;
} BT;

// creating a node in binary tree
BT *createNode (char el)
{
    BT *newNode;
    if ( (newNode=(BT *)malloc(sizeof(BT))) == NULL )
        printf("Error: Malloc Error in Creating Node!\n");
    else {
        newNode->element = el;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    return (newNode);
}

// creating an exact replica of the the binary tree
BT *duplicate(BT *orig)
{
    BT *dup = NULL;
    if(orig != NULL) {
        dup = createNode(orig->element);
        dup->leftChild = duplicate(orig->leftChild);
        dup->rightChild = duplicate(orig->rightChild);
    }
    return (dup);
}

/**********************************************************/


/**********************************************************/
/******************** Utility Functions *******************/
/**********************************************************/

// structure holding propositional values
typedef struct PropVal {
    char prop;
    int val; // '0' for False and '1' for True
} PV;

// scan every propositional values (one by one) from user as input
PV *scanPropValueInput(int &x)
{
    unsigned int noProp, i;
    PV *pvarr;

    printf("Enter Total Number of Propositions: ");
    scanf("%u",&noProp);
    x=noProp;
    pvarr = (PV *)malloc(noProp * sizeof(PV));

    for (i = 0; i < noProp; i++) {
        printf("Enter Proposition [%u] (Format: Name <SPACE> Value): ", i+1);
        scanf(" %c %d", &pvarr[i].prop, &pvarr[i].val);
    }
    return pvarr;
}

// determines whether p is a proposition including 0 or 1
int isProposition (char p)
{
    return ( ((p >= 'a') && (p <= 'z')) || ((p >= 'A') && (p <= 'Z')) || (p == '0') || (p == '1') );
}

// printing the validity and satisfiability flags
void printResult (int valid, int sat)
{
    printf("\nThe Given Formula is: < ");
    valid ? printf("VALID") : printf("INVALID");
    printf(" + ");
    sat ? printf("SATISFIABLE") : printf("UNSATISFIABLE");
    printf(" >\n\n");
}

//display postfix form of propositional formula (from internally represented string)
void displayPfForm(char *pfForm)
{
    int i;

    printf("Postfix Representation of Formula:");
    for(i = 0; pfForm[i] != '\0'; i++){
        if ( pfForm[i] == '-' )
            printf(" ->");
        else if (pfForm[i] == '~')
            printf(" <->");
        else
            printf(" %c", pfForm[i]);
    }
    printf("\n");
}

// count number of characters in the formula representing only propositions and ops
int noOfIdsInFormula (char *formula)
{
    int i, len = strlen(formula), count = 0;
    for(i = 0; i < len; i++){
        if ( (formula[i] != '(') && (formula[i] != ')') && (formula[i] != ' ') && (formula[i] != '\t') )
            count++;
    }
    return count;
}

// pre-defined priority of in-stack element
int inStackPriority (char op){
    switch(op){
        case '!': return 3; break;

        case '&':
        case '|': return 2; break;

        case '~':
        case '-': return 1; break;

        case '(': return 0; break;

        default : return -1; break;
    }
}

// pre-defined priority of in-coming element
int inComingPriority (char op){
    switch(op){
        case '!': return 4; break;

        case '&':
        case '|': return 2; break;

        case '~':
        case '-': return 1; break;

        case '(': return 4; break;

        default : return -1; break;
    }
}

// generate postfix formula from the given input formula
char *genPostFixFormula(char *formula)
{
    int noOfIds = noOfIdsInFormula(formula), i, len = strlen(formula), k;
    char *pf = (char *)malloc((noOfIds+1) * sizeof(char));
    char out;
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    initStack(stack); push(stack,'#');

    for (i = 0, k = 0; i < len; i++){
        if ( (formula[i] != ' ') && (formula[i] != '\t') ){
            if ( isProposition(formula[i]) )
                pf[k++] = formula[i];
            else if (formula[i] == ')') {
                while ( (out = pop(stack)) != '(')
                    pf[k++] = out;
            }
            else {
                while ( inStackPriority(out = pop(stack)) >= inComingPriority(formula[i]) )
                    pf[k++] = out;
                push(stack, out);
                push(stack, formula[i]);
            }
        }
    }
    while( (out = pop(stack)) != '#' )
        pf[k++] = out;
    pf[k] = '\0';

    return pf;
}

/**********************************************************/





/**********************************************************/
/****************** YOUR CODE STARTS HERE *****************/
/**********************************************************/

// expression tree formation from postfix formula string
bool isSym(char c){
    return (c=='&' || c=='-' || c=='~' || c=='|');
}

BT *ETF (char *pfForm, int start, int end)
{
    BT *et = NULL;

    // WRITE YOUR CODE HERE ...
    et  = createNode(pfForm[end]);
    if(start==end) return et;
    if((end-start)>0 && pfForm[end]=='!'){
        et->rightChild = ETF(pfForm, start, end-1);
        return et;
    }
    if(end-start>1){
        int count = 0;

        for(int i=end-1; i>=start; i--){
            if(isSym(pfForm[i])){
                count += 1;
            }
            else if(pfForm[i]!='!'){
                count -= 1;
            }
            if(count == -1){
                et->rightChild = ETF(pfForm, i, end-1);
                et->leftChild = ETF(pfForm, start, i-1);
                return et;
            }
        }
    }
    return et;
}

// printing the expresion tree using inorder traversal
void ETP (BT *et)
{
    // WRITE YOUR CODE HERE ...
    if (et != NULL)
    {
        if(isSym(et->element)){
            cout<<"( ";
            ETP(et->leftChild);
            cout << et->element<<" ";
            ETP(et->rightChild);
            cout<<") ";
        }
        else{
            ETP(et->leftChild);
            cout << et->element<<" ";
            ETP(et->rightChild);
        }
    }
}

// evaluate the formula from the expression tree from the proposition values provided in varr[] array
int EVAL (BT *et, PV *pvarr, int x)
{
    int result;

    // WRITE YOUR CODE HERE ...
    if(isProposition(et->element)){
        for(int i=0; i<x; i++){
            if(et->element == pvarr[i].prop)
                result = pvarr[i].val;
        }
    }
    if(et->element == '&'){
        return (EVAL(et->leftChild, pvarr, x)&&EVAL(et->rightChild, pvarr, x));
    }
    if(et->element == '|'){
        return (EVAL(et->leftChild, pvarr, x)||EVAL(et->rightChild, pvarr, x));
    }
    if(et->element == '-'){
        return !(EVAL(et->leftChild, pvarr, x)&&(!EVAL(et->rightChild, pvarr, x)));
    }
    if(et->element == '~'){
        bool var1 = (EVAL(et->leftChild, pvarr, x)&&EVAL(et->rightChild, pvarr, x));
        bool var2 = (!EVAL(et->leftChild, pvarr, x))&&(!EVAL(et->rightChild, pvarr, x));
        return (var1||var2);
    }
    if(et->element == '!'){
        return !EVAL(et->rightChild, pvarr, x);
    }

    return result;
}

// convert expression tree to IFF expression tree
BT *IFF (BT *et)
{
    // WRITE YOUR CODE HERE ...
    if(et->element == '!'){
        et->rightChild = IFF(et->rightChild);
    }
    if(et->element == '&' || et->element == '|'){
        et->rightChild = IFF(et->rightChild);
        et->leftChild = IFF(et->leftChild);
    }
    if(et->element == '-'){
        et->element = '|';
        BT* temp = createNode('!');
        temp->rightChild = IFF(et->leftChild);
        et->leftChild = temp;
        et->rightChild = IFF(et->rightChild);
    }
    if(et->element == '~'){
        BT* temp1 = createNode('-');
        BT* temp2 = createNode('-');
        temp1->leftChild = et->leftChild;
        temp1->rightChild = et->rightChild;
        temp2->leftChild = duplicate(et->rightChild);
        temp2->rightChild = duplicate(et->leftChild);
        et->element = '&';
        et->leftChild = IFF(temp1);
        et->rightChild = IFF(temp2);
    }

    return et;
}

// convert IFF expression tree to NNF expression tree
BT *NNF (BT *et)
{
    // WRITE YOUR CODE HERE ...
    if(et->element == '&' || et->element == '|'){
        et->rightChild = NNF(et->rightChild);
        et->leftChild = NNF(et->leftChild);
    }
    if(et->element == '!'){
        if(et->rightChild->element == '!'){
            et = (et->rightChild)->rightChild;
        }
        else if(et->rightChild->element == '&'){
            BT* temp = et->rightChild;
            et->element = '|';
            et->leftChild = createNode('!');
            et->leftChild->rightChild = temp->leftChild;
            et->rightChild = createNode('!');
            et->rightChild->rightChild = temp->rightChild;
            et->leftChild = NNF(et->leftChild);
            et->rightChild = NNF(et->rightChild);
        }
        else if(et->rightChild->element == '|'){
            BT* temp = et->rightChild;
            et->element = '&';
            et->leftChild = createNode('!');
            et->leftChild->rightChild = temp->leftChild;
            et->rightChild = createNode('!');
            et->rightChild->rightChild = temp->rightChild;
            et->leftChild = NNF(et->leftChild);
            et->rightChild = NNF(et->rightChild);
        }
    }
    return et;
}

// convert NNF expression tree to CNF expression tree
BT *CNF (BT *et)
{
    // WRITE YOUR CODE HERE ...
    if(et->element == '&'){
        et->rightChild = CNF(et->rightChild);
        et->leftChild = CNF(et->leftChild);
    }
    if(et->element == '|'){
        if(et->leftChild->element != '&' && et->rightChild->element != '&'){
            et->rightChild = CNF(et->rightChild);
            et->leftChild = CNF(et->leftChild);
        }
    }
    if(et->element == '|'){
        if(et->rightChild->element == '&'){
            BT* n1 = et->leftChild;
            BT* n21 = et->rightChild->leftChild;
            BT* n22 = et->rightChild->rightChild;
            et->element = '&';
            et->rightChild = createNode('|');
            et->leftChild = createNode('|');
            et->leftChild->leftChild = n1;
            et->leftChild->rightChild = n21;
            et->rightChild->leftChild = duplicate(n1);
            et->rightChild->rightChild = n22;
            et->rightChild = CNF(et->rightChild);
            et->leftChild = CNF(et->leftChild);
        }
    }
    if(et->element == '|'){
        if(et->leftChild->element == '&'){
            BT* n11 = et->leftChild->leftChild;
            BT* n12 = et->leftChild->rightChild;
            BT* n2 = et->rightChild;
            et->element = '&';
            et->rightChild = createNode('|');
            et->leftChild = createNode('|');
            et->leftChild->leftChild = n11;
            et->leftChild->rightChild = n2;
            et->rightChild->leftChild = n12;
            et->rightChild->rightChild = duplicate(n2);
            et->rightChild = CNF(et->rightChild);
            et->leftChild = CNF(et->leftChild);
        }
    }
    if(et->leftChild != NULL){
        et->leftChild = CNF(et->leftChild);
    }
    if(et->rightChild != NULL){
        et->rightChild = CNF(et->rightChild);
    }
    return et;
}

// convert NNF expression tree to DNF expression tree
BT *DNF (BT *et)
{
    // WRITE YOUR CODE HERE ...
    if(et->element == '|'){
        et->rightChild = DNF(et->rightChild);
        et->leftChild = DNF(et->leftChild);
    }
    if(et->element == '&'){
        if(et->leftChild->element != '|' && et->rightChild->element != '|'){
            et->rightChild = DNF(et->rightChild);
            et->leftChild = DNF(et->leftChild);
        }
    }
    if(et->element == '&'){
        if(et->rightChild->element == '|'){
            BT* n1 = et->leftChild;
            BT* n21 = et->rightChild->leftChild;
            BT* n22 = et->rightChild->rightChild;
            et->element = '|';
            et->rightChild = createNode('&');
            et->leftChild = createNode('&');
            et->leftChild->leftChild = duplicate(n1);
            et->leftChild->rightChild = n21;
            et->rightChild->leftChild = duplicate(n1);
            et->rightChild->rightChild = n22;
            et->rightChild = DNF(et->rightChild);
            et->leftChild = DNF(et->leftChild);
        }
    }
    if(et->element == '&'){
        if(et->leftChild->element == '|'){
            BT* n11 = et->leftChild->leftChild;
            BT* n12 = et->leftChild->rightChild;
            BT* n2 = et->rightChild;
            et->element = '|';
            et->rightChild = createNode('&');
            et->leftChild = createNode('&');
            et->leftChild->leftChild = n11;
            et->leftChild->rightChild = duplicate(n2);
            et->rightChild->leftChild = n12;
            et->rightChild->rightChild = duplicate(n2);
            et->rightChild = DNF(et->rightChild);
            et->leftChild = DNF(et->leftChild);
        }
    }
    return et;
}

// exhaustive search for checking validity / satisfiability
void helper(BT* et, PV* pv, int x, int i, int &valid, int &sat){
    if(i == x){
        cout<<endl;
        cout<<"{ ";
        for(int i=0; i<x; i++){
            cout<<"("<<pv[i].prop<<"=";
            cout<<pv[i].val<<") ";
        }
        cout<<"} : ";
        int ev = EVAL(et, pv, x);
        if(ev) sat = 1;
        if(!ev) valid = 0;
        cout<<ev;
        return;
    }
    pv[i].val = 0;
    helper(et, pv, x, i+1, valid, sat);
    pv[i].val = 1;
    helper(et, pv, x, i+1, valid, sat);
    return;
}

void CHECK (BT *et)
{
    int valid = 1, sat = 0;
    // WRITE YOUR CODE HERE ...
    int x;
    cout<<"\nEnter Number of Propositions:";
    cin>>x;
    PV *pv;
    pv = (PV *)malloc(x * sizeof(PV));
    cout<<"\nEnter Proposition Names (<SPACE> Separated):";
    cout<<"\nEvaluations of the Formula:";
    for (int i = 0; i < x; i++) {
        cin>>pv[i].prop;
    }
    helper(et, pv, x, 0, valid, sat);
    cout<<"\n";
    printResult(valid,sat);
}

/**********************************************************/
/******************* YOUR CODE ENDS HERE ******************/
/**********************************************************/

// main routine
int main ()
{
    char formula[MAX], *pfForm;
    int len, i;

    BT *et, *etI, *etN, *etDup, *etC, *etD;
    int *varr;
    PV *pvarr;
    int result;

    // scan propositional formula from user input
    printf("\nEnter Propositional Logic Formula: ");
    scanf("%[^\n]", formula);

    // internal formula string with ops as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    len = strlen(formula);
    for(i = 0; i < len; i++){
        if(formula[i] == '<'){ // denoting iff op (<->) using ~
            formula[i] = ' ';
            formula[i+1] = '~';
        }
        if(formula[i] == '>'){ // denoting imply op (->) using -
            formula[i] = ' ';
        }
    }

    // postfix form generation from represented formula string
    pfForm = genPostFixFormula(formula);

    // display postfix form of the internally represented formula
    displayPfForm(pfForm);

    // internal postfix formula string with ops as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    printf("\n++++ PostFix Format of the Propositional Formula ++++\n('-' used for '->' and '~' used for '<->')\n");
    printf("YOUR INPUT STRING: %s\n", pfForm);



    /**********************************************************/
    /********** YOUR CODE ENABLES THE FOLLOWING PARTS *********/
    /**********************************************************/

    printf("\n++++ Expression Tree Generation ++++");
    et = ETF(pfForm, 0, strlen(pfForm)-1);
    printf("\nOriginal Formula (from Expression Tree):");
    ETP(et);
    printf("\n");

    printf("\n++++ Expression Tree Evaluation ++++\n");
    int x;
    pvarr = scanPropValueInput(x);
    result = EVAL(et, pvarr, x);
    printf("\nThe Formula is Evaluated as: ");
    (result) ? printf("True\n") : printf("False\n");

    printf("\n++++ IFF Expression Tree Conversion ++++");
    etI = IFF(et);
    printf("\nFormula in Implication Free Form (IFF from Expression Tree):");
    ETP(etI);
    printf("\n");

    printf("\n++++ NNF Expression Tree Conversion ++++");
    etN = NNF(etI);
    printf("\nFormula in Negation Normal Form (NNF from Expression Tree):");
    ETP(etN);
    printf("\n");

    etDup = duplicate(etN); // keeping a duplicate copy for DNF conversion

    printf("\n++++ CNF Expression Tree Conversion ++++");
    etC = CNF(etN);
    printf("\nFormula in Conjunctive Normal Form (CNF from Expression Tree):");
    ETP(etC);
    printf("\n");

    printf("\n++++ DNF Expression Tree Conversion ++++");
    etD = DNF(etDup);
    printf("\nFormula in Disjunctive Normal Form (DNF from Expression Tree):");
    ETP(etD);
    printf("\n");

    printf("\n++++ Exhaustive Search from Expression Tree for Validity / Satisfiability Checking ++++");
    CHECK (et);
    /**********************************************************/


    return 0;
}
