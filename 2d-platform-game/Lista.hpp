#pragma once
#ifndef NULL
#define NULL 0x0
#endif
////////////////////////////////////////////////////////

template <class TIPO>
class Lista {
private:
    template <class T> class ElLista;
    ElLista<TIPO>* _primElem;
    ElLista<TIPO>* _atualElem;
    int _n_infos;
    // ElLista/*{{{{*/
    template<class T>
    class ElLista {
    private:
        T _info;
        ElLista<T>* _proximo;
        ElLista<T>* _anterior;
    public:
        // Construtora  /*{{{{*/
        ElLista(T info)
        :   _info (info)
        ,   _proximo  ( NULL )
        ,   _anterior ( NULL ) {
        } /*}}}}*/
        // Construtora  /*{{{{*/
        ElLista()
        :    _info ()
        ,   _proximo  ( NULL )
        ,   _anterior ( NULL ) {
        } /*}}}}*/
        // Destrutora  /*{{{{*/
        ~ElLista() {
            _proximo = NULL;
            _anterior = NULL;
        } /*}}}}*/
       // setProx /*{{{{*/
        void setProx(ElLista<T>* proximo) {
            _proximo = proximo;
        } /*}}}}*/
       // getProx /*{{{{*/
        ElLista<TIPO>* getProx() const {
            return _proximo;
        } /*}}}}*/
       // setAnte /*{{{{*/
        void setAnte(ElLista<T>* anterior) {
            _anterior = anterior;
        } /*}}}}*/
       // getAnte /*{{{{*/
        ElLista<T>* getAnte() const {
            return _anterior;
        } /*}}}}*/
       // setInfo /*{{{{*/
        void setInfo(const T& info) {
            _info = &info;
        } /*}}}}*/
       // getInfo /*{{{{*/
        T& getInfo() {
            return (_info);
        } /*}}}}*/
    };/*}}}}*/

public:
    class iterator;
    class const_iterator;
    // Construtora  /*{{{{*/
    Lista()
    :   _primElem  (NULL)
    ,   _atualElem (NULL)
    ,   _n_infos   ( 0 ) {
    }/*}}}}*/
    // Destrutora  /*{{{{*/
    ~Lista() {
        clear();
    }/*}}}}*/
   // push_back  /*{{{{*/
    void push_back ( TIPO info ) {
        ElLista<TIPO>* novo = new ElLista<TIPO>(info);
        if ( _primElem == NULL ) {
            _primElem = novo;
        }
        else {
            _atualElem->setProx(novo);
            novo->setAnte(_atualElem);
        }
        _atualElem = novo;
        _n_infos++;
    }/*}}}}*/
   // push_front  /*{{{{*/
    void push_front ( TIPO info) {
        ElLista<TIPO>* novo = new ElLista<TIPO>(info);
        if ( _primElem == NULL ) {
            _atualElem = novo;
        }
        else {
            _primElem->setAnte(novo);
            novo->setProx(_primElem);
        }
        _primElem = novo;
        _n_infos++;
    }/*}}}}*/
   // pop_back /*{{{{*/
    void pop_back() {
        if (_atualElem != NULL) {
            ElLista<TIPO>* aux = _atualElem;
            _atualElem = _atualElem->getAnte();
            _atualElem->setProx(NULL);
            delete aux;
            _n_infos--;
        }
    }/*}}}}*/
   // pop_front /*{{{{*/
    void pop_front() {
        if (_primElem != NULL) {
            ElLista<TIPO>* aux = _primElem;
            _primElem = _primElem->getProx();
            if (_primElem != NULL) {
                _primElem->setAnte(NULL);
            }
            delete aux;
            _n_infos--;
        }
    }/*}}}}*/
   // begin  /*{{{{*/
    iterator begin () {
        return iterator (_primElem);
    }/*}}}}*/
    // cbegin  /*{{{{*/
    const_iterator cbegin () const {
        return const_iterator (_primElem);
    }/*}}}}*/
    // end  /*{{{{*/
    iterator end () {
        return iterator (NULL);
    }/*}}}}*/
    // cend  /*{{{{*/
    const_iterator cend () const {
        return const_iterator (NULL);
    }/*}}}}*/
   // clear /*{{{{*/
    void clear() {
        while ( _primElem != NULL ) {
            _atualElem = _primElem ->getProx();
            delete _primElem;
            _primElem = _atualElem;
        }
        _n_infos = 0;
    }/*}}}}*/
   // erase  /*{{{{*/
    iterator erase (iterator it) {
        if (it == end()) { // eh nulo
            return end();
        }
        // remove o primeiro elemento
        if (it == begin()) {
            pop_front();
            return begin();
        }
        ElLista<TIPO>* elem = it.getElemento();
        // remove o ultimo elemento
        if (elem->getProx() == end().getElemento()) {
            pop_back();
            return end();
        }
        // remove um elemento do meio da lista
        elem->getAnte()->setProx(elem->getProx());
        elem->getProx()->setAnte(elem->getAnte());
        it++;
        _n_infos--;
        delete elem;
        return it;
    }/*}}}}*/
    // remove/*{{{{*/
    void remove (TIPO elem) {
        for (iterator it = begin(); it != end(); it++) {
            if ((*it) == elem) {
                erase(it);
                break;
            }
        }
    }/*}}}}*/
    // size/*{{{{*/
    int size() const {
        return (_n_infos);
    }/*}}}}*/
   // iterator  /*{{{{*/
    class iterator {
        public:
            iterator()
            :   _elem(NULL) {
            }
            iterator(ElLista<TIPO>* ptr)
            :   _elem(ptr) {
            }
            TIPO& operator*() {
                return (_elem->getInfo());
            }
            TIPO& operator->() {
                return (*(*this));
            }
            // ++it;
            iterator& operator++() {
                if (_elem) {
                    _elem = _elem->getProx(); 
                }
                return (*this);
            }
            // incrementa mas retorna o valor anterior para ser usado
            // it++;
            iterator operator++ (int) {
                iterator it = *this;
                ++(*this);
                return it;
            }
            iterator& operator--() {
                if (_elem) {
                    _elem = _elem->getAnte(); 
                }
                return (*this);
            }
            iterator operator-- (int) {
                iterator it = *this;
                --(*this);
                return it;
            }
            bool operator != (const iterator& iterador) const {
                return (_elem != iterador._elem); 
            }
            bool operator == (const iterator& iterador) const {
                return (_elem == iterador._elem); 
            }
            ElLista<TIPO>* getElemento() const {
                return _elem;
            }
        private:
            ElLista<TIPO>* _elem;

    };/*}}}}*/  
   // const_iterator  /*{{{{*/
    class const_iterator {
        public:
            const_iterator()
            :   _elem(NULL) {
            }
            const_iterator(ElLista<TIPO>* ptr)
            :   _elem(ptr) {
            }
            const TIPO& operator*() const {
                return (_elem->getInfo());
            }
            const TIPO& operator->() const {
                return (*(*this));
            }
            // ++it;
            const_iterator& operator++() {
                if (_elem) {
                    _elem = _elem->getProx(); 
                }
                return (*this);
            }
            // incrementa mas retorna o valor anterior para ser usado
            // it++;
            const_iterator operator++ (int) {
                const_iterator it = *this;
                ++(*this);
                return it;
            }
            const_iterator& operator--() {
                if (_elem) {
                    _elem = _elem->getAnte(); 
                }
                return (*this);
            }
            const_iterator operator-- (int) {
                const_iterator it = *this;
                --(*this);
                return it;
            }
            bool operator != (const const_iterator& iterador) const {
                return (_elem != iterador._elem); 
            }
            bool operator == (const const_iterator& iterador) const {
                return (_elem == iterador._elem); 
            }
            ElLista<TIPO>* getElemento() const {
                return _elem;
            }
        private:
            ElLista<TIPO>* _elem;
    };/*}}}}*/
};
