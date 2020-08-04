import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

public class Algorithms {


    public static double getHamiltonianCycleCost(ArrayList<Point> points) {
        double cost = 0;
        int size = points.size();
        for (int i = 0; i < size - 1; i++) {
            double currentCost = points.get(i).distance(points.get(i + 1));
            cost += Math.round(currentCost);
        }
        cost += points.get(0).distance(points.get(points.size() - 1));
        return cost;
    }

    public static ArrayList<Point> getBestNearest(ArrayList<Point> points){
        double cost = Double.MAX_VALUE;
        ArrayList<Point> points2=points;
        ArrayList<Point> temp;
        int iteration = points.size()>1000 ? 1000 : points.size();
        System.out.println(iteration);
        for(int i=0;i<iteration;i++){
            temp = nearestNeighbourAlgorithmWithStartingNode(points,i);
            double currentCost = getHamiltonianCycleCost(temp);
            if(currentCost<cost){
                cost=currentCost;
                points2=temp;
            }
        }
        return points2;
    }


    public static ArrayList<Point> nearestNeighbourAlgorithmWithStartingNode(ArrayList<Point> points2, int startPoint) {

        ArrayList<Point> points = new ArrayList<Point>(points2);
        Point currentPoint = points.remove(startPoint);
        ArrayList<Point> newRoute = new ArrayList<Point>();
        newRoute.add(currentPoint);
        double currentDistance = Double.MAX_VALUE;

        while (points.size() > 0) {
            int index = 0;
            int size = points.size();
            for (int j = 0; j < size; j++) {
                double tempDistance = points.get(j).distance(currentPoint);
                if (tempDistance < currentDistance) {
                    index = j;
                    currentDistance = tempDistance;
                }
            }
            currentPoint = points.remove(index);
            newRoute.add(currentPoint);
            currentDistance = Double.MAX_VALUE;
        }
        return newRoute;
    }

    public static ArrayList<Point> twoOptOptimization(ArrayList<Point> initialRoute) {
        boolean swap = true;
        int size = initialRoute.size();
        int iterationCounter = 0;
        int swapCounter = 0;
        int comparisonCounter = 0;

        while (swap) {
            iterationCounter++;
            swap = false;
            for (int i = 0; i < size - 3; i++) {
                for (int j = i + 2; j < size - 1; j++) {
                    comparisonCounter++;
                    if ((initialRoute.get(i).distance(initialRoute.get(i + 1)) + initialRoute.get(j).distance(initialRoute.get(j + 1)))
                            > (initialRoute.get(i).distance(initialRoute.get(j)) + initialRoute.get(i + 1).distance(initialRoute.get(j + 1)))) {
                        initialRoute = swapAndArrange(initialRoute, i, j);
                        swap = true;
                        swapCounter++;
                    }
                }
            }
        }

        System.out.println("-------------------------------------------------------");
        System.out.println("2-OPT Algorithm completed.");
        System.out.println("Number of iterations : " + iterationCounter);
        System.out.println("Number of comparison : " + comparisonCounter);
        System.out.println("Number of improvements : " + swapCounter);
        System.out.println("-------------------------------------------------------");
        return initialRoute;
    }

    public static ArrayList<Point> swapAndArrange(ArrayList<Point> nnr, int a, int c) {
        ArrayList<Point> newRoute = new ArrayList<>();
        for (int k = 0; k <= a; k++) {
            newRoute.add(nnr.get(k));
        }
        for (int k = c; k > a; k--) {
            newRoute.add(nnr.get(k));
        }
        for (int k = c + 1; k < nnr.size(); k++) {
            newRoute.add(nnr.get(k));
        }
        return newRoute;
    }

    public static HashMap<Integer, Integer> getFarthestPair(ArrayList<Point> points) {
        int size = points.size();
        HashMap<Integer, Integer> pointPair = new HashMap<>();
        double currentDistance = Double.MIN_VALUE;
        int first = 0, second = 1;
        for (int i = 0; i < size - 1; i++) {
            for (int j = i + 1; j < size; j++) {
                if (currentDistance < points.get(i).distance(points.get(j))) {
                    currentDistance = points.get(i).distance(points.get(j));
                    first = i;
                    second = j;
                }
            }
        }
        pointPair.put(first, second);
        return pointPair;
    }

    static double shortestDistanceBetweenPointAndEdge(double x1, double y1, double x2, double y2, double x, double y) {
        double A = x - x1;
        double B = y - y1;
        double C = x2 - x1;
        double D = y2 - y1;

        double dot = A * C + B * D;
        double len_sq = C * C + D * D;
        double param = -1;
        if (len_sq != 0) //in case of 0 length line
            param = dot / len_sq;

        double xx, yy;

        if (param < 0) {
            xx = x1;
            yy = y1;
        } else if (param > 1) {
            xx = x2;
            yy = y2;
        } else {
            xx = x1 + param * C;
            yy = y1 + param * D;
        }

        double dx = x - xx;
        double dy = y - yy;
        return Math.sqrt(dx * dx + dy * dy);
    }

    public static ArrayList<Point> fartheshInsertionAlgorithm(ArrayList<Point> initialRoute) {

        ArrayList<Point> points = new ArrayList<>(initialRoute);
        HashMap<Integer, Integer> pointPair = getFarthestPair(points);
        ArrayList<Point> newRoute = new ArrayList<>();

        int first = pointPair.keySet().iterator().next();
        Point firstPoint = points.remove(first);
        ;
        newRoute.add(firstPoint);
        int second = pointPair.get(first);
        Point secondPoint = points.remove(second - 1);
        newRoute.add(secondPoint);

        double farthest = 0;
        int index = 0;
        int size = points.size();
        for (int i = 0; i < size; i++) {
            Point currentPoint = points.get(i);
            double currentDistance = shortestDistanceBetweenPointAndEdge(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y, currentPoint.x, currentPoint.y);
            if (farthest < currentDistance) {
                farthest = currentDistance;
                index = i;
            }
        }
        newRoute.add(points.remove(index));

        while (points.size() != 0) {
            farthest = Double.MIN_VALUE;
            int pointSize = points.size();
            int newRouteSize = newRoute.size();

            for (int i = 0; i < pointSize; i++) {
                for (int j = 0; j < newRouteSize; j++) {
                    if (farthest < points.get(i).distance(newRoute.get(j))) {
                        index = i;
                        farthest = points.get(i).distance(newRoute.get(j));
                    }
                }
            }


            double currentDistance = Double.MAX_VALUE;
            int edgeIndex = 0;
            double newDistance;

            for (int i = 0; i < newRouteSize - 1; i++) {
                firstPoint = newRoute.get(i);
                secondPoint = newRoute.get(i + 1);
                Point currentPoint = points.get(index);

                newDistance = shortestDistanceBetweenPointAndEdge(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y, currentPoint.x, currentPoint.y);

                if (currentDistance > newDistance) {
                    currentDistance = newDistance;
                    edgeIndex = i;
                }
            }
            firstPoint = newRoute.get(newRouteSize - 1);
            secondPoint = newRoute.get(0);
            Point currentPoint = points.get(index);

            newDistance = shortestDistanceBetweenPointAndEdge(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y, currentPoint.x, currentPoint.y);
            if (currentDistance > newDistance) {
                edgeIndex = newRouteSize - 1;
            }
            newRoute.add(edgeIndex + 1, points.remove(index));
        }
        return newRoute;
    }

    public static void validateAPath(ArrayList<Point> cities) {
        Set<Point> set = new HashSet<>(cities);
        if (set.size() == cities.size()) {
            System.out.println("This tour is valid");
        } else {
            System.out.println("This tour is invalid");
        }
    }

}
