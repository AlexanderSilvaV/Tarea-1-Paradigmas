#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Pregunta {
private:
    int id;
    string enunciado;
    string tipo;
    string nivel;
    string respuesta;
    int tiempo;

public:
    Pregunta() {}

    void mostrar() const {
        cout << "ID: " << id << "\n"
             << "Enunciado: " << enunciado << "\n"
             << "Tipo: " << tipo << "\n"
             << "Nivel: " << nivel << "\n"
             << "Respuesta esperada: " << respuesta << "\n"
             << "Tiempo estimado: " << tiempo << " minutos\n";
    }

    void guardar(ofstream& archivo) const {
        archivo << "ID: " << id << "; Enunciado: " << enunciado << ";Tipo: " << tipo
                << ";Nivel Taxonomico: " << nivel << "; Respuesta esperada: " << respuesta
                << "; tiempo estimado en minutos: " << tiempo << "\n";
    }

    static Pregunta* cargarDesdeLinea(const string& linea) {
        Pregunta* p = new Pregunta();
        size_t pos = 0, prev = 0;
        vector<string> campos;

        while ((pos = linea.find(';', prev)) != string::npos) {
            campos.push_back(linea.substr(prev, pos - prev));
            prev = pos + 1;
        }
        campos.push_back(linea.substr(prev));

        if (campos.size() == 6) {
            p->id = stoi(campos[0]);
            p->enunciado = campos[1];
            p->tipo = campos[2];
            p->nivel = campos[3];
            p->respuesta = campos[4];
            p->tiempo = stoi(campos[5]);
        }

        return p;
    }

    int getID() const { return id; }
    void setID(int i) { id = i; }

    string getTipo() const { return tipo; }
    void setTipo(const string& t) { tipo = t; }

    string& getRespuesta() { return respuesta; }
    void setRespuesta(const string& r) { respuesta = r; }

    void setEnunciado(const string& e) { enunciado = e; }
    void setNivel(const string& n) { nivel = n; }
    void setTiempo(int t) { tiempo = t; }

    string getNivel() const { return nivel; }
    int getTiempo() const { return tiempo; }
};

class Evaluacion {
private:
    vector<Pregunta*> preguntas;

    int buscarPorID(int id) const {
        for (size_t i = 0; i < preguntas.size(); ++i) {
            if (preguntas[i]->getID() == id)
                return i;
        }
        return -1;
    }

public:
    ~Evaluacion() {
        for (auto p : preguntas)
            delete p;
    }

    void crearPregunta() {
        Pregunta* p = new Pregunta();
        int id;
        cout << "ID de la pregunta: ";
        cin >> id;
        if (buscarPorID(id) != -1) {
            cout << "Error: ya existe una pregunta con ese ID.\n";
            delete p;
            return;
        }
        p->setID(id);
        cin.ignore();
        string enunciado, tipo, nivel, respuesta, justificacion;
        int tiempo;
        cout << "Enunciado: ";
        getline(cin, enunciado);
        p->setEnunciado(enunciado);

        cout << "Tipo (multiple/vof): ";
        getline(cin, tipo);
        p->setTipo(tipo);

        cout << "Nivel taxonómico (Recordar, Entender, etc): ";
        getline(cin, nivel);
        p->setNivel(nivel);

        cout << "Respuesta esperada: ";
        getline(cin, respuesta);

        if (tipo == "vof") {
            cout << "Justificación (opcional): ";
            getline(cin, justificacion);
            if (!justificacion.empty()) {
                respuesta += " | Justificación: " + justificacion;
            }
        }
        p->setRespuesta(respuesta);

        cout << "Tiempo estimado (minutos): ";
        cin >> tiempo;
        p->setTiempo(tiempo);

        preguntas.push_back(p);
        cout << "Pregunta agregada con éxito.\n";
    }

    void actualizarPregunta() {
        int id;
        cout << "Ingrese ID a actualizar: ";
        cin >> id;
        int idx = buscarPorID(id);
        if (idx != -1) {
            cin.ignore();
            string enunciado, tipo, nivel, respuesta, justificacion;
            int tiempo;
            cout << "Nuevo enunciado: ";
            getline(cin, enunciado);
            preguntas[idx]->setEnunciado(enunciado);
            cout << "Nuevo tipo: ";
            getline(cin, tipo);
            preguntas[idx]->setTipo(tipo);
            cout << "Nuevo nivel: ";
            getline(cin, nivel);
            preguntas[idx]->setNivel(nivel);
            cout << "Nueva respuesta esperada: ";
            getline(cin, respuesta);

            if (tipo == "vof") {
                cout << "Justificación (opcional): ";
                getline(cin, justificacion);
                if (!justificacion.empty()) {
                    respuesta += " | Justificación: " + justificacion;
                }
            }
            preguntas[idx]->setRespuesta(respuesta);
            cout << "Nuevo tiempo estimado: ";
            cin >> tiempo;
            preguntas[idx]->setTiempo(tiempo);
            cout << "Pregunta actualizada con éxito.\n";
        } else {
            cout << "Pregunta no encontrada.\n";
        }
    }

    void eliminarPregunta() {
        int id;
        cout << "Ingrese ID a eliminar: ";
        cin >> id;
        int idx = buscarPorID(id);
        if (idx != -1) {
            delete preguntas[idx];
            preguntas.erase(preguntas.begin() + idx);
            cout << "Pregunta eliminada.\n";
        } else {
            cout << "Pregunta no encontrada.\n";
        }
    }

    void consultarPregunta() const {
        int id;
        cout << "Ingrese ID a consultar: ";
        cin >> id;
        int idx = buscarPorID(id);
        if (idx != -1) {
            preguntas[idx]->mostrar();
        } else {
            cout << "Pregunta no encontrada.\n";
        }
    }

    void buscarPorNivel() const {
        string nivel;
        cin.ignore();
        cout << "Ingrese nivel taxonómico: ";
        getline(cin, nivel);
        bool encontrado = false;
        for (auto p : preguntas) {
            if (p->getNivel() == nivel) {
                p->mostrar();
                cout << "------------------\n";
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "No se encontraron preguntas con ese nivel.\n";
        }
    }

    void mostrarTodas() const {
        for (auto p : preguntas) {
            p->mostrar();
            cout << "------------------\n";
        }
    }

    void calcularTiempoTotal() const {
        int total = 0;
        for (auto p : preguntas) {
            total += p->getTiempo();
        }
        cout << "Tiempo total estimado: " << total << " minutos\n";
    }

    void guardarEnArchivo(const string& nombre) const {
        ofstream archivo(nombre);
        for (auto p : preguntas)
            p->guardar(archivo);
        archivo.close();
        cout << "Preguntas guardadas en archivo.\n";
    }

    void cargarDesdeArchivo(const string& nombre) {
        ifstream archivo(nombre);
        string linea;
        while (getline(archivo, linea)) {
            Pregunta* p = Pregunta::cargarDesdeLinea(linea);
            preguntas.push_back(p);
        }
        archivo.close();
        cout << "Preguntas cargadas desde archivo.\n";
    }

    void registrarTiemposReal(const string& archivo) {
        ofstream out(archivo);
        int tiempoTotal = 0;
        int tiempoPregunta;
        for (auto p : preguntas) {
            cout << "\nPregunta ID " << p->getID() << ":\n";
            cout << "¿Cuántos minutos te demoraste en responder esta pregunta?: ";
            cin >> tiempoPregunta;
            out << "ID: " << p->getID() << " - Tiempo real: " << tiempoPregunta << " minutos\n";
            tiempoTotal += tiempoPregunta;
        }
        out << "Tiempo total real: " << tiempoTotal << " minutos\n";
        out.close();
        cout << "\nTiempos reales guardados en '" << archivo << "'\n";
    }
};

void menu() {
    Evaluacion eval;
    eval.cargarDesdeArchivo("preguntas.txt");

    int opcion;
    do {
        cout << "\n--- MENÚ ---\n";
        cout << "1. Crear pregunta\n";
        cout << "2. Actualizar pregunta\n";
        cout << "3. Eliminar pregunta\n";
        cout << "4. Consultar pregunta\n";
        cout << "5. Buscar por nivel taxonómico\n";
        cout << "6. Mostrar todas\n";
        cout << "7. Calcular tiempo total estimado\n";
        cout << "8. Guardar en archivo\n";
        cout << "9. Registrar tiempos reales de evaluación\n";
        cout << "0. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: eval.crearPregunta(); break;
            case 2: eval.actualizarPregunta(); break;
            case 3: eval.eliminarPregunta(); break;
            case 4: eval.consultarPregunta(); break;
            case 5: eval.buscarPorNivel(); break;
            case 6: eval.mostrarTodas(); break;
            case 7: eval.calcularTiempoTotal(); break;
            case 8: eval.guardarEnArchivo("preguntas.txt"); break;
            case 9: eval.registrarTiemposReal("tiempos_resueltos.txt"); break;
            case 0: cout << "Saliendo...\n"; break;
            default: cout << "Opción inválida.\n"; break;
        }

    } while (opcion != 0);
}

int main() {
    menu();
    return 0;
}