#include "diccionari.hpp"

//Inicializamos que para cualquier nodo inicial esté vacío
diccionari::node::node(const char &c, node* pf, node* sg): _c(c), _pf(pf), _sg(sg), _es_paraula(false), _npal(0){}

//Pedimos memoria y como una palabra vacia tambien cuenta como palabra
//le asignamos al nodo raiz padre como el primer nodo existente valido
diccionari::diccionari() : _arrel(new node('\0'))
{
    _arrel->_es_paraula = true;
    _arrel->_npal = 1;
}

//Coste: n = numero de palabras, L = longitud maxima del conjunto de palabras, 
//Por lo que el coste es O(N * L) 
//Leemos flujo de datos y lo insertamos en nuestro diccionario. Delegamos constructor
diccionari::diccionari(istream& is) : diccionari()
{
    string s;
    //Leemos el flujo de datos donde cada palabra se guarda en s y se inserta
    while(getline(is, s))
    {
        insereix(s);
    }
}

//En la propia lista de inicializacion iniciamos que _arrel = copia(D._arrel)
diccionari::diccionari(const diccionari& D) : _arrel(copia(D._arrel)) {}  

diccionari& diccionari::operator=(const diccionari& D)
{
    //Si el diccionario que se quiere asignar no es la que tenemos actualmente, entonces hacemos cositas
    if(this != &D)
    {
        //Creamos una copia de ese diccionario D
        node* copia_arbre = copia(D._arrel);
        //Borramos el actual
        esborra_nodes(_arrel);
        //La raiz desreferenciado ahora apunta a la copia, es decir, la nueva
        _arrel = copia_arbre;
    }
    return *this;
}

diccionari::~diccionari() noexcept
{
    esborra_nodes(_arrel);
}

void diccionari::insereix(const string& p)
{
    if(not p.empty())
    {
        bool es_nova = false;
        _arrel->_pf = insereix_nodes(_arrel->_pf, 0, p, es_nova);

        if(es_nova)
        {
            ++_arrel->_npal;
        }
    }
}
// Devuelve la palabra más larga del diccionario que es prefijo de p.
// Se baja por el trie siguiendo las letras de p; en cada nivel se busca
// p[i] entre los hermanos (ordenados), y se recuerda la longitud del último
// nodo del camino que marca fin de palabra. Si no hay ninguno, devuelve ""
// (la palabra vacía, que siempre está en el diccionario).
// Coste: O(L), siendo L = p.size() (hasta 26 hermanos por nivel).
string diccionari::prefix(const string& p) const
{
    const node* it = _arrel->_pf;
    nat i = 0;
    nat llarg = 0;
    bool continua = true;
    while(continua and it != nullptr and i < p.size())
    {
        while(it != nullptr and it->_c < p[i])
        {   //El caracter es mas grande que el actual, es decir, debe de estar hacia la derecha(si hay)
            it = it->_sg;
        }
        if(it != nullptr and it->_c == p[i])
        {//Encontramos el caracter, si hasta este extremo es una palabra, margamos la longitud
            if (it->_es_paraula)
            {
                llarg = i + 1;
            } //bajamos al siguiente nivel de nuestro trie pf-sg
            it = it->_pf;
            ++i;
        }
        else
        { //Si El caracter es mas PEQUEÑO que el caracter del nodo actual, quiere decir que no existe en el arbol mas caracteres que coincidan.
            continua = false;
        }
    }
    string res;
    //Aqui recorremos la palabra para guardar en res hasta CUÁNTO se ha podido recorrer el prefix.
    for(nat j = 0; j < llarg; ++j)
    {
        res += p[j];
    }
    return res;
}
  
void diccionari::satisfan_patro(const vector<string>& q, list<string>& L) const
{//Vaciamos lista por si tiene basura
    L.clear();
    if(q.empty())
    { 
        L.push_back("");
    }
    else
    { /*
        Creamos una tabla de booleanos, en el que cada fila es un patron, y cada columna una letra de la A a la Z. De forma que 
        todos estan inicializados en false. SI es true significa que ese caracter forma parte del patron de la fila en concreto
        Ej: A..Z, A..Z, AEIOU, S.  Tendriamos una q.size() = 4, en el que para q[0] y q[1] todas las columnas de estas filas serian true, 
                                    mientras que en la fila q[2] solo estarian a true las columnas A, E, I, O ,U. Y en el caso final solo 
                                    la columna S.
        */
        vector<vector<bool>> taula(q.size(), vector<bool>(26, false));
        for(nat i = 0; i < q.size(); ++i)
        {
            for(nat j = 0; j < q[i].size(); ++j)
            {
                //Recorremos la tabla para establecer los patrones.  
                //Fila i, palabra i, caracter j de la palabra i
                taula[i][q[i][j] - 'A'] = true;
            }
        }
        string paraula;
        //En paraula se guarda una palabra encontrada, en L la lista, en 0 el nivel en que esta actualmente(o en cuál caracter de la palabra esta actualmente)
        satisfan_patro_nodes(_arrel->_pf, 0, taula, paraula, L);
    }
}

// Deja en L las palabras del diccionario de longitud >= k, en orden
// alfabético. Recorrido en preorden del trie: la palabra de un nodo se
// guarda antes que las de sus hijos, y los hermanos se visitan en orden.
// La palabra vacía (la raíz) solo se incluye si k == 0.
// Coste: O(N), siendo N el número de nodos del trie.
void diccionari::llista_paraules(nat k, list<string>& L) const
{
    L.clear();
    string paraula = "";
    if(k == 0)
    {
        L.push_back(paraula);
    }
    else 
    {
        llista_paraules_nodes(_arrel->_pf, k, paraula, L);
    }
    return;
}

nat diccionari::num_pal() const noexcept
{
    return _arrel->_npal;
}

string diccionari::operator[](nat i) const
{
    if (i >= num_pal())
    {
        throw error(INDEX_INCORRECTE);
    }
    const node* t = _arrel;
    string res;
    bool trobat = false;
    while (not trobat)
    {
        if (t->_es_paraula)
        {   //Si la palabra buscada esta en la 0, devolvemos instantaneamente
            if (i == 0)
            {
                trobat = true;
            }
            else
            {
                --i;
            }
        }
        if (not trobat)
        { /*
            Aqui hacemos busqueda con poda, como a cada nodo guardamos el numero de subnodos que tiene, podemos calcular rapidamente a 
            cual nodo debemos bajar, ej:  queremos la 9, analizamos el primer nodo y este tiene 5 hijos, 9 > 5 , es decir que auqnue visitemos 
            los hijos de este nodo, a ninguno le pertoca el 9, nisiquiera el 6 porque el maximo es el 5. asi pues saltamos al siguiente hermano
            no sin antes realizar la resta de 9 - 5 = 4. porque en cada nodo se realiza el mismo criterio de comprobar si en este caso seria 4 > nodo.
            Pongamos en el caso de que el segundo nodo tiene numero de hijos 6, entonces 4 < 6, por lo que el hijo pertenece al subarbol del nodo actual.
            
            Guardamos el caracter del nivel actual, de forma iterativa indicamos que vamos a iterar en el nuevo nodo, es decir, el hijo del actual.
            En el caso nuestro t = h, en el que t incialmente era la raiz y h su hijo, posteriormente t = nodo actual, y h el hijo del nodo actual
            */
            const node* h = t->_pf;
            while (i >= h->_npal)
            {
                i -= h->_npal;
                h = h->_sg;
            }
            res += h->_c;
            t = h;
        }
    }
    return res;
}


diccionari::node* diccionari::copia(const node* t)
{ //Si el nodo a copiar esta vacio, no lo copiamos
    if(t == nullptr)
    {
        return nullptr;
    }
    //En caso contrario pedimos memoria y guadamos la clave, y sus demas atributos
    node* n = new node(t->_c);
    
    n->_es_paraula = t->_es_paraula;
    n->_npal = t->_npal;

    //LLamadas recursivas tanto por hijo como hermano siguiendo la misma logica anterior 
    n->_pf = copia(t->_pf);
    n->_sg = copia(t->_sg);
    //Cuando lleguemos al final se empezara a hacer un return encadenado y se conectaran todos
    return n;
}

void diccionari::esborra_nodes(node* t)
{
    if(t != nullptr)
    {
        esborra_nodes(t->_pf);
        esborra_nodes(t->_sg);
        delete t;
    }
}

// Pre: t es la lista de hermanos de la posición i; i < k.size().
// Post: devuelve la lista de hermanos (ordenada) con k[i] colocada y el resto
//       de k insertado por debajo. es_nova indica si k no estaba en el
//       diccionario; si lo es, se suma 1 al _npal de los nodos del camino de k.
// Coste: O(L), siendo L = k.size() (hasta 26 hermanos por nivel).
diccionari::node* diccionari::insereix_nodes(node* t, nat i, const string& k, bool& es_nova)
{
    if(t != nullptr and t->_c < k[i])
    {
        //Si es un caracter superior al actual nodo, es decir a la derecha, iteramos los hermanos e insertamos a la derecha
        t->_sg = insereix_nodes(t->_sg, i, k, es_nova);
    }
    else
    {
        /*Si el nodo del caracter no existe o estaria a la izquierda del nodo actual, insertamos nuevo nodo.
        
        En el segundo caso lo convertimos en elnuevo referente del nivel que encabeza. 
        
        Caracter que queremos insertar: 'B'
        Nivel actual                    [C]->[D]->[F]->[H]->nullptr
                                       <<t>>
        Nivel final:        creamos [B], decimos que [b]->[C]->[D]->[F]->[H]->nullptr, y finalmente apuntamos <<t>> a [B] tal que  [b]->[C]->[D]->[F]->[H]->nullptr,
                                                         <<t>>                                                                    <<t>>
        En el primer caso tras crear el nuevo nodo, si termina la palabra, devuelve el nodo siendo apuntado por el
        ultimo nodo del nivel existente tal que:
                       [C]->[D]->[F]->[H]->llamad_funcion(t->_sg), donde creamos el nodo [I] y hacemos return [I].
                                               
                                                Estado inicial: [C]->[D]->[F]->[H]->nullptr 
                                                Estado final:   [C]->[D]->[F]->[H]->[I]
        */                                          
        if(t == nullptr or t->_c > k[i])
        {
            t = new node(k[i], nullptr, t);
        }
        //Si llegamos al final de la palabra por insertar
        if(i == k.size() - 1)
        {
            es_nova = true;
            t->_es_paraula = true;
        }
        else
        {
            //Si todavia quedan caracteres por insertar de la palabra, seguimos bajando
            t->_pf = insereix_nodes(t->_pf, i + 1, k, es_nova);
        }
        if(es_nova)
        {
            ++t->_npal;
        }
    }
    return t;
}

// Pre: t es la lista de hermanos de la posición i; i < taula.size();
//      paraula contiene las i letras del camino hasta t.
// Post: añade al final de L, en orden alfabético, las palabras de los
//       subárboles de t que satisfacen el patrón desde la posición i.
//       paraula queda como estaba al empezar.
// Coste: O(V), siendo V el número de nodos visitados.
void diccionari::satisfan_patro_nodes(const node* t, nat i, const vector<vector<bool>>& taula, string& paraula, list<string>& L)
{
    while(t != nullptr)
    {   //Si el caracter del nodo actual pertenece true a la tabla de la fila actual i:
        if(taula[i][t->_c - 'A'])
        { //Guardamos caracter
            paraula.push_back(t->_c);
            if(i == taula.size() - 1)
            { //Si cumple todos los patrones y acaba en palabra
                if(t->_es_paraula)
                { //Guardamos
                    L.push_back(paraula);
                }
            }
            else
            { // Si no hemos terminado seguimos bajando a sus hijos
                satisfan_patro_nodes(t->_pf, i + 1, taula, paraula, L);
            }
            //Al volver al nivel superior, no debemos guardar el caracter del nivel inferior porque vamos a continuacion a visitar su hermano
            paraula.pop_back();
        }
        t = t->_sg;
    }
}


// Pre: t es la lista de hermanos de la posición paraula.size();
//      paraula contiene las letras del camino hasta t.
// Post: añade al final de L, en orden alfabético, las palabras de longitud
//       >= k de los subárboles de t y de sus hermanos. paraula queda como
//       estaba al empezar.
// Coste: O(n), siendo n el número de nodos de esos subárboles.
void diccionari::llista_paraules_nodes(node* t, nat k, string paraula, list<string>& L)
{
    if(t != nullptr)
    {
        paraula.push_back(t->_c);
        if(t->_es_paraula and paraula.size() >= k)
        {
            L.push_back(paraula);
        }
        llista_paraules_nodes(t->_pf, k - 1, paraula, L);
        paraula.pop_back();
        llista_paraules_nodes(t->_sg, k, paraula, L);
    }
}
