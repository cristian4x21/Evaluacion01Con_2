#include <iostream> 
#include <string>  

using namespace std;


struct Producto {
    int id;
    string nombre;
    double precio;
    int stock;
    int prioridad_pedido;
    Producto* siguiente;
};

struct Devolucion {
    int id_devolucion;
    string motivo;
    Devolucion* siguiente;
};


Producto* g_listaInventario = NULL;
Producto* g_colaPedidos_frente = NULL;
Producto* g_colaPedidos_fin = NULL;
Devolucion* g_pilaDevoluciones = NULL;


void insertarProducto(Producto*& lista, Producto* nuevoProducto) {
    if (lista == NULL) { lista = nuevoProducto; }
    else {
        Producto* actual = lista;
        while (actual->siguiente != NULL) { actual = actual->siguiente; }
        actual->siguiente = nuevoProducto;
    }
}
void eliminarProducto(Producto*& lista, int id) {
    Producto* actual = lista, *anterior = NULL;
    while (actual != NULL && actual->id != id) {
        anterior = actual; actual = actual->siguiente;
    }
    if (actual == NULL) { cout << "  ID no encontrado.\n"; return; }
    if (anterior == NULL) { lista = actual->siguiente; }
    else { anterior->siguiente = actual->siguiente; }
    delete actual;
    cout << "  Producto eliminado.\n";
}
Producto* buscarProducto(Producto* lista, int id) {
    Producto* actual = lista;
    while (actual != NULL) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return NULL;
}
void modificarStock(Producto* lista, int id) {
    Producto* p = buscarProducto(lista, id);
    if (p != NULL) {
        cout << "  Stock actual: " << p->stock << ". Nuevo stock: ";
        cin >> p->stock;
        cout << "  Stock actualizado.\n";
    } else { cout << "  Producto no encontrado.\n"; }
}
void mostrarInventario(Producto* lista) {
    cout << "  --- Inventario General ---\n";
    Producto* actual = lista;
    if (actual == NULL) { cout << "  (Inventario vacio)\n"; return; }
    while (actual != NULL) {
        cout << "  [ID: " << actual->id << "] " << actual->nombre << " (Stock: " << actual->stock << ")\n";
        actual = actual->siguiente;
    }
}

void encolarPrioridad(Producto*& frente, Producto*& fin, Producto* nuevoPedido) {
    if (frente == NULL) { frente = fin = nuevoPedido; return; }
    if (nuevoPedido->prioridad_pedido < frente->prioridad_pedido) {
        nuevoPedido->siguiente = frente; frente = nuevoPedido; return;
    }
    Producto* actual = frente;
    while (actual->siguiente != NULL && actual->siguiente->prioridad_pedido <= nuevoPedido->prioridad_pedido) {
        actual = actual->siguiente;
    }
    nuevoPedido->siguiente = actual->siguiente;
    actual->siguiente = nuevoPedido;
    if (nuevoPedido->siguiente == NULL) { fin = nuevoPedido; }
}
void desencolarPedido(Producto*& frente, Producto*& fin) {
    if (frente == NULL) { cout << "  (Cola de pedidos vacia)\n"; return; }
    Producto* temp = frente;
    cout << "  Procesando pedido: [ID: " << temp->id << "] " << temp->nombre << endl;
    frente = frente->siguiente;
    if (frente == NULL) { fin = NULL; }
    delete temp;
}
void mostrarColaPedidos(Producto* frente) {
    cout << "  --- Cola de Pedidos (Prioridad) ---\n";
    if (frente == NULL) { cout << "  (Cola vacia)\n"; return; }
    cout << "  FRENTE -> ";
    Producto* actual = frente;
    while (actual != NULL) {
        cout << "[" << actual->nombre << "(P:" << actual->prioridad_pedido << ")] -> ";
        actual = actual->siguiente;
    }
    cout << "FIN\n";
}

void pushDevolucion(Devolucion*& pila, int id, string motivo) {
    Devolucion* nuevaDevolucion = new Devolucion{id, motivo, pila};
    pila = nuevaDevolucion;
}
void popDevolucion(Devolucion*& pila) {
    if (pila == NULL) { cout << "  (Pila de devoluciones vacia)\n"; return; }
    Devolucion* temp = pila;
    cout << "  Procesando devolucion: ID " << temp->id_devolucion << " (Motivo: " << temp->motivo << ")\n";
    pila = pila->siguiente;
    delete temp;
}
void mostrarPilaDevoluciones(Devolucion* pila) {
    cout << "  --- Pila de Devoluciones ---\n";
    if (pila == NULL) { cout << "  (Pila vacia)\n"; return; }
    cout << "  CIMA\n";
    Devolucion* actual = pila;
    while (actual != NULL) {
        cout << "  [ID: " << actual->id_devolucion << " - Motivo: " << actual->motivo << "]\n";
        actual = actual->siguiente;
    }
    cout << "  BASE\n";
}


int main() {
    int opcion;
    int id, stock, prioridad, id_dev;
    double precio;
    string nombre, motivo;
    Producto* p; 

    do {
       
        cout << "\n--- GESTION DE INVENTARIO  ---\n";
        cout << " 1. Registrar Producto \n";
        cout << " 2. Eliminar Producto \n";
        cout << " 3. Buscar Producto \n";
        cout << " 4. Modificar Stock \n";
        cout << " 5. Mostrar Inventario \n";
        cout << " 6. Encolar Pedido \n";
        cout << " 7. Procesar Pedido \n";
        cout << " 8. Mostrar Cola Pedidos \n";
        cout << " 9. Registrar Devolucion \n";
        cout << " 10. Procesar Devolucion \n";
        cout << " 11. Mostrar Pila Devoluciones \n";
        cout << " 12. Salir \n";
        cout << "--------------------------------------\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;
        

        switch (opcion) {
            case 1: 
                cout << "  ID: "; cin >> id;
                if (buscarProducto(g_listaInventario, id) != NULL) {
                    cout << "  Error: ID ya existe.\n"; break;
                }
                cout << "  Nombre: "; (cin >> ws); getline(cin, nombre);
                cout << "  Precio: "; cin >> precio;
                cout << "  Stock: "; cin >> stock;
                p = new Producto{id, nombre, precio, stock, 0, NULL};
                insertarProducto(g_listaInventario, p);
                cout << "  Producto registrado.\n";
                break;
            case 2: 
                cout << "  ID a eliminar: "; cin >> id;
                eliminarProducto(g_listaInventario, id);
                break;
            case 3: 
                cout << "  ID a buscar: "; cin >> id;
                p = buscarProducto(g_listaInventario, id);
                if (p) cout << "  Encontrado: " << p->nombre << " (Stock: " << p->stock << ")\n";
                else cout << "  No encontrado.\n";
                break;
            case 4: 
                cout << "  ID del producto a modificar: "; cin >> id;
                modificarStock(g_listaInventario, id);
                break;
            case 5:
                mostrarInventario(g_listaInventario);
                break;
            case 6: 
                cout << "  ID del producto a encolar (del inventario): ";
                cin >> id;
                p = buscarProducto(g_listaInventario, id);
                if (p != NULL) {
                    cout << "  Prioridad (1-Express, 5-Estandar): "; cin >> prioridad;
                    Producto* pCola = new Producto{p->id, p->nombre, p->precio, 1, prioridad, NULL};
                    encolarPrioridad(g_colaPedidos_frente, g_colaPedidos_fin, pCola);
                    cout << "  Pedido encolado.\n";
                } else { cout << "  Producto no encontrado.\n"; }
                break;
            case 7: 
                desencolarPedido(g_colaPedidos_frente, g_colaPedidos_fin);
                break;
            case 8: 
                mostrarColaPedidos(g_colaPedidos_frente);
                break;
            case 9: 
                cout << "  ID de devolucion: "; cin >> id_dev;
                cout << "  Motivo: "; (cin >> ws); getline(cin, motivo);
                pushDevolucion(g_pilaDevoluciones, id_dev, motivo);
                cout << "  Devolucion registrada.\n";
                break;
            case 10: 
                popDevolucion(g_pilaDevoluciones);
                break;
            case 11:
                mostrarPilaDevoluciones(g_pilaDevoluciones);
                break;
            case 12: 
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
        cout << endl;

    } while (opcion != 12);

    return 0; 
}
