import javax.swing.*;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class Main {


    public static void main(String[] args) {

        ArrayList<Point> points;
        ArrayList<Point> constructedPath;
        Scanner scan = new Scanner(System.in);
        System.out.print("Enter the input file name: ");
        String fileName = scan.next();
        FileHelper fh = new FileHelper(fileName);
        points = fh.getPointsFromFile();
        

        System.out.println("\nCost of initial path without any modification: " + Algorithms.getHamiltonianCycleCost(points) + "\n");

        if (points.size() < 1000) {
            System.out.println("Applying Farthest Insertion Algorithm...");
            constructedPath = Algorithms.fartheshInsertionAlgorithm(points);
            System.out.println("Cost of path after applying farthest insertion algorithm : " + Algorithms.getHamiltonianCycleCost(constructedPath) + "\n");
        } else {
            System.out.println("Applying Nearest Neighbor Algorithm...");
            constructedPath = Algorithms.getBestNearest(points);
            System.out.println("Cost of path after applying nearest neighbor algorithm : " + Algorithms.getHamiltonianCycleCost(constructedPath) + "\n");
        }

        System.out.println("Applying 2-OPT Algorithm...");
        ArrayList<Point> twoOptRoute = Algorithms.twoOptOptimization(constructedPath);
        double cost = Algorithms.getHamiltonianCycleCost(twoOptRoute);
        System.out.println("Cost of path after applying 2-opt : " + cost + "\n");
        fh.createOutputFile(twoOptRoute);
    }

}