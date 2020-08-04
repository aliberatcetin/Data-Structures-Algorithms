public class Point {

    public double x,y;
    public int id;
    public Point(int id,double x, double y) {
        this.x = x;
        this.y = y;
        this.id=id;
    }

    public double distance(Point point){
        return Math.sqrt( Math.pow(point.x-this.x,2) + Math.pow(point.y-this.y,2) );
    }

    @Override
    public String toString() {
        return "Point{" +
                "x=" + x +
                ", y=" + y +
                ", id=" + id +
                '}';
    }
}
