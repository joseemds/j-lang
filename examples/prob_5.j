-- parâmetro r por referência?
func mdc(n : Int, m : Int) : Int {
    if (n == 0) {
        return m;
    }
    
    if (m == 0) {
        return n;
    }
    
    if (m > n) {
        return mdc(n, m % n);
    } else {
        return mdc(m, n % m);
    }
}

func main() : Void {
    val numero1 : Int = 48;
    val numero2 : Int = 18;
    val resultado : Int = mdc(numero1, numero2);
    
    print("MDC de " + int_to_string(numero1) + " e " + int_to_string(numero2) + " é: " + int_to_string(resultado));
}
