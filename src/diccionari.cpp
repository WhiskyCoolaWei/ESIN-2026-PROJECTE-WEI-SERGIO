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
{

}                      
void diccionari::llista_paraules(nat k, list<string>& L) const
{

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
        {
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
        {
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
