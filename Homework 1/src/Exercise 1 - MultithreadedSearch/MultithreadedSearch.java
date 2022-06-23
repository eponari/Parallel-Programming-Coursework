import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class MultithreadedSearch {
    //keyword to be searched in each file.
    static String wordSearched;

    //path of each one of the files to search.
    static ArrayList<String> filesSearched = new ArrayList<>();

    //total occurrences of the word in every file.
    static int totalOccurrences = 0;

    //option to change the execution.
    static boolean liveResults = false;
    
    public static void main(String[] args) throws InterruptedException {
        //first argument is the word to be searched.
        //cast to lower case to make the searching easier.
        wordSearched = args[0].toLowerCase();

        //process each argument from the second to the last.
        for(int i=1;i<(args.length);i++){
            //if the user used the option -liveResults, change the static variable of this class.
            if(args[i].equals("-liveResults")){
                liveResults = true;
                continue;
            }

            //store all file names.
            filesSearched.add(args[i]);
        }

        //create a array of threads to search for the word we need to find.
        SearchingThread[] threadsToSearch = new SearchingThread[filesSearched.size()];

        //if liveResult is selected as a option, print the live results.
        if(liveResults){
            System.out.println("LIVE RESULTS:\n");
        }

        for(int i=0;i<filesSearched.size();i++){
            //create and start each thread.
            threadsToSearch[i] = new SearchingThread(filesSearched.get(i));
            threadsToSearch[i].start();
        }

        //join all the threads.
        for(Thread thread:threadsToSearch){
            thread.join();
        }

        //print the summary.
        System.out.println("\nTHE SUMMARY:\n");

        //each thread is printed in parallell to each other.
        Thread[] printThreads = new Thread[filesSearched.size()];

        for(int i=0;i<filesSearched.size();i++){
            //create and start threads to print the result of each thread.
            printThreads[i] = new PrintableThread(threadsToSearch[i]);
            printThreads[i].start();
        }

        for(Thread thread:printThreads){
            //join the printing threads.
            thread.join();
        }

        //print final result.
        System.out.println("\nTOTAL: "+totalOccurrences+" occurrence"+(totalOccurrences>1?"s":"")+" of keyword "+wordSearched);
    }

    //class to print threads' data in parallel.
    static class PrintableThread extends Thread{
        //thread to be printed.
        private final SearchingThread thread;

        //constructor
        PrintableThread(SearchingThread thread){
            this.thread = thread;
        }

        //override the run method to be run in parallel.
        @Override
        public void run(){
            //print the thread data.
            System.out.println(thread);
        }
    }

    //class to search a word in a file in parallel.
    static class SearchingThread extends Thread{
        //path of the file the thread is searching in.
        private final String path_file;
        //total occurrences of the word in the file.
        private int total=0;

        //constructor.
        SearchingThread(String path_file){
            this.path_file = path_file;
        }


        @Override
        public void run(){
            //try to open the file.
            try {
                //scanner for the file.
                Scanner fileInput = new Scanner(new File(path_file));

                //keep note of the number of lines of the file.
                int lineNum=0;

                while(fileInput.hasNext()){
                    //count lines.
                    lineNum++;

                    //get the next line from the file.
                    //cast to lowercase to make it easier to search the pattern.
                    String line = fileInput.nextLine().toLowerCase();

                    //linear search in the line for the word.
                    for(int i=0;i<line.length()-wordSearched.length()+1;i++){
                        if(line.substring(i,i+wordSearched.length()).equals(wordSearched)){
                            total++;

                            //print the live result.
                            if(liveResults){
                                System.out.println(countOccurrence(lineNum));
                            }
                        }
                    }
                }

                //lock the totalOccurrences variable to add and then unlock.
                synchronized (this) {
                    totalOccurrences += total;
                }
            }
            catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }

        //print the live result.
        public String countOccurrence(int line){
            return path_file+":\t"+total+" "+"occurrence"+(total>1?"s":"") + " at line "+line;
        }

        //print the data of the thread.
        @Override
        public String toString(){
            return path_file+":\t"+total+" "+"occurrence"+(total>1?"s":"");
        }
    }
}