principal ()
{
    @
        entero n, curr;
    @

    imprime("introduce un numero : ");
    leer(n);
    imprime(n, " == " );
    curr = 2;

    mientras(curr <= n){
        @
            entero d;
        @
        d = n / curr;

        si( d*curr == n)
        {
            imprime("* ", curr);
            n = n / curr;
        }
        sino
            curr = curr + 1;
    }
    imprime("\n");
}