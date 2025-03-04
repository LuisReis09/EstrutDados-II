

class NoSaltos<T>{
    protected T elemento;
    protected NoSaltos<T>[] proximo = null;

    @SuppressWarnings("unchecked")
    protected NoSaltos(T elemento, int nivel){
        this.elemento = elemento;
        proximo = new NoSaltos[nivel + 1];
        for(int i = 0; i < nivel; i++){
            proximo[i] = null;
        }
    } 
}
