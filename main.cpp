/*
Program Name: Shunting Yard Postfix
Date: 2023-11-10
Author: David Vu
Module Purpose:
This program implements a stack-based and tree-based implementation of returning infix expressions in postorder and preorder notations.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <stack>

using namespace std;

string inputFileNameStr = "expressions.txt";                  // Default location in solution folder

class OperatorMapClass {

private:
    typedef map<char, int>    OperatorPrecedenceMapType;
    OperatorPrecedenceMapType operatorMapObj;

public:

    OperatorMapClass() {
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('+', 1));
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('-', 1));
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('*', 2));
        operatorMapObj.insert(OperatorPrecedenceMapType::value_type('/', 2));
        // xxx insert code here to insert * and / in the map object
    }//OperatorMapClass ()

    bool isStackOperatorHigherPrecedence(char operatorChar, char operatorStackChar) {
        return((operatorMapObj.count(operatorStackChar))
               &&
               (operatorMapObj.at(operatorStackChar) >= operatorMapObj.at(operatorChar)));
    }//isStackOperatorHigherPrecedence()

    bool  isOperator(char token) {
        // xxx check if token operator Map Object is 0 or not to return true or false
        return operatorMapObj.find(token) != operatorMapObj.end();
        //return true; // dummy return

    }//isOperator()

};//OperatorMapClass

OperatorMapClass  operatorMapObj;

class ShuntingYardClass {

public:

    static string createPostFixFrom(const string& infixString) {

        string       outputString;
        stack <char> operatorStackObj;
        outputString = "";
        for (char token : infixString) {
            switch (token) {
                case '/':case '*':case '+':case '-':
                    // xxx insert code here
                    /*

                     while (the stack is not empty
                              AND
                            the top of the stack token is not a left parenthesis
                              AND
                            precedence of the current operator <= precedence of the top of the stack token
                           )
                     {
                          Push back the stack top token to the output string
                          Pop the stack top and discard it
                      }//while-end

                       Push the current operator token onto the stack
                    */
                    while(!operatorStackObj.empty() && operatorStackObj.top() != '(' && operatorMapObj.isStackOperatorHigherPrecedence(token, operatorStackObj.top())) {
                        outputString += operatorStackObj.top();
                        operatorStackObj.pop();
                    }
                    operatorStackObj.push(token);
                    break;
                case '(':                                                       // left parenthesis
                    // xxx insert code here
                    // push this token on the stack
                    operatorStackObj.push(token);
                    break;
                case ')':
                    // xxx insert code here                                                      // right parenthesis
                    /*

                    while (the stack is not empty AND the top stack token is not a left parenthesis)
                    {
                       Push back the stack top token to the output string
                    }//while-end

                    Pop the left parenthesis from the stack and discard it
                    */
                    while(!operatorStackObj.empty() && operatorStackObj.top() != '(') {
                        outputString += operatorStackObj.top();
                        operatorStackObj.pop();
                    }
                    operatorStackObj.pop();
                    break;
                default:                                                        // operand
                    // xxx insert code here
                    // push back the operand symbol to the output string
                    outputString += token;
                    break;
            }//switch
        }//for

        // xxx insert code here
        /*
         while (the stack is not empty)
         {
           //Push back any remaining stack tokens to the output string
        }//while-end
        */
        while(!operatorStackObj.empty()){
            outputString += operatorStackObj.top();
            operatorStackObj.pop();
        }
        return(outputString);

    }//postfix()

};//ShuntingYardClass



class TreeNodeClass {

public:
    TreeNodeClass* left;
    char           value;
    TreeNodeClass* right;

};//TreeNodeClass

TreeNodeClass* BuildNewNodeObjPtrMethod(char value) {

    // xxx new a new TreeNodeClass pointer
    // set value in new node and left and right pointers
    // return new node pointer
    auto* Node = new TreeNodeClass;
    Node->value = value;
    Node->left = nullptr;
    Node->right = nullptr;

    return Node;
}


TreeNodeClass* ConstructBET(const string& postFixStr) {
    stack<TreeNodeClass*>   parseStack;
    TreeNodeClass*          newNodePtr;
    OperatorMapClass        OperatorMapObj;

    // xxx must develop code here
    // Process each character of the post-fix expression into token
    //char token=' ';
    for (char token : postFixStr)
    {
        // Form a new node pointer
        newNodePtr = BuildNewNodeObjPtrMethod(token);
        // check if an operator
        if(OperatorMapObj.isOperator(newNodePtr->value))
        {
            // parse stack nodes into a new subtree as children
            // Save/Add this subtree node to the stack
            newNodePtr->right = parseStack.top();
            parseStack.pop();
            newNodePtr->left = parseStack.top();
            parseStack.pop();

            parseStack.push(newNodePtr);
        }
        // not operator
        else {
            // operand, push node onto stack
            parseStack.push(newNodePtr);
        }
    }

    //  Place formed root node on the stack into tree

    return parseStack.top();
}

string buildString;

void preorder(TreeNodeClass* treeNode) {
    //xxx do pre order transversal to build string
    if(treeNode == nullptr)
        return;

    buildString += treeNode->value;
    preorder(treeNode->left);
    preorder(treeNode->right);
}

bool areParensRequired(TreeNodeClass* treeNode, char value) {
    if (operatorMapObj.isOperator(value) &&
        operatorMapObj.isOperator(treeNode->value) &&
        operatorMapObj.isStackOperatorHigherPrecedence(treeNode->value, value)) {
        //buildString += '(';
        return true;
    }
    return false;
}

void inorder(TreeNodeClass* treeNode) {
    //xxx do in order transversal to build string
    if (treeNode) {
        //left
        bool leftParensRequired = areParensRequired(treeNode->left, treeNode->value);

        if (leftParensRequired)
            buildString += '(';
        inorder(treeNode->left);
        if (leftParensRequired)
            buildString += ')';

        buildString += treeNode->value;

        //right
        bool rightParensRequired = areParensRequired(treeNode->right, treeNode->value);

        if (rightParensRequired)
            buildString += '(';
        inorder(treeNode->right);
        if (rightParensRequired)
            buildString += ')';
    }//if
}

void postorder(TreeNodeClass* treeNode) {
    //xxx do post order transversal to build string
    if(treeNode == nullptr)
        return;
    postorder(treeNode->left);
    postorder(treeNode->right);
    buildString += treeNode->value;

}



int main() {

    ifstream  inputFileStreamObj(inputFileNameStr);

    if (inputFileStreamObj.fail()) {
        cout << "File could not be opened !" << endl;

        return (EXIT_FAILURE);
    }//if

    string  infixExpressionStr,
            postfixExpressionStr;

    while (inputFileStreamObj >> infixExpressionStr) {

        cout << "InFix   Expression : " << infixExpressionStr << endl;
        postfixExpressionStr = ShuntingYardClass::createPostFixFrom(infixExpressionStr);
        cout << "PostFix Expression : " << postfixExpressionStr << endl << endl;

        TreeNodeClass* expressionTreeRootPtr = ConstructBET(postfixExpressionStr);

        buildString = "";  preorder(expressionTreeRootPtr);
        cout << "Tree  pre-order expression is " << endl << buildString << endl << endl;

        buildString = "";  inorder(expressionTreeRootPtr);
        cout << "Tree   in-order expression is " << endl << buildString << endl << endl;

        buildString = "";  postorder(expressionTreeRootPtr);
        cout << "Tree post-order expression is " << endl << buildString << endl << endl;

        cout << endl << endl;

    }//while

    inputFileStreamObj.close();

    cout << "Press the enter key once or twice to end." << endl;

    return EXIT_SUCCESS;

}//main()

