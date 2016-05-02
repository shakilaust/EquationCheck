// BismillahirRahmanir Rahim
#include <bits/stdc++.h>
using namespace std ;

string PostfixExpression , Expression ;

struct Node
{
    Node *leftChild , *rightChild , *parent;
    double value;
    bool isOperator;
    string iam; // who i am 
    Node()
    {
        leftChild = NULL;
        rightChild = NULL ;
        parent = NULL ;
    }

};
Node *root1 , *root2 ;

bool Operator( char c )
{
    if( c == '(' || c == ')' || c == '^' || c == '/' || c == '+' || c == '-' || c == '*') return 1;
    else return 0;
}


int Value_of_Operator( char ch)
{
    if( ch == '(' || ch == ')' ) return 1;
    else if( ch == '+' || ch == '-' ) return 2;
    else if( ch == '*' ) return 3 ;
    else if( ch == '/' ) return 4 ;
    else return 5 ; // '^' highest priority
}

double toDouble ( string s )
{
    double r = 0 ;
    istringstream sin(s);
    sin >> r ;
    return r;
}
string ExpressionToPostfixExpression( string inp )
{
    string res = "";
    int sz = inp.size();
    stack < char > op;
    stringstream ss ;
    ss << inp ;
    string now ;
    while( ss >> now )
    {

        if( Operator(now[0]) && now.size() == 1 )
        {
            if( now[0] == ')')
            {
                while( op.top() != '(' )
                {
                    res += ( op.top());
                    res += ' ';
                    op.pop();
                }
                op.pop();
            }
        else {
            while( !op.empty() && now[0] != '(' && Value_of_Operator(op.top()) > Value_of_Operator(now[0]))
            {
                res += ( op.top() );
                res += ' ';
                op.pop();
            }

            op.push(now[0]);
            }
        }
        else res += ( now + ' ' );
    }
    while( !op.empty() )
    {

        res += ( op.top());
        res += ' ';
        op.pop();
    }

    return res ;

}


Node *ConvertIntoTree( string expression )
{
    Node *head = new Node();
    int sz = expression.size();
    stringstream ss ;
    ss << expression ;
    string now ;
    stack < Node * > st ;
    while( ss >> now )
    {
        if( Operator( now[0] ) && now.size() == 1 )
        {
            Node *right = st.top();
            st.pop();
            Node *left = st.top();
            st.pop();
            head = new Node();
            left->parent = head ;
            right->parent = head ;
            head->leftChild = left;
            head->rightChild = right;
            if( now[0] == '*' ) head->value = ( left->value * right->value);
            else if( now[0] == '/' ) head->value = ( left->value)/(right->value);
            else if( now[0] == '+' ) head->value = ( left->value) + ( right->value);
            else if( now[0] == '-' ) head->value = ( left->value) - (right->value);
            else head->value = pow((left->value),right->value);
            head->iam = now ;
            head->isOperator = 1 ;
            st.push(head);
        }
        else
        {
            head = new Node();
            head->value = toDouble( now );
            head->isOperator = 0 ;
            head->iam = now ;
            st.push(head);
        }
    }
    return head;
}

// for checking
void treeTraverse( Node *now )
{
    cout << now->iam << " (" << now->value << " )" << endl ;
    if( now->leftChild != NULL )
    {
        cout << "   ( leftchild )   " << endl ;
        treeTraverse( now->leftChild );
    }
    if( now->rightChild != NULL )
    {
        cout << "  ( rightchild)   " << endl ;
        treeTraverse( now->rightChild );
    }
}

bool update , notok ;

double value_check( string now , double val1 , double val2)
{
    if( now[0] == '*' ) return val1 * val2 ;
    else if( now[0] == '/' ) return val1 / val2 ;
    else if( now[0] == '+' ) return val1 + val2 ;
    else if( now[0] == '-' ) return val1 - val2 ;
    else if( now[0] == '^' ) return pow( val1 , val2 );
    else return 0 ;
}

void TreeCheck( Node *root1 , Node *root2 )
{
    if( notok ) return ;
    if( root1->value != root2->value )
    {
       // printf(" now :: value root2->value :: %f root1-Value:: %f\n" , root2->value , root1->value);
        if( root2->isOperator == true && root1->leftChild != NULL && root1->rightChild != NULL ) notok = !(root2->value == value_check( root2->iam , root1->rightChild->value , root1->leftChild->value));

        return ;
    }

    if( root2->leftChild != NULL && root1->leftChild != NULL ) TreeCheck( root1->leftChild , root2->leftChild );
    if( root2->rightChild != NULL && root1->rightChild != NULL ) TreeCheck(root1->rightChild , root2->rightChild);

}


int main()
{
    int n ;
    printf(" Number of Problem do you want to check ? ");
    cin >> n ;
    getchar();
    while( n-- ) {
    printf("Enter the Question ");
    getline( cin , Expression );
    printf(" Conver to Postfix : " );
    string exp = ExpressionToPostfixExpression(Expression) ;
    cout << exp << endl ;
    root1 = ConvertIntoTree( exp );

    printf(" Now solve it :\n");
    bool successful = 1 , complete = 0;
    int same = 0 ;
    string prv = exp ;
    int successfulline = 0;
    while( successful && !complete  )
    {
        getline( cin , Expression );
        update = 0 , notok = 0 ;
        exp = ExpressionToPostfixExpression(Expression);
        root2 = ConvertIntoTree( exp );
        TreeCheck( root1 , root2 );
        if( notok || ( root1->value != root2->value ) )
        {
            successful = 0 ;
            break ;
        }
        if( prv == exp ) same++;
        else
        {
           same = 0 ;
           successfulline++;
        }
        if( same >= 2 )
        {
            printf("What you are doing !!!! , typing same same line !!!\n");
        }
        if( same >= 5 )
        {
            successful = 0 ;
            break;
        }
        if( root2->leftChild == NULL && root2->rightChild == NULL )
        {
            complete = 1 ;
        }
        root1 = root2 ;
        prv = exp ;
    }

    if( successful ) {
        if( successfulline < 2 ) printf("What you are doing !!! memorizing answer !!! very bad -_- \n");
        else printf("Yes Pro!!! you have successfully solved it\n");
    }
    else printf("Ohh no, you just did something wrong\n");


    }


}
