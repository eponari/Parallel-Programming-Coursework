import java.io.*;
import java.net.*;
import java.util.HashMap;

// Server class
class Server {
    public static void main(String[] args)
    {
        ServerSocket welcomeSocket = null;

        try {

            // server is listening on port 1234
            welcomeSocket = new ServerSocket(1234);
            welcomeSocket.setReuseAddress(true);

            // running infinite loop waiting for client requests
            while (true) {

                // socket to receive data from user.
                Socket clientSocket = welcomeSocket.accept();

                // display connection to new client
                System.out.println("New client connected");

                // create a new object with the runnable interface to handle interaction to a client.
                ClientHandler clientThread = new ClientHandler(clientSocket);

                // start the thread with the runnable interface.
                clientThread.start();
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        finally {
            if (welcomeSocket != null) {
                try {
                    welcomeSocket.close();
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // ClientHandler class
    private static class ClientHandler extends Thread {
        private final Socket clientSocket;

        // Constructor
        public ClientHandler(Socket socket)
        {
            this.clientSocket = socket;
        }

        public void run()
        {
            PrintWriter output = null;
            BufferedReader input = null;
            try {

                // send data to user.
                output = new PrintWriter(clientSocket.getOutputStream(), true);

                // get data from user.
                input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                //get user input.
                String userChoice;
                while ((userChoice = input.readLine()) != null) {

                    //possible choices.
                    String[] choices = {"P","R","S"};

                    //mapping to the choices and the words they represent.
                    HashMap<String,String> words = new HashMap<String,String>();
                    words.put("P","Paper");
                    words.put("R","Rock");
                    words.put("S","Scissors");

                    //random choice for the computer.
                    String computerChoice = choices[(int) (Math.random() * 3)];

                    //result of the game.
                    String result = "";

                    //logic of the game.
                    if(computerChoice.equals(userChoice)){
                        result = "tied";
                    }
                    else if((computerChoice.equals("R")&&userChoice.equals("P"))|| (computerChoice.equals("P")&&userChoice.equals("S")) || computerChoice.equals("S")&&userChoice.equals("R")){
                        result = "won";
                    }
                    else{
                        result = "lost";
                    }

                    //message to send the client.
                    String message = "Computer chose "+ words.get(computerChoice) + "\t" +
                                     "Player chose "+ words.get(userChoice) + "\t" +
                                     "You "+result;

                    //send the message to the client.
                    output.println(message);
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
            finally {
                try {
                    if (output != null) {
                        output.close();
                    }
                    if (input != null) {
                        input.close();
                        clientSocket.close();
                    }
                }
                catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}