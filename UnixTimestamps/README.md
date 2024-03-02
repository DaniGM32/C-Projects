Pentru taskul 1 am salvat in variabila time, timestampul initial, dupa care l-am egalat cu restul impartirii la nr de secunde dintr-o zi. Dupa, nr de ore e dat de partea intreaga la impartirea cu secundele dintr-o ora. La fel am procedat pt minute, iar pt secunde am scazut nr de ore si de minute din time.

Pentru taskul 2 am procedat asemanator ca la taskul 1, doar ca pt nr de ani am impartit direct variabila time la nr de secunde dintr-un an si am adunat la 1970, care e an de referinta. Dupa pentru luni am luat 3 cazuri, daca lunile au 30 de zile, 31 sau 28 pentru ca nu se iau in considerare anii bisecti. La final am verificat daca nr de zile se incadreaza in luna respectiva, daca nu, am marit luna si am scazut nr de zile

Pentru taskul 3 am folosit functia de la taskul 2, si am scazut din nr de zile, cati ani bisecti au fost de la 1970 incoace, dupa am verificat cazurile pe care le-am verif si la taskul 3, nr de zile raportat la nr de zile din luna in care ma aflu, dar si cazul in care luna devine 0 sau mai mare decat 12, se modifica si anul

Pentru taskul 4 am folosit functiile de la taskurile 1 si 3, dupa am luat mai multe cazuri, daca nr de ore + utc_hour_dif depaseste 24 sau e mai micdecat 0, trebuie modificata ziua, dupa daca ziua e mai mica decat 1 sau depaseste 30, 31, 28/29 in functie de luna trebuie modificata luna, si in final anul daca luna e 0 sau >12

Pentru taskul 5 am folosit in principiu 2 while-uri in care parcurg toti anii de la 1970 incoace pana la anul curent si toate lunile din acel an, adunand nr de secunde dintr-un an bisect/nebisect sau dintr-o luna de 31 sau 30 sau 29/28 de zile

Pentru taskul 6 pur si simplu am facut afisarea, pentru afisarea lunii am folosit 12 if-uri ca sa afisez cele 2 stringuri (nu am stiut altfel :))

Pentru taskul 7 am luat un vector de date in care sa retin datele, ca mai apoi sa le sortez. Am definit 3 masti care corespund cu bitii de 1 de care am nevoie ca sa calculez ziua, luna respectiv anul, iar dupa conversie am sortat dupa ani, daca anii sunt egali sortez dupa luna, daca lunile sunt egale sortez dupa zi si fac afisarea.

Pentru taskul 9 am considerat o noua structura, Person, pentru a retine toate datele citite de la tastatura. Am adus toate intervalele in acelasi timezone si am concatenat intervalele consecutive. 
