#include "word_toolkit.hpp"
#include <vector>

//Retorna cert si i només si les lletres de l’string s estan en ordre lexicogràfic ascendent.
//Coste: O(n) lineal donde n es el tamaño de la palabra s
bool word_toolkit::es_canonic(const string& s) noexcept
{
    bool compleix = true;

    unsigned int i = 0;
    
    while(compleix and i + 1 < s.size())
    {
        /*
            La condicion del while cubre el caso en el que la palabra este vacia
            o contenga solo una letra, el cual consideraremos true. 

            Si la letra actual es mayor que la letra siguiente, entonces no cumple
            que sea lexicograficamente ascendente

            ej: ADC ->   D > C, entonces es false. s[1] > s[2]
        */
        if(s[i] > s[i + 1]) compleix = false;
        else i++;
    }
    return compleix;
}

/*
Retorna el caràcter que no apareix a l’string excl i és el més freqüent en la llista de paraules
L, sent L una llista no buida de paraules formades exclusivament amb lletres majúscules de
la ’A’ a la ’Z’—excloses la ’Ñ’, ’Ç’, majúscules accentuades, .... En cas d’empat, retorna
el caràcter alfabèticament menor. Si l’string excl inclou totes les lletres de la ’A’ a la ’Z’
es retorna el caràcter ’\0’, és a dir, el caràcter de codi ASCII 0.
*/
//Coste: e(numero de letras excluidas) que acceden a vfrequencia, accesos O(1), mientras que para recorrer todas las palabras de la lista L es de m(longitud de la palabra) * n(numero de palabras de la lista)
//El coste total es la suma del coste de e + m * n, donde e es size de excl, m es el size de una palabra de la lista y n es la cantidad de la palabras de la lista
char word_toolkit::mes_frequent(const string& excl, const list<string>& L) noexcept
{
    //Creamos un vector de 26 casillas, lo que ocuparia de la A..Z, todos a 0
    vector<int>vfrequencia(26,0);

    for(unsigned int i = 0; i < excl.size(); i++)
    {
        //Asignamos -1 como nuestro flag para indicar casilla invalida/excluida a las posiciones del vector.
        //hacemos la operacion aritmetica de caracter - 'A' para calcular de manera ascii la posicion de 0 a 26, que coincide con las posiciones de la A a la Z
        vfrequencia[excl[i] - 'A'] = -1;
    }
    
    for(list<string>::const_iterator it = L.begin(); it != L.end(); ++it)
    {
        //En cada iteracion de la lista cogemos el objeto tipo string apuntado actual
        const string& p = *it;
        //Recorremos cada caracter de la palabra en nuestro vector de frecuencia, en el que incrementamos +1 a cada posicion que pertenezca al caracter comparado, 
        //exceptuando a las excluidas
        for(unsigned int i = 0; i < p.size(); i++)
        {
            int posVector = p[i] - 'A';
            //Si no es excluida le sumamos +1
            if(vfrequencia[posVector] != -1)
            {
                ++vfrequencia[posVector];
            }
        }
    }
    //Recorremos todo el vector frecuencia en busca de la casilla con el valor mas alto, 
    // tal que ej:   [-1][4][0][1][2][5] 
    // Recorreriamos todo el vector comparando, si en dicha casilla el valor es mayor, guardamos su NUMERO DE CASILLA respecto al VECTOR y la informacion de la casilla.
    char cMax = '\0';
    int fMax = -1;

    for(unsigned int i = 0; i < vfrequencia.size(); ++i)
    {
        if(vfrequencia[i] > fMax)
        {
            fMax = vfrequencia[i];
            //Pasar de nuevo de numero int a caracter,  el valor de 'A' en ascii y le incrementamos i, donde i es una de las posiciones del vector frecuencia que traducido seria una letra de la A a la Z
            cMax = 'A' + i;
        }
    }
    return cMax;
}
