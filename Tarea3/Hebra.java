import java.util.Collections;
import java.util.List;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Hebra implements Runnable{
    
    int[][] matriz; // matriz que contiene el mapa del informe nautico
    int row_t, row_b; // row top, row bottom: delimitan el espacio de busqueda en las filas
    int col_l, col_r; // column left, column right: delimitan el espacio de busqueda en las columnas
    int M; // tamano de la embarcacion

    public Hebra(int row_t, int row_b, int col_l, int col_r, int M, int matriz[][])
    {
        this.row_t = row_t;
        this.row_b = row_b;
        this.col_l = col_l;
        this.col_r = col_r;
        this.M = M;
        this.matriz = matriz;
    }

    /*
     * run: este metodo se ejecuta por cada hebra.
     * Crea las submatrices, sus hebras correspondientes, ejecuta cada hebra y verifica una condicion de parada
     * recibe: nada
     * retorna: nada
     */
    public void run()
    {
        // Si la dimension de la submatriz es distinta al largo de la embarcacion
        if((row_b - row_t + 1) != M && (col_l - col_r + 1) != M)
        {
            // Calculamos la mitad de la dimension
            int half = (row_b - row_t + 1) / 2;

            // Creamos 4 submatrices para los 4 cuadrantes definidos por la mitad de la matriz original
            Hebra c1 = new Hebra(row_t, row_t + half - 1, col_l, col_l + half - 1, M, matriz);
            Hebra c2 = new Hebra(row_t + half, row_b, col_l, col_l + half - 1, M, matriz);
            Hebra c3 = new Hebra(row_t, row_t + half - 1, col_l + half, col_r, M, matriz);
            Hebra c4 = new Hebra(row_t + half, row_b, col_l + half, col_r, M, matriz);

            // Creamos las 4 hebras correspondientes a las submatrices
            Thread thr1 = new Thread(c1);
            Thread thr2 = new Thread(c2);
            Thread thr3 = new Thread(c3);
            Thread thr4 = new Thread(c4);

            try
            {
                // Ejecutamos cada hebra
                thr1.start();
                thr2.start();
                thr3.start();
                thr4.start();

                // Esperamos su ejecucion
                thr1.join();
                thr2.join();
                thr3.join();
                thr4.join();
            } catch (Exception e)
            {
                System.out.println("Error en la hebra!");
                System.out.println(e);
            }
        }
        // Si la dimension de la submatriz corresponde al largo de la embarcacion, chequeamos si el barco esta ahi
        else
            check();

    }

    /*
     * check: revisa la matriz en busca de la primera ocurrencia del barco, imprime por
     * pantalla la posicion tanto en filas como columnas en caso de encontrarlo
     * recibe: nada
     * retorna: nada
     */
    void check()
    {
        for(int i = row_t; i <= row_b; i++)
        {
            if(matriz[i][col_l] == 1)
            {
                System.out.println("fila " + (i + 1) + ", columna " + (col_l + 1));
                return;
            }
        }
    }

    /*
     * imprimirMatriz: imprime la matriz segun el espacio de busqueda delimitado dentro de la clase
     * recibe: nada
     * retorna: nada
     */
    void imprimirMatriz()
    {
        for(int i = row_t; i <= row_b; i++)
        {
            for(int j = col_l; j <= col_r; j++)
                System.out.print(matriz[i][j] + " ");
            System.out.println("");
        }
    }

    /*
     * main: abre el archivo a analizar, transforma los datos segun sea conveniente, y
     * crea la clase junto a la hebra para ejecutar y analizar el informe nautico
     */
    public static void main(String args[])
    {
        // Creamos una lista en donde guardaremos las lineas leidas
        List<String> data = Collections.emptyList();
        try {
            String nombre = "informe-nautico.txt";
            data = Files.readAllLines(Paths.get(nombre), StandardCharsets.UTF_8); // leemos las lineas
        } catch (IOException e){
            System.out.println("Error abriendo archivo de prueba!");
        }

        int N = Integer.parseInt(data.get(0)); // extraemos la dimension de la matriz
        int M = Integer.parseInt(data.get(1)); // extraemos la dimension de la embarcacion
        int[][] matriz = new int[N][N]; // creamos la matriz

        String[] rows = data.get(2).split(",");
        for(int i = 0; i < N; i++) // agregamos los datos de cada casilla a la matriz
        {
            String[] cols = rows[i].substring(1, rows[i].length() - 1).split(" ");
            for(int j = 0; j < N; j++)
                matriz[i][j] = Integer.parseInt(cols[j]);
        }

        Hebra hbr = new Hebra(0, N - 1, 0, N - 1, M, matriz); // creamos un objeto hebra

        Thread thr = new Thread(hbr); // creamos la hebra del objeto
        try
        {
            thr.start(); // ejecutamos la hebra
            thr.join(); // esperamos a que termine
        } catch (Exception e)
        {
            System.out.println("Error en la hebra!");
        }

    }
}
