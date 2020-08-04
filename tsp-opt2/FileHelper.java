import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Scanner;

public class FileHelper {

    private String filename;

    public FileHelper(String filename) {
        this.filename = filename;
    }

    public ArrayList<Point> getPointsFromFile() {
        ArrayList<Point> points = new ArrayList<Point>();
        File file = new File(filename);
        try {
            Scanner reader = new Scanner(file);
            while (reader.hasNextLine()) {
                String currentLine = reader.nextLine();
                Scanner scanner = new Scanner(currentLine);
                int id = scanner.nextInt();
                double x = scanner.nextDouble();
                double y = scanner.nextDouble();
                points.add(new Point(id, x, y));
            }
        } catch (FileNotFoundException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
        return points;
    }

    public void createOutputFile(ArrayList<Point> lastRoute) {
        try {
            String outputFileName = filename.substring(0, filename.length() - 4) + "Output.txt";
            File file = new File(outputFileName);
            FileWriter myWriter = new FileWriter(file);
            double cost = Algorithms.getHamiltonianCycleCost(lastRoute);
            NumberFormat formatter = new DecimalFormat("#0");
            myWriter.write(formatter.format(cost) + "\n");
            for (int i = 0; i < lastRoute.size(); i++) {
                myWriter.write(lastRoute.get(i).id + "\n");
            }
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}