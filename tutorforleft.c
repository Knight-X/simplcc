
int factor() 
{
    int value = 0;
    if (token == '(') {
        match('(');
        value = expr();
        match(')');
    } else {
        value = token_val;
        match(Num);
    }
    return value;
}


int term_tail(int lvalue)
{
    if (token == '*') {
        match('*');
        int value = lvalue * factor();
        return term_tail(value);

    } else if (token == '/') {
        match('/');
        int value = lvalue / factor();
        return term_tail(value);
    } else {
        return;
    }
int term()
{
    int lvalue = factor();
    return term_tail(lvalue);
}
int expr_tail(int lvalue)
{
    if (token == '+') {
        match('+');
        int value = lvalue + term();
        return expr_tail(tmp);
    } else if (token == '-') {
        match('-');
        int value = lvalue - term();
        return expr_tail(tmp);
    } else {
        return;
    }
}
int expr()
{
    int lvalue = term();
    return expr_tail(lvalue);
}


