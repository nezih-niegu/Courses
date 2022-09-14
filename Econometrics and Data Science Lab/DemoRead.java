/**
 * DemoRead.java  Implementa un demo para lectura de un grupo de datos
 * 
 * Sesion20   16 de octubre de 2020
 **/

import java.util.*;

class DemoRead {

    public static void readDataParcial(Scanner s, int ini, int fin) {
        while (s.hasNextLine()) {
            String line = s.nextLine();
            String[] data = line.split(",");
            int userId = Integer.parseInt(data[0]);
            String name = data[1];
            String lastName = data[2];
            String mail = data[3];
            String passwd = data[4];
            if (userId >= ini && userId <= fin) {
                //myUserData newD = new myUserData(userId, name, lastName, mail, passwd);
                //LLNode<myUserData> = new LLNode<myUserData>(newD);
                System.out.print(userId + " ");
                System.out.print(name + " ");
                System.out.print(lastName + " ");
                System.out.print(mail + " ");
                System.out.print(passwd + " ");
                System.out.println();
            }

        }
    }
    public static void readData(Scanner s) {
        while (s.hasNextLine()) {
            String line = s.nextLine();
            String[] data = line.split(",");
            int userId = Integer.parseInt(data[0]);
            String name = data[1];
            String lastName = data[2];
            String mail = data[3];
            String passwd = data[4];
            //myUserData newD = new myUserData(userId, name, lastName, mail, passwd);
            //LLNode<myUserData> = new LLNode<myUserData>(newD);
            System.out.print(userId + " ");
            System.out.print(name + " ");
            System.out.print(lastName + " ");
            System.out.print(mail + " ");
            System.out.print(passwd + " ");
            System.out.println();
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        //readData(scanner);
        readDataParcial(scanner, 2, 4);

    }
}