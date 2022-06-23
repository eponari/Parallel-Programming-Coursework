import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) throws IOException {
        try (Socket socket = new Socket("localhost", 1234)) {

            // writing to server
            PrintWriter output = new PrintWriter(
                    socket.getOutputStream(), true);

            // reading from server
            BufferedReader input
                    = new BufferedReader(new InputStreamReader(
                    socket.getInputStream()));

            // object of scanner class
            Scanner userInput = new Scanner(System.in);
            String userChoice = null;

            while (true) {

                System.out.println("Enter scissors(S), rock(R), paper(P) or exit(E) :");

                // reading from user
                userChoice = userInput.nextLine();

                if(userChoice.equals("E")){
                    System.out.println("Thank you for playing!");
                    break;
                }

                if(!userChoice.equals("R")&&!userChoice.equals("P")&&!userChoice.equals("S")){
                    System.out.println("You can only choose scissors(S), rock(R) or paper(P)! You entered " + userChoice + " . Try again!");
                    continue;
                }

                // sending the user input to server
                output.println(userChoice);
                output.flush();

                // displaying server reply
                System.out.println(input.readLine());
            }

            // closing the scanner object
            userInput.close();
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
