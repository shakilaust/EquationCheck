
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
Node *root1 , *root2 , *root3 , *root4 , *MainRoot ;

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

struct VAR{
	float i;
	VAR() {}
};

struct LINE{//k*x+a
	double a, k;
	VAR* x;

	LINE(){}
	LINE(double a) : a(a), k(0), x(0) {}
	LINE(double xx, VAR& v) : a(0), k(xx), x(&v) {}
};

LINE ADD (LINE A, LINE B){//assumes that A.x == 0 or B.x == 0 or A.x == B.x
	LINE R;
	R.a = A.a + B.a;
	R.k = A.k + B.k;
	if(A.x) R.x = A.x;
	else R.x = B.x;
	return R;
}

LINE MINUS (LINE A, LINE B){//same as +
	LINE R;
	R.a = A.a - B.a;
	R.k = A.k - B.k;
	if(A.x) R.x = A.x;
	else R.x = B.x;
	return R;
}

LINE MULTIPLICATION (LINE A, LINE B){//assumes that A.x == 0 or B.x == 0
	LINE R;
	R.a = A.a * B.a;
	R.k = A.k * B.a + B.k * A.a;
	if(A.x) R.x = A.x;
	else R.x = B.x;
	return R;
}

LINE DIVIDE (LINE A, LINE B){//assumes that B.x == 0
	LINE R;
	R.a = A.a / B.a;
	R.k = A.k / B.a;
	R.x = A.x;
	return R;
}

float EQUAL (LINE A, LINE B){
	LINE C = MINUS( A , B ) ;
	//cout << setprecision(10) << "A.a : " << A.a << " A.k : " << A.k << " B.a : " << B.a << " B.k :" << B.k << endl ;
	//cout << setprecision(10) << " C.a : " << C.a << " C.k : " << C.k << endl ;

	return C.x->i = -C.a/C.k;
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
        cout << "   ( leftChild )   " << endl ;
        treeTraverse( now->leftChild );
    }
    if( now->rightChild != NULL )
    {
        cout << "  ( rightChild)   " << endl ;
        treeTraverse( now->rightChild );
    }
}

int isItSideChange( Node *now , string iam , double value )
{

        if( now->iam == iam )
        {
             if( now->leftChild != NULL && now->leftChild->value == value ) return 1 ;
             if( now->rightChild != NULL && now->rightChild->value == value ) return 1;
        }
        if( now->leftChild != NULL && isItSideChange( now->leftChild , iam , value ) == 1 ) return 1 ;
        if( now->rightChild != NULL && isItSideChange( now->rightChild , iam , value ) == 1 ) return 1 ;
        return 0 ;

}

int isSideChangeOk( Node *one , Node *two )
{


    if( one->leftChild == NULL && one->rightChild == NULL && two->leftChild == NULL && two->rightChild == NULL) return 1 ;
    if( one->leftChild != NULL && one->rightChild != NULL && two->leftChild != NULL && two->rightChild != NULL )
    {
        int a = isSideChangeOk( one->leftChild , two->leftChild );
        int b = isSideChangeOk( one->rightChild , two->rightChild );
        printf("here a , b :: %d %d\n",a,b);
        return min( a , b );
    }
    // Left child included
    if( one->leftChild == NULL && two->leftChild != NULL )
    {
        printf("Left CHild \n" );
        cout << two->iam << "   and value " << two->leftChild->value << endl ;
        if( two->iam == "+" ) // minus silo
        return isItSideChange( root1 , "-" , two->leftChild->value );
        else if( two->iam == "-" ) // plus silo
        return isItSideChange( root1 , "+" , two->leftChild->value );
        else if( two->iam == "*" ) // divide silo
        return isItSideChange( root1 , "/" , two->leftChild->value );
        else if( two->iam == "/" ) // plus silo
        return isItSideChange( root1 , "*" , two->leftChild->value );
    }
    // Right Child included
     if( one->rightChild == NULL && two->rightChild != NULL )
    {
        printf("Right CHILD\n");
        if( two->iam == "+" ) // minus silo
        return isItSideChange( root1 , "-" , two->rightChild->value );
        else if( two->iam  == "-" ) // plus silo
        return isItSideChange( root1 , "+" , two->rightChild->value );
        else if( two->iam  == "*" ) // divide silo
        return isItSideChange( root1 , "/" , two->rightChild->value);
        else if( two->iam  == "/" ) // plus silo
        return isItSideChange( root1 , "*" , two->rightChild->value );
    }
    // leftChild reduce
     if( one->leftChild != NULL && two->leftChild == NULL )
    {
        if( one->iam == "+" ) // minus hoiche
        return isItSideChange( root2 , "-" , one->leftChild->value );
        else if( one->iam == "-" ) // plus hoiche
        return isItSideChange( root2 , "+" , one->leftChild->value );
        else if( one->iam == "*" ) // divide hoiche
        return isItSideChange( root2 , "/" , one->leftChild->value );
        else if( one->iam == "/" ) // plus hoiche
        return isItSideChange( root2 , "*" , one->leftChild->value );
    }
    // rChildight reduce
     if( one->rightChild != NULL && two->rightChild == NULL )
    {
        if( one->iam == "+" ) // minus hoiche
        return isItSideChange( root2 , "-" , one->rightChild->value );
        else if( one->iam == "-" ) // plus hoiche
        return isItSideChange( root2 , "+" , one->rightChild->value );
        else if( one->iam == "*" ) // divide hoiche
        return isItSideChange( root2 , "/" , one->rightChild->value );
        else if( one->iam == "/" ) // plus hoiche
        return isItSideChange( root2 , "*" , one->rightChild->value );
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

bool Isnumber( string one)
{
    if( one[0] >= '0' && one[0] <= '9' ) return 1 ;
    else if( one.size() > 1 && one[1] >= '0' && one[1] <= '9' ) return 1 ;
    else return 0 ;
}

float toFloat ( string s )
{
    float r = 0 ;
    istringstream sin(s);
    sin >> r ;
    return r;
}

string toString(double number){
stringstream ss;
ss << number;
return ss.str();
}

LINE WhatIsTheValueOfLine( string exp )
{
    VAR x ;
	stack < LINE > st ;
    stringstream ss ;
    ss << exp ;
    string now ;
    while( ss >> now )
    {
         if( Operator(now[0]) && now.size() == 1 )
         {

             LINE two = st.top();
             st.pop();
             LINE one = st.top();
             st.pop();
             if( now[0] == '+' ) st.push( ADD(one,two) );
             else if( now[0] == '-') st.push( MINUS(one,two));
             else if( now[0] == '*') st.push( MULTIPLICATION(one,two));
             else st.push( DIVIDE(one,two));
         }
         else
         {
            if( Isnumber(now) )
            {
                st.push( LINE(toDouble(now)) );
            }
            else
            {
                if( now[0] == 'x' ) // only small case letter
                {
                    st.push( LINE( 1 , x));
                }
                else st.push(LINE(-1 , x));


            }


        }
    }
    return st.top();

}


float valueOfx( LINE A , LINE B )
{
    return EQUAL( A , B );

}

string ChangeExp( string exp , double rpl )
{
    string res = "" ;
    int sz = exp.size();
    for( int i = 0 ; i < sz ; i++ )
    {
        if( exp[i] == 'x' ) // always small letter x
            res += toString(rpl);
        else res += exp[i];
    }

    return res;
}

string now[3] , ExpressionOne , ExpressionTwo ;

void SplitStrings( string Expression )
{
    int sz = Expression.size();
    now[0] = "" , now[1] = "";
    int idx = 0 ;
    for( int i = 0 ; i < sz ; i++ )
    {
        if( Expression[i] == '=' ) idx++;
        else now[idx] += Expression[i];
    }

    // Postfix order for Left SubTree and RightSubtree
    ExpressionOne = ExpressionToPostfixExpression( now[0] );
    ExpressionTwo = ExpressionToPostfixExpression( now[1] );

    cout << " Here exp1 " << ExpressionOne << "    exp2 " << ExpressionTwo << endl ;
}


bool CheaterChecking()
{
    notok = 0 ;
    TreeCheck( root1 , root2 );
    if( notok ) return false ;
    TreeCheck( root3 , root4 );
    if( notok ) return false ;
    else return true ;
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
    SplitStrings( Expression );

    double result = valueOfx(WhatIsTheValueOfLine(ExpressionOne ), WhatIsTheValueOfLine(ExpressionTwo) );



    printf(" Ohh !!! Yoo :: result is :: %f\n" , result);

    MainRoot = new Node();

    root1 = ConvertIntoTree( ChangeExp(ExpressionOne , result ) );
    root3 = ConvertIntoTree( ChangeExp(ExpressionTwo, result ) );
    printf(" Now solve it :\n");


    bool successful = 1 , complete = 0;
    int same = 0 ;
    string prv = Expression ;
    int successfulline = 0;
    while( successful && !complete  )
    {
        getline( cin , Expression );
        SplitStrings( Expression );

        double nowRes = valueOfx(WhatIsTheValueOfLine(ExpressionOne ), WhatIsTheValueOfLine(ExpressionTwo) );
        if( nowRes != result )
        {
            successful = 0;
            break;
        }



        root2 = ConvertIntoTree( ChangeExp(ExpressionOne , result ) );
        root4 = ConvertIntoTree( ChangeExp(ExpressionTwo, result ) );

        if( root1->value == root2->value ) // No slide change
        {
            successful = CheaterChecking();
            printf(" cheaterChecking successfull : %d\n" , successful );
        }
        else
        {
            successful = isSideChangeOk( root3 , root4 );

            printf(" isSideok successfull :: %d\n" , successful );
        }

        if( successful == 0 )
        {
            break ;
        }



        if( prv == Expression ) same++;
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
        if( root2->leftChild == NULL && root2->rightChild == NULL &&  root4->leftChild == NULL && root4->rightChild == NULL )
        {
            printf("root2->value :: %f root4->value :: %f \n", root2->value , root4->value );
            if( root2->value == root4->value && root4->value == result )
            {
                if( root2->iam == "x" || root4->iam == "x" ) complete = 1 ;
                else successful = 0 ;

            }
            else successful = 0 ;
            printf("At end here successfull %d\n", successful );
        }

        root1 = root2 ;
        root3 = root4 ;
        prv = Expression ;
    }

    if( successful ) {
        if( successfulline < 2 ) printf("What you are doing !!! memorizing answer !!! very bad -_- \n");
        else printf("Yes Pro!!! you have successfully solved it\n");
    }
    else printf("Ohh no, you just did something wrong\n");




    }


}
